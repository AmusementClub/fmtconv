# https://www.vapoursynth.com/doc/packaging.html
#
# python -m venv .venv
# .venv\Scripts\activate
# pip install build
# python -m build --wheel
# pip install twine
# twine upload dist/*

from __future__ import annotations

import os
import re
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Any

from hatchling.builders.hooks.plugin.interface import BuildHookInterface
from packaging import tags

# Visual Studio 2022
DEFAULT_PLATFORM_TOOLSET = "v143"
DEFAULT_VS_VERSION = "17"


class CustomHook(BuildHookInterface[Any]):
    """
    Custom build hook to compile the project and package the resulting binaries.
    """

    target_dir = Path("vapoursynth/plugins")

    def initialize(self, version: str, build_data: dict[str, Any]) -> None:
        """
        Called before the build process starts.
        Sets build metadata and executes the compilation.
        """
        build_data["pure_python"] = False
        build_data["tag"] = f"py3-none-{next(tags.platform_tags())}"

        # On Windows: invokes MSBuild via vswhere to build the Visual Studio solution.
        # On Linux/macOS: runs autogen.sh + ./configure + make inside build/unix.
        if sys.platform == "win32":
            built_files = self.build_windows()
        elif sys.platform in ("linux", "darwin"):
            built_files = self.build_unix()
        else:
            raise RuntimeError(f"Unsupported build platform: {sys.platform}")

        self.target_dir.mkdir(parents=True, exist_ok=True)
        for fp in built_files:
            print(f"[hatch_build] Staging {fp}")
            shutil.copy2(fp, self.target_dir / fp.name)

    def finalize(self, version: str, build_data: dict[str, Any], artifact_path: str) -> None:
        """
        Called after the build process finishes.
        Cleans up temporary build artifacts.
        """
        shutil.rmtree(self.target_dir.parent, ignore_errors=True)

    def discover_msvc(self) -> tuple[str, str]:
        """
        Search for MSBuild and the best available PlatformToolset.
        Returns "msbuild.exe", "v1xx"
        """
        platform_toolset = DEFAULT_PLATFORM_TOOLSET

        vswhere = (
            Path(os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)"))
            / "Microsoft Visual Studio"
            / "Installer"
            / "vswhere.exe"
        )

        if not vswhere.is_file():
            raise RuntimeError("vswhere.exe not found. Please install Visual Studio or Build Tools.")

        # Find MSBuild specifically
        result_mb = subprocess.run(
            [
                vswhere,
                "-latest",
                "-products",
                "*",
                "-requires",
                "Microsoft.Component.MSBuild",
                "-find",
                r"MSBuild\**\Bin\MSBuild.exe",
            ],
            capture_output=True,
            text=True,
            check=True,
        )
        msbuild_path = next((p.strip() for p in result_mb.stdout.splitlines() if p.strip()), None)

        if not msbuild_path:
            raise RuntimeError("MSBuild.exe not found. Please install Visual Studio or Build Tools.")

        # Discover the best toolset via vswhere find
        result_ts = subprocess.run(
            [
                vswhere,
                "-latest",
                "-products",
                "*",
                "-find",
                r"MSBuild\Microsoft\VC\v*\Platforms\x64\PlatformToolsets",
            ],
            capture_output=True,
            text=True,
            check=True,
        )
        toolset_dirs = [p.strip() for p in result_ts.stdout.splitlines() if p.strip()]

        if toolset_dirs:
            # Sort to get the highest MSBuild internal version
            toolset_dir = Path(sorted(toolset_dirs, reverse=True)[0])
            if toolset_dir.is_dir():
                toolsets = [d.name for d in toolset_dir.iterdir() if d.is_dir() and re.match(r"v\d+", d.name)]
                if toolsets:
                    platform_toolset = sorted(toolsets, reverse=True)[0]

        return msbuild_path, platform_toolset

    def build_windows(self) -> list[Path]:
        solution = Path(self.root) / "build" / "win" / "fmtconv.sln"

        if not solution.is_file():
            raise FileNotFoundError(f"Solution not found: {solution}")

        msbuild, platform_toolset = self.discover_msvc()
        print(f"[hatch_build] MSBuild: {msbuild}", file=sys.stderr)
        print(f"[hatch_build] PlatformToolset: {platform_toolset}", file=sys.stderr)

        print(f"[hatch_build] Building with PlatformToolset={platform_toolset}", file=sys.stderr)
        print(f"[hatch_build] Building: {solution}", file=sys.stderr)

        subprocess.run(
            [
                msbuild,
                solution,
                "/p:Configuration=Release",
                "/p:Platform=x64",
                f"/p:PlatformToolset={platform_toolset}",
                "/m",
            ],
            check=True,
        )

        output_dir = Path(self.root) / "build" / "win" / "fmtconv" / "Releasex64"
        dlls = list(output_dir.glob("*.dll"))
        if not dlls:
            raise FileNotFoundError(f"No DLLs found in {output_dir} after MSBuild succeeded.")
        return dlls

    def build_unix(self) -> list[Path]:
        """
        Build via autotools (autogen.sh -> ./configure -> make) inside build/unix
        and return paths to produced shared libraries (.so / .dylib).
        """
        unix_dir = Path(self.root) / "build" / "unix"

        autogen = unix_dir / "autogen.sh"
        if not autogen.is_file():
            raise FileNotFoundError(f"autogen.sh not found: {autogen}")

        print("[hatch_build] Running autogen.sh ...", file=sys.stderr)
        subprocess.run(["sh", str(autogen)], cwd=unix_dir, check=True)

        print("[hatch_build] Running ./configure ...", file=sys.stderr)
        subprocess.run(["./configure"], cwd=unix_dir, check=True)

        print("[hatch_build] Running make ...", file=sys.stderr)
        subprocess.run(["make", f"-j{(os.cpu_count() or 1) + 1}"], cwd=unix_dir, check=True)

        libs_dir = unix_dir / ".libs"
        extensions = (".so", ".dylib")
        libs = [p for p in libs_dir.glob("libfmtconv*") if p.suffix in extensions and ".la" not in p.name]
        if not libs:
            raise FileNotFoundError(f"No shared libraries found in {libs_dir} after make.")
        return libs
