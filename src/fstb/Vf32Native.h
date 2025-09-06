/*****************************************************************************

        Vf32Native.h
        Author: Laurent de Soras, 2024

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://www.wtfpl.net/ for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (fstb_Vf32Native_HEADER_INCLUDED)
#define fstb_Vf32Native_HEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/def.h"

#if ! defined (fstb_HAS_SIMD)
	#include <array>
#elif (fstb_ARCHI == fstb_ARCHI_X86)
	#include <emmintrin.h>
#elif (fstb_ARCHI == fstb_ARCHI_ARM)
	#include <arm_neon.h>
#else
	#error
#endif



namespace fstb
{



#if ! defined (fstb_HAS_SIMD)

typedef std::array <float, 4> Vf32Native;

#elif fstb_ARCHI == fstb_ARCHI_X86

typedef __m128      Vf32Native;

#elif fstb_ARCHI == fstb_ARCHI_ARM

typedef float32x4_t Vf32Native;

#else // fstb_ARCHI
#error
#endif // fstb_ARCHI



}  // namespace fstb



//#include "fstb/Vf32Native.hpp"



#endif   // fstb_Vf32Native_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
