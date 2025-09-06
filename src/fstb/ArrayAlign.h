/*****************************************************************************

        ArrayAlign.h
        Author: Laurent de Soras, 2010

Static array of fixed length containng aligned objects, even if the array
itself is on a nonaligned memory.
Probably not as useful as before now we have alignas().

Template parameters:

- T: Element type. Requires:
	T::T ();
	T::~T ();

- LEN: Number of elements, > 0

- AL: Desired memory alignement, in bytes. > 0 and must a power of 2.

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (fstb_ArrayAlign_HEADER_INCLUDED)
#define	fstb_ArrayAlign_HEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <climits>
#include <cstdint>



namespace fstb
{



template <typename T, long LEN, long AL>
class ArrayAlign
{

	static_assert (LEN > 0, "Number of elements must be > 0");
	static_assert (AL > 0, "Alignment must be strictly positive");
	static_assert ((AL & -AL) == AL, "Alignment must be a power of 2");

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef	T	Element;

	static constexpr int _nbr_elt   = LEN;
	static constexpr int _alignment = AL;

	               ArrayAlign ();
	               ArrayAlign (const ArrayAlign &other);
	               ~ArrayAlign ();

	ArrayAlign &   operator = (const ArrayAlign &other);

	inline const Element &
	               operator [] (long pos) const noexcept;
	inline Element &
	               operator [] (long pos) noexcept;

	inline const Element *
	               data () const noexcept;
	inline Element *
	               data () noexcept;

	static inline long
	               size () noexcept;
	static inline long
	               length () noexcept;
	static inline long
	               get_alignment () noexcept;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	static constexpr int _elt_size_byte = int (sizeof (Element) * CHAR_BIT / 8);

	uint8_t        _data [_nbr_elt * _elt_size_byte + _alignment - 1];

	// Pointer on aligned data. Set by the constructor.
	Element *      _data_ptr = nullptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const ArrayAlign &other) const = delete;
	bool           operator != (const ArrayAlign &other) const = delete;

};	// class ArrayAlign



}	// namespace fstb



#include "fstb/ArrayAlign.hpp"



#endif	// fstb_ArrayAlign_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
