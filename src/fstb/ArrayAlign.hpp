/*****************************************************************************

        ArrayAlign.hpp
        Author: Laurent de Soras, 2012

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (fstb_ArrayAlign_CODEHEADER_INCLUDED)
#define	fstb_ArrayAlign_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/def.h"

#include <new>
#include <type_traits>

#include <cassert>
#include <cstddef>



namespace fstb
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <typename T, long LEN, long AL>
ArrayAlign <T, LEN, AL>::ArrayAlign ()
{
	_data_ptr = reinterpret_cast <Element *> (
		reinterpret_cast <ptrdiff_t> (&_data [_alignment - 1]) & -_alignment
	);

	for (long pos = 0; pos < _nbr_elt; ++pos)
	{
		new (&_data_ptr [pos]) Element ();
	}
}



template <typename T, long LEN, long AL>
ArrayAlign <T, LEN, AL>::ArrayAlign (const ArrayAlign <T, LEN, AL> &other)
{
	_data_ptr = reinterpret_cast <Element *> (
		reinterpret_cast <ptrdiff_t> (&_data [_alignment - 1]) & -_alignment
	);

	for (long pos = 0; pos < _nbr_elt; ++pos)
	{
		new (&_data_ptr [pos]) Element (other [pos]);
	}
}



template <typename T, long LEN, long AL>
ArrayAlign <T, LEN, AL>::~ArrayAlign ()
{
	assert (_data_ptr != nullptr);

	if constexpr (! std::is_trivially_destructible_v <Element>)
	{
		for (long pos = 0; pos < _nbr_elt; ++pos)
		{
			Element *   ptr = reinterpret_cast <Element *> (&_data_ptr [pos]);
			ptr->~T ();
		}
	}
}



template <typename T, long LEN, long AL>
ArrayAlign <T, LEN, AL> &	ArrayAlign <T, LEN, AL>::operator = (const ArrayAlign <T, LEN, AL> &other)
{
	if (this != &other)
	{
		for (long pos = 0; pos < _nbr_elt; ++pos)
		{
			(*this) [pos] = other [pos];
		}
	}

	return *this;
}



template <typename T, long LEN, long AL>
const typename ArrayAlign <T, LEN, AL>::Element &	ArrayAlign <T, LEN, AL>::operator [] (long pos) const noexcept
{
	assert (_data_ptr != nullptr);
	assert (pos >= 0);
	assert (pos < _nbr_elt);

	return _data_ptr [pos];
}



template <typename T, long LEN, long AL>
typename ArrayAlign <T, LEN, AL>::Element &	ArrayAlign <T, LEN, AL>::operator [] (long pos) noexcept
{
	assert (_data_ptr != nullptr);
	assert (pos >= 0);
	assert (pos < _nbr_elt);

	return _data_ptr [pos];
}



template <typename T, long LEN, long AL>
const typename ArrayAlign <T, LEN, AL>::Element *	ArrayAlign <T, LEN, AL>::data () const noexcept
{
	assert (_data_ptr != nullptr);

	return _data_ptr;
}



template <typename T, long LEN, long AL>
typename ArrayAlign <T, LEN, AL>::Element *	ArrayAlign <T, LEN, AL>::data () noexcept
{
	assert (_data_ptr != nullptr);

	return _data_ptr;
}



template <typename T, long LEN, long AL>
long	ArrayAlign <T, LEN, AL>::size () noexcept
{
	return _nbr_elt;
}



template <typename T, long LEN, long AL>
long	ArrayAlign <T, LEN, AL>::length () noexcept
{
	return _nbr_elt;
}



template <typename T, long LEN, long AL>
long	ArrayAlign <T, LEN, AL>::get_alignment () noexcept
{
	return _alignment;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}	// namespace fstb



#endif	// fstb_ArrayAlign_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
