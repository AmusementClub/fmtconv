/*****************************************************************************

        Vs32.h
        Author: Laurent de Soras, 2021

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://www.wtfpl.net/ for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (fstb_Vs32_HEADER_INCLUDED)
#define fstb_Vs32_HEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/bit_cast.h"
#include "fstb/Vs32Native.h"

#include <tuple>



namespace fstb
{



class Vf32;
class Vu32;

class Vs32
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static constexpr int _len_l2 = 2;
	static constexpr int _length = 1 << _len_l2;
	typedef int32_t Scalar;
	typedef Vs32Native Native;

	               Vs32 ()                        = default;
   fstb_FORCEINLINE
	               Vs32 (Vs32Native a) noexcept : _x { a } {}
   explicit fstb_FORCEINLINE // Defined in Vx32_conv.hpp
	               Vs32 (const Vf32 &f) noexcept;
   explicit fstb_FORCEINLINE // Defined in Vx32_conv.hpp
	               Vs32 (const Vu32 &u) noexcept;
   explicit fstb_FORCEINLINE
	               Vs32 (Scalar a) noexcept;
	explicit fstb_FORCEINLINE
	               Vs32 (Scalar a0, Scalar a1, Scalar a2, Scalar a3) noexcept;
	explicit fstb_FORCEINLINE
	               Vs32 (const std::tuple <Scalar, Scalar, Scalar, Scalar> &a) noexcept;
	               Vs32 (const Vs32 &other)       = default;
	               Vs32 (Vs32 &&other)            = default;
	               ~Vs32 ()                       = default;
	Vs32 &         operator = (const Vs32 &other) = default;
	Vs32 &         operator = (Vs32 &&other)      = default;

	template <typename MEM>
	fstb_FORCEINLINE void
	               store (MEM *ptr) const noexcept;
	template <typename MEM>
	fstb_FORCEINLINE void
	               storeu (MEM *ptr) const noexcept;
	template <typename MEM>
	fstb_FORCEINLINE void
	               storeu_part (MEM *ptr, int n) const noexcept;

	fstb_FORCEINLINE
	               operator Vs32Native () const noexcept { return _x; }
	fstb_FORCEINLINE explicit
	               operator bool () const noexcept;

	fstb_FORCEINLINE Vs32 &
	               operator += (const Vs32Native &other) noexcept;
	fstb_FORCEINLINE Vs32 &
	               operator -= (const Vs32Native &other) noexcept;
	fstb_FORCEINLINE Vs32 &
	               operator *= (const Vs32Native &other) noexcept;

	fstb_FORCEINLINE Vs32 &
	               operator &= (const Vs32Native &other) noexcept;
	fstb_FORCEINLINE Vs32 &
	               operator |= (const Vs32Native &other) noexcept;
	fstb_FORCEINLINE Vs32 &
	               operator ^= (const Vs32Native &other) noexcept;

	fstb_FORCEINLINE Vs32 &
	               operator <<= (int imm) noexcept;
	fstb_FORCEINLINE Vs32 &
	               operator >>= (int imm) noexcept;

	fstb_FORCEINLINE Vs32
	               operator - () const noexcept;
	fstb_FORCEINLINE Vs32
	               operator ~ () const noexcept;
	fstb_FORCEINLINE Vs32
	               is_lt_0 () const noexcept;
	fstb_FORCEINLINE Vs32
	               reverse () const noexcept;

	template <int SHIFT>
	fstb_FORCEINLINE Vs32
	               rotate () const noexcept;
	template <int POS>
	fstb_FORCEINLINE int32_t
	               extract () const noexcept;
	template <int POS>
	fstb_FORCEINLINE Vs32
	               insert (int32_t val) const noexcept;
	template <int POS>
	fstb_FORCEINLINE Vs32
	               spread () const noexcept;

	fstb_FORCEINLINE std::tuple <int32_t, int32_t, int32_t, int32_t>
	               explode () const noexcept;

	fstb_FORCEINLINE int32_t
	               sum_h () const noexcept;
	fstb_FORCEINLINE int32_t
	               min_h () const noexcept;
	fstb_FORCEINLINE int32_t
	               max_h () const noexcept;

	fstb_FORCEINLINE bool
	               and_h () const noexcept;
	fstb_FORCEINLINE bool
	               or_h () const noexcept;
	fstb_FORCEINLINE unsigned int
	               movemask () const noexcept;
	fstb_FORCEINLINE int
	               count_bits () const noexcept;

	static fstb_FORCEINLINE Vs32
	               zero () noexcept;
	static fstb_FORCEINLINE Vs32
	               all1 () noexcept;
	static fstb_FORCEINLINE Vs32
	               set_mask (bool m0, bool m1, bool m2, bool m3) noexcept;
	template <int POS>
	static fstb_FORCEINLINE Vs32
	               compose (Vs32 a, Vs32 b) noexcept;

	template <typename MEM>
	static fstb_FORCEINLINE Vs32
	               load (const MEM *ptr) noexcept;
	template <typename MEM>
	static fstb_FORCEINLINE Vs32
	               loadu (const MEM *ptr) noexcept;

	static fstb_FORCEINLINE Vs32 *
	               ptr_v (Scalar *ptr) noexcept;
	static fstb_FORCEINLINE const Vs32 *
	               ptr_v (const Scalar *ptr) noexcept;
	static fstb_FORCEINLINE Scalar *
	               ptr_s (Vs32 *ptr) noexcept;
	static fstb_FORCEINLINE const Scalar *
	               ptr_s (const Vs32 *ptr) noexcept;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

#if ! defined (fstb_HAS_SIMD)
public:
#endif
	Vs32Native  _x;
private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class Vs32



/*\\\ GLOBAL OPERATORS AND FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



fstb_FORCEINLINE Vs32 operator + (Vs32 lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator - (Vs32 lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator * (Vs32 lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator & (Vs32 lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator | (Vs32 lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator ^ (Vs32 lhs, const Vs32 &rhs) noexcept;

template <typename T>
fstb_FORCEINLINE Vs32 operator << (Vs32 lhs, T rhs) noexcept;
template <typename T>
fstb_FORCEINLINE Vs32 operator >> (Vs32 lhs, T rhs) noexcept;

fstb_FORCEINLINE Vs32 operator == (const Vs32 &lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator != (const Vs32 &lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator <  (const Vs32 &lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator <= (const Vs32 &lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator >  (const Vs32 &lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 operator >= (const Vs32 &lhs, const Vs32 &rhs) noexcept;

fstb_FORCEINLINE Vs32 abs (const Vs32 &v) noexcept;
fstb_FORCEINLINE Vs32 min (const Vs32 &lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 max (const Vs32 &lhs, const Vs32 &rhs) noexcept;
fstb_FORCEINLINE Vs32 limit (const Vs32 &v, const Vs32 &mi, const Vs32 &ma) noexcept;
fstb_FORCEINLINE Vs32 select (const Vs32 &cond, const Vs32 &v_t, const Vs32 &v_f) noexcept;
fstb_FORCEINLINE std::tuple <Vs32, Vs32> swap_if (const Vs32 &cond, Vs32 lhs, Vs32 rhs) noexcept;

// Defined in Vx32_conv.hpp
template <>
fstb_FLATINLINE Vs32 bit_cast (const Vf32 &x) noexcept;
template <>
fstb_FLATINLINE Vs32 bit_cast (const Vu32 &x) noexcept;



}  // namespace fstb



#include "fstb/Vs32.hpp"



#endif   // fstb_Vs32_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
