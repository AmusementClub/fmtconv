/*****************************************************************************

        bit_cast.h
        Author: Laurent de Soras, 2024

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (fstb_bit_cast_HEADER_INCLUDED)
#define	fstb_bit_cast_HEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/def.h"

#include <type_traits>

#if fstb_COMPILER == fstb_COMPILER_MSVC
	// Nothing
#elif (__cplusplus >= 202002L)
	#include <bit>
#else
	#include <cstring>
#endif



namespace fstb
{



// Source: https://en.cppreference.com/w/cpp/numeric/bit_cast
template <typename T, typename F>
fstb_FLATINLINE T	bit_cast (const F &src) noexcept
{
	static_assert (sizeof (T) == sizeof (F), "Data size mismatch");
	static_assert (
		std::is_trivially_copyable_v <F>, "Source must be trivially copiable"
	);
	static_assert (
		std::is_trivially_copyable_v <T>, "Destination must be trivially copiable"
	);
	static_assert (
		std::is_trivially_constructible_v <T>,
		"This implementation additionally requires "
		"destination type to be trivially constructible"
	);

#if fstb_COMPILER == fstb_COMPILER_MSVC

	// Type punning. UB in general, but works on most compilers.
	// Still the fastest method on MSVC.
	const union
	{
		const F        _s;
		T              _d;
	}              u { src };
	return u._d;

#elif (__cplusplus >= 202002L)

	return std::bit_cast <T> (src);

#else

	// Default method, should be safe and optimised out by the compiler.
	T              dst;
   memcpy (&dst, &src, sizeof (T));
	return dst;

#endif
}



}	// namespace fstb



//#include "fstb/bit_cast.hpp"



#endif	// fstb_bit_cast_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
