@echo off
setlocal

:: Check if virtual environment exists, if not create it
if not exist ".venv" (
    echo [.venv] not found. Creating virtual environment...
    python -m venv .venv
)

:: Activate the virtual environment
echo Activating .venv...
call .venv\Scripts\activate

:: Ensure build and twine are installed
echo Installing/Updating build and twine...
pip install -U build twine

:: Build the package
echo Building distributions...
python -m build

:: Upload to PyPI
echo Uploading to PyPI...
twine upload dist/*

echo Done.
pause
