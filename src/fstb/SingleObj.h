/*****************************************************************************

        SingleObj.h
        Author: Laurent de Soras, 2015

Wrapper around an object stored on the heap using a custom allocator.
Uses the pointer semantic.

Template parameters:

- T: class to wrap. Requires:
	any ctor
	T::~T ();

- A: allocator class for the object. Requires allocate() and deallocate().

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (fstb_SingleObj_HEADER_INCLUDED)
#define	fstb_SingleObj_HEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/AllocAlign.h"



namespace fstb
{



template <class T, class A = AllocAlign <T, 16> >
class SingleObj
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	template <typename... U>
	               SingleObj (U... args);
	virtual        ~SingleObj ();

	T *            operator -> () const noexcept;
	T &            operator * () const noexcept;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	A              _allo;
	T *            _obj_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               SingleObj (const SingleObj <T, A> &other)         = delete;
	               SingleObj (const SingleObj <T, A> &&other)        = delete;
	SingleObj <T, A> &
	               operator = (const SingleObj <T, A> &other)        = delete;
	SingleObj <T, A> &
	               operator = (const SingleObj <T, A> &&other)       = delete;
	bool           operator == (const SingleObj <T, A> &other) const = delete;
	bool           operator != (const SingleObj <T, A> &other) const = delete;

};	// class SingleObj



}	// namespace fstb



#include "fstb/SingleObj.hpp"



#endif	// fstb_SingleObj_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
