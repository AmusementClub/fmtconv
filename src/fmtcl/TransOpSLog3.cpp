/*****************************************************************************

        TransOpSLog3.cpp
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fmtcl/TransOpSLog3.h"

#include <algorithm>

#include <cassert>
#include <cmath>



namespace fmtcl
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



TransOpSLog3::TransOpSLog3 (bool inv_flag)
:	_inv_flag (inv_flag)
{
	// Nothing
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



double	TransOpSLog3::do_convert (double x) const
{
	if (_inv_flag)
	{
		x = zoom_out (x);
		x = log_to_lin (x);
	}
	else
	{
		x = lin_to_log (x);
		x = zoom_in (x);
	}

	return x;
}



TransOpInterface::LinInfo	TransOpSLog3::do_get_info () const
{
	return {
		Type::OETF,
		Range::UNDEF,
		log_to_lin (1.0),
		log_to_lin (_white / _range),
		0.0, 0.0
	};
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
convert() prototype maps x in [0.0 ; 1.0] to the limited range on the "coded"
side (usually 16-235 in 8 bits, or 64-940 in 10 bits) so 0.0 is black and 1.0
white. However S-Log3 formulas use [0.0 ; 1.0] as full-range equivalent, so
we have to stretch the coded side. From the transfer() user perspective, this
requires specifying the coded range as limited.
*/

double	TransOpSLog3::zoom_out (double x) noexcept
{
	return (x * (_top - _bot) + _bot) / _range;
}



double	TransOpSLog3::zoom_in (double x) noexcept
{
	return (x * _range - _bot) / (_top - _bot);
}



double	TransOpSLog3::log_to_lin (double x) noexcept
{
	return
		  (x < 171.2102946929 / _range)
		? (x * _range - _black) * 0.01125000 / (171.2102946929 - _black)
		: (pow (10, (x * _range - _grey) / 261.5)) * (0.18 + 0.01) - 0.01;
}



double	TransOpSLog3::lin_to_log (double x) noexcept
{
	return
		  (x < 0.01125000)
		? (x * (171.2102946929 - _black) / 0.01125000 + _black) / _range
		: (_grey + log10 ((x + 0.01) / (0.18 + 0.01)) * 261.5) / _range;
}



}	// namespace fmtcl



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
