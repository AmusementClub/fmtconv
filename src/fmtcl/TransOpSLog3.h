/*****************************************************************************

        TransOpSLog3.h
        Author: Laurent de Soras, 2016

Ref:
Technical Summary for S-Gamut3.Cine/S-Log3 and S-Gamut3/S-Log3
https://pro.sony/s3/cms-static-content/uploadfile/06/1237494271406.pdf

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (fmtcl_TransOpSLog3_HEADER_INCLUDED)
#define	fmtcl_TransOpSLog3_HEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fmtcl/TransOpInterface.h"



namespace fmtcl
{



class TransOpSLog3
:	public TransOpInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	explicit       TransOpSLog3 (bool inv_flag);
	virtual        ~TransOpSLog3 () {}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	// TransOpInterface
	double         do_convert (double x) const override;
	LinInfo        do_get_info () const override;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	static constexpr double _range = 1023.0;
	static constexpr double _bot   =   64.0;
	static constexpr double _black =   95.0;
	static constexpr double _grey  =  420.0;
	static constexpr double _white =  598.0;
	static constexpr double _top   =  940.0;

	static double  zoom_out (double x) noexcept;
	static double  zoom_in (double x) noexcept;

	static double  log_to_lin (double x) noexcept;
	static double  lin_to_log (double x) noexcept;

	const bool     _inv_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               TransOpSLog3 ()                               = delete;
	               TransOpSLog3 (const TransOpSLog3 &other)      = delete;
	TransOpSLog3 & operator = (const TransOpSLog3 &other)        = delete;
	bool           operator == (const TransOpSLog3 &other) const = delete;
	bool           operator != (const TransOpSLog3 &other) const = delete;

};	// class TransOpSLog3



}	// namespace fmtcl



//#include "fmtcl/TransOpSLog3.hpp"



#endif	// fmtcl_TransOpSLog3_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
