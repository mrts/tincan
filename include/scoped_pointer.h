#ifndef SCOPED_POINTER_H__
#define SCOPED_POINTER_H__

//  This file is a simplified, in-tree copy of scoped_pointer.hpp from the Boost
//  libraries.
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//  Copyright (c) 2011 Mart Somermaa (modifications for Linde)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  http://www.boost.org/libs/smart_ptr/scoped_pointer.htm
//

//  scoped_pointer mimics a built-in pointer except that it guarantees deletion
//  of the object pointed to, either on destruction of the scoped_pointer or via
//  an explicit reset(). scoped_pointer is a simple solution for simple needs;
//  use shared_ptr or std::auto_ptr if your needs are more complex.

#include "release_assert.h"

#include <memory>

namespace tincan
{

template<class T> class scoped_pointer // noncopyable
{
private:
    typedef scoped_pointer<T> this_type;

public:
    explicit scoped_pointer(T* p = 0): px(p)
    {
    }

    explicit scoped_pointer(std::auto_ptr<T> p): px(p.release())
    {
    }

    this_type& operator=(std::auto_ptr<T> p)
    {
        this->reset(p.release());
        return *this;
    }

    ~scoped_pointer()
    {
        delete px;
    }

    void reset(T* p = 0)
    {
        RELEASE_ASSERT(p == 0 || p != px, "Self-reset in scoped_pointer"); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T& operator*() const
    {
        RELEASE_ASSERT(px != 0, "Dereferencing null pointer in scoped_pointer");
        return *px;
    }

    T* operator->() const
    {
        RELEASE_ASSERT(px != 0, "Dereferencing null pointer in scoped_pointer");
        return px;
    }

    T* get() const
    {
        return px;
    }

    operator bool() const
    {
        return px != 0;
    }

    void swap(scoped_pointer& b)
    {
        T* tmp = b.px;
        b.px = px;
        px = tmp;
    }

private:

    T* px;

    // noncopyable
    scoped_pointer(const scoped_pointer&);
    scoped_pointer& operator=(const scoped_pointer&);

    void operator==(const scoped_pointer&) const;
    void operator!=(const scoped_pointer&) const;
};

}

#endif /* SCOPED_POINTER_H */
