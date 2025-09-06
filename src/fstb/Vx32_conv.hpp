/*****************************************************************************

        Vx32_conv.hpp
        Author: Laurent de Soras, 2024

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (fstb_Vx32_conv_CODEHEADER_INCLUDED)
#define fstb_Vx32_conv_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/fnc.h"
#include "fstb/Vf32.h"
#include "fstb/Vs32.h"
#include "fstb/Vu32.h"



namespace fstb
{



//----------------------------------------------------------------------------
// Vf32
//----------------------------------------------------------------------------



Vf32::Vf32 (const Vs32 &s) noexcept
:	_x (
#if ! defined (fstb_HAS_SIMD)
		{
			float (s._x [0]),
			float (s._x [1]),
			float (s._x [2]),
			float (s._x [3])
		}
#elif fstb_ARCHI == fstb_ARCHI_X86
		_mm_cvtepi32_ps (s)
#elif fstb_ARCHI == fstb_ARCHI_ARM
		vcvtq_f32_s32 (s)
#endif // fstb_ARCHI
	)
{
	// Nothing
}

Vf32::Vf32 (const Vu32 &u) noexcept
:	_x (
#if ! defined (fstb_HAS_SIMD)
		{
			float (u._x [0]),
			float (u._x [1]),
			float (u._x [2]),
			float (u._x [3])
		}
#elif fstb_ARCHI == fstb_ARCHI_X86
		[u] () {
			// https://stackoverflow.com/a/34067907
			const auto v   = __m128i (u);
			const auto v2  = _mm_srli_epi32 (v, 1); // v2 = v / 2
			const auto v1  = _mm_and_si128 (v, _mm_set1_epi32 (1)); // v1 = v & 1
			const auto v2f = _mm_cvtepi32_ps (v2);
			const auto v1f = _mm_cvtepi32_ps (v1);
			return _mm_add_ps (_mm_add_ps (v2f, v2f), v1f); // return 2 * v2 + v1
		} ()
#elif fstb_ARCHI == fstb_ARCHI_ARM
		vcvtq_f32_u32 (u)
#endif // fstb_ARCHI
	)
{
	// Nothing
}



template <>
Vf32 bit_cast <Vf32, Vs32> (const Vs32 &x) noexcept
{
#if ! defined (fstb_HAS_SIMD)
	Vf32           result;
	memcpy (&result, &x, sizeof (x));
	return result;
#elif fstb_ARCHI == fstb_ARCHI_X86
	return _mm_castsi128_ps (x);
#elif fstb_ARCHI == fstb_ARCHI_ARM
	return vreinterpretq_f32_s32 (x);
#endif
}

template <>
Vf32 bit_cast <Vf32, Vu32> (const Vu32 &x) noexcept
{
#if ! defined (fstb_HAS_SIMD)
	Vf32           result;
	memcpy (&result, &x, sizeof (x));
	return result;
#elif fstb_ARCHI == fstb_ARCHI_X86
	return _mm_castsi128_ps (x);
#elif fstb_ARCHI == fstb_ARCHI_ARM
	return vreinterpretq_f32_u32 (x);
#endif
}



//----------------------------------------------------------------------------
// Vs32
//----------------------------------------------------------------------------



// Rounding method is unspecified (towards 0 on ARM, depends on MXCSR on x86)
Vs32::Vs32 (const Vf32 &f) noexcept
:	_x (
#if ! defined (fstb_HAS_SIMD)
		{
			int32_t (f._x [0]),
			int32_t (f._x [1]),
			int32_t (f._x [2]),
			int32_t (f._x [3])
		}
#elif fstb_ARCHI == fstb_ARCHI_X86
		_mm_cvtps_epi32 (f)
#elif fstb_ARCHI == fstb_ARCHI_ARM
		vcvtq_s32_f32 (f)
#endif // fstb_ARCHI
	)
{
	// Nothing
}

Vs32::Vs32 (const Vu32 &u) noexcept
:	_x (
#if ! defined (fstb_HAS_SIMD)
		{
			int32_t (u._x [0]),
			int32_t (u._x [1]),
			int32_t (u._x [2]),
			int32_t (u._x [3])
		}
#elif fstb_ARCHI == fstb_ARCHI_X86
		__m128i (u)
#elif fstb_ARCHI == fstb_ARCHI_ARM
		vreinterpretq_s32_u32 (u)
#endif // fstb_ARCHI
	)
{
	// Nothing
}



template <>
Vs32 bit_cast <Vs32, Vf32> (const Vf32 &x) noexcept
{
#if ! defined (fstb_HAS_SIMD)
	Vs32           result;
	memcpy (&result, &x, sizeof (x));
	return result;
#elif fstb_ARCHI == fstb_ARCHI_X86
	return _mm_castps_si128 (x);
#elif fstb_ARCHI == fstb_ARCHI_ARM
	return vreinterpretq_s32_f32 (x);
#endif
}

template <>
Vs32 bit_cast <Vs32, Vu32> (const Vu32 &x) noexcept
{
#if ! defined (fstb_HAS_SIMD)
	Vs32           result;
	memcpy (&result, &x, sizeof (x));
	return result;
#elif fstb_ARCHI == fstb_ARCHI_X86
	return __m128i (x);
#elif fstb_ARCHI == fstb_ARCHI_ARM
	return vreinterpretq_s32_u32 (x);
#endif
}



//----------------------------------------------------------------------------
// Vu32
//----------------------------------------------------------------------------



// Rounding method is unspecified (towards 0 on ARM, depends on MXCSR on x86)
Vu32::Vu32 (const Vf32 &f) noexcept
:	_x (
#if ! defined (fstb_HAS_SIMD)
		{
			uint32_t (f._x [0]),
			uint32_t (f._x [1]),
			uint32_t (f._x [2]),
			uint32_t (f._x [3])
		}
#elif fstb_ARCHI == fstb_ARCHI_X86
		[f] () {
			const auto t = _mm_castps_si128 (f <= Vf32 (float (1u << 31)));
			const auto h = _mm_cvtps_epi32 (_mm_mul_ps (f, _mm_set1_ps (0.5f)));
			const auto z = _mm_andnot_si128 (t, _mm_cvtps_epi32 (f));
			const auto o = _mm_and_si128 (t, _mm_add_epi32 (h, h));
			return _mm_or_si128 (z, o);
		} ()
#elif fstb_ARCHI == fstb_ARCHI_ARM
		vcvtq_u32_f32 (f)
#endif // fstb_ARCHI
	)
{
	// Nothing
}

Vu32::Vu32 (const Vs32 &s) noexcept
:	_x (
#if ! defined (fstb_HAS_SIMD)
		{
			uint32_t (s._x [0]),
			uint32_t (s._x [1]),
			uint32_t (s._x [2]),
			uint32_t (s._x [3])
		}
#elif fstb_ARCHI == fstb_ARCHI_X86
		__m128i (s)
#elif fstb_ARCHI == fstb_ARCHI_ARM
		vreinterpretq_u32_s32 (s)
#endif // fstb_ARCHI
	)
{
	// Nothing
}



template <>
Vu32 bit_cast <Vu32, Vf32> (const Vf32 &x) noexcept
{
#if ! defined (fstb_HAS_SIMD)
	Vu32           result;
	memcpy (&result, &x, sizeof (x));
	return result;
#elif fstb_ARCHI == fstb_ARCHI_X86
	return _mm_castps_si128 (x);
#elif fstb_ARCHI == fstb_ARCHI_ARM
	return vreinterpretq_u32_f32 (x);
#endif
}

template <>
Vu32 bit_cast <Vu32, Vs32> (const Vs32 &x) noexcept
{
#if ! defined (fstb_HAS_SIMD)
	Vu32           result;
	memcpy (&result, &x, sizeof (x));
	return result;
#elif fstb_ARCHI == fstb_ARCHI_X86
	return __m128i (x);
#elif fstb_ARCHI == fstb_ARCHI_ARM
	return vreinterpretq_u32_s32 (x);
#endif
}



}  // namespace fstb



#endif   // fstb_Vx32_conv_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
