import ctypes
import vapoursynth as vs
core = vs.core

 

def show_chroma_ss (colorspace, cplace, dispname, itl, field = 0, transpose = False):
    res   = 8
    scale = 24
    shrp  = True
    bkg_y = 235
    bor   = 4
    
    def build_pattern (n: int, f: vs.VideoFrame) -> vs.VideoFrame:
        f  = f.copy ()
        w  = f.width
        h  = f.height
        it = 1 if itl else 0
        sy = f.get_stride (0)
        py = ctypes.cast (f.get_write_ptr (0), ctypes.POINTER (ctypes.c_ubyte))
        for y in range (h):
            for x in range (w):
                yy = y >> it
                py [sy * y + x] = ((x + yy) & 1) * 128 + 64
        if f.format.num_planes == 3:
            ws = w >> f.format.subsampling_w
            hs = h >> f.format.subsampling_h
            su = f.get_stride (1)
            sv = f.get_stride (2)
            pu = ctypes.cast (f.get_write_ptr (1), ctypes.POINTER (ctypes.c_ubyte))
            pv = ctypes.cast (f.get_write_ptr (2), ctypes.POINTER (ctypes.c_ubyte))
            xy = 0
            for y in range (hs):
                yy = y >> it
                # xy = yy if itl and colorspace == vs.YUV420P8 and cplace == "DV" else 0
                for x in range (ws):
                    xx = x + xy
                    pu [su * y + x] = (xx & 1) * 80 + 88
                    pv [sv * y + x] = (yy & 1) * 80 + 88
        return f
    
    c = core.std.BlankClip (width=res, height=res, format=colorspace, length=1, fpsnum=1)
    if transpose:
        c = core.std.Transpose (c)
    c = core.std.ModifyFrame (c, c, build_pattern)
    
    bkg = [bkg_y] + [128] * (c.format.num_planes - 1)
    c = core.std.AddBorders (c, bor, bor, bor, bor, bkg)
    w = c.width
    h = c.height
    fx = 8 if shrp else 1
    if itl:
        c = core.std.SetFrameProps (c, _FieldBased=vs.FIELD_TOP)
        c = core.std.SeparateFields (c)
        c = core.std.SetFrameProps (c, _FieldBased=vs.FIELD_TOP)
    c = core.fmtc.resample (
        c, w * scale, h * scale, kernel="gaussian", a1=80, fh=fx, fv=fx,
        css="444", fulls=False, fulld=False, cplace=cplace
    )
    if itl:
        c = core.std.SelectEvery (c, cycle=2, offsets=field)
    
    if c.format.num_planes == 3:
        c = core.fmtc.matrix (c, mat="709", fulls=False, fulld=True)
        c = core.fmtc.bitdepth (c, bits=8, dmode=1)
    else:
        c = core.resize.Point (c, format=vs.RGB24)

    cpen = [255 if bkg [0] < 128 else 0] * c.format.num_planes
    bs   = bor * scale
    bf   = (scale + 31) // 32
    br   = bs - bf
    col0 = [  0] * c.format.num_planes
    col1 = [255] * c.format.num_planes
    mask = core.std.BlankClip (c, width=res*scale, height=res*scale, color=col1)
    mask = core.std.AddBorders (mask, bf, bf, bf, bf, color=col0)
    mask = core.std.AddBorders (mask, br, br, br, br, color=col1)
    pen  = core.std.BlankClip (c, color=cpen)
    c    = core.std.MaskedMerge (pen, c, mask)

    b2s  = (bor - 2) * scale
    c    = core.std.Crop (c, b2s, b2s, b2s - scale // 4, b2s + scale // 4)

    pen  = core.std.CropAbs (pen, c.width, c.height)
    ftx  = ""
    if itl:
        ftx = ", " + ("bottom" if (field == 1) else "top") + " field"
    mask = core.std.BlankClip (c, color=[0]*c.format.num_planes)
    mask = core.text.Text (mask, dispname + ftx, alignment=8, scale=(scale+47)//48)
    mask = core.std.Convolution (mask, matrix=[0, 1, 1], divisor=1, mode="h")
    mask = core.std.BinarizeMask (mask)
    c    = core.std.MaskedMerge (c, pen, mask)

    return c



x00 = show_chroma_ss (vs.GRAY8, "MPEG2", "4:0:0", False)
x01 = show_chroma_ss (vs.YUV444P8, "MPEG2", "4:4:4 MPEG-2", False)
r0  = core.std.StackHorizontal ([x00, x01])
x10 = show_chroma_ss (vs.YUV420P8, "MPEG1", "4:2:0 MPEG-1", False)
x11 = show_chroma_ss (vs.YUV420P8, "MPEG2", "4:2:0 MPEG-2", False)
r1  = core.std.StackHorizontal ([x10, x11])
x20 = show_chroma_ss (vs.YUV420P8, "MPEG2", "4:2:0 MPEG-2", True, 0)
x21 = show_chroma_ss (vs.YUV420P8, "MPEG2", "4:2:0 MPEG-2", True, 1)
r2  = core.std.StackHorizontal ([x20, x21])
x30 = show_chroma_ss (vs.YUV420P8, "DV", "4:2:0 DV-PAL", True, 0)
x31 = show_chroma_ss (vs.YUV420P8, "DV", "4:2:0 DV-PAL", True, 1)
r3  = core.std.StackHorizontal ([x30, x31])
x40 = show_chroma_ss (vs.YUV411P8, "DV", "4:1:1 DV-NTSC", True, 0)
x41 = show_chroma_ss (vs.YUV411P8, "DV", "4:1:1 DV-NTSC", True, 1)
r4  = core.std.StackHorizontal ([x40, x41])
x50 = show_chroma_ss (vs.YUV411P8, "DV", "4:1:1 DV-NTSC", False)
x51 = show_chroma_ss (vs.YUV411P8, "DV", "Transposed 4:1:1 DV-NTSC", False, 0, True)
r5  = core.std.StackHorizontal ([x50, x51])
x60 = show_chroma_ss (vs.YUV422P8, "MPEG2", "4:2:2", False)
x61 = show_chroma_ss (vs.YUV422P8, "MPEG2", "Transposed 4:2:2", False, 0, True)
r6  = core.std.StackHorizontal ([x60, x61])
x70 = show_chroma_ss (vs.YUV422P8, "MPEG2", "4:2:2", True, 0)
x71 = show_chroma_ss (vs.YUV422P8, "MPEG2", "4:2:2", True, 1)
r7  = core.std.StackHorizontal ([x70, x71])
x80 = show_chroma_ss (vs.YUV422P8, "MPEG1", "4:2:2 MPEG-1", False)
x81 = show_chroma_ss (vs.YUV420P8, "TL", "4:2:0 Top-left", False)
r8  = core.std.StackHorizontal ([x80, x81])

c   = core.std.StackVertical ([r0, r1, r2, r3, r4, r5, r6, r7, r8])

c.set_output (0)
