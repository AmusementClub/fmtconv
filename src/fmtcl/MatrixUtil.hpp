/*****************************************************************************

        MatrixUtil.hpp
        Author: Laurent de Soras, 2025

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law.You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://www.wtfpl.net/ for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (fmtcl_MatrixUtil_CODEHEADER_INCLUDED)
#define fmtcl_MatrixUtil_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace fmtcl
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <typename T>
T	MatrixUtil::select_target_color_family (ColorSpaceH265 tmp_csp, T cf_invalid, T cf_rgb, T cf_yuv) noexcept
{
	auto           cf_alt = cf_invalid;

	switch (tmp_csp)
	{
	case fmtcl::ColorSpaceH265_RGB:
	case fmtcl::ColorSpaceH265_BT709:
	case fmtcl::ColorSpaceH265_FCC:
	case fmtcl::ColorSpaceH265_BT470BG:
	case fmtcl::ColorSpaceH265_SMPTE170M:
	case fmtcl::ColorSpaceH265_SMPTE240M:
	case fmtcl::ColorSpaceH265_YCGCO:
	case fmtcl::ColorSpaceH265_BT2020NCL:
	case fmtcl::ColorSpaceH265_BT2020CL:
	case fmtcl::ColorSpaceH265_YDZDX:
	case fmtcl::ColorSpaceH265_CHRODERNCL:
	case fmtcl::ColorSpaceH265_CHRODERCL:
	case fmtcl::ColorSpaceH265_ICTCP:
	case fmtcl::ColorSpaceH265_ICTCP_PQ:
	case fmtcl::ColorSpaceH265_ICTCP_HLG:
		cf_alt = cf_yuv;
		break;

	case fmtcl::ColorSpaceH265_LMS:
		cf_alt = cf_rgb;
		break;

	default:
		// Nothing
		break;
	}

	return cf_alt;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace fmtcl



#endif // fmtcl_MatrixUtil_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
