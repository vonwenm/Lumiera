/*
  SCOPEDHOLDER.hpp  -  general purpose wrapper for dealing with ownership problems 
 
  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>
 
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 
*/

/** @file scopedholder.hpp
 ** Some wrappers for coping with ownership problems.
 ** Working with collections of objects, especially in conjunction with
 ** polymorphism, can be challenging when we are bound to care for lifecycle
 ** and ownership for the contained classes. There are several solutions,
 ** including the boost::ptr_container library, the use of shared_ptr
 ** or even a garbage collector. Sometimes the circumstances rather call
 ** for a very simple or lightweight solution though.
 ** 
 ** ScopedPtrHolder is a small extension to boost::scoped_ptr, enabling
 ** to use it within STL containers if we stick to a specific protocol.
 ** The idea is to permit copying as long as the scoped_ptr is empty.
 ** This can be used to allow for extension of the STL container on
 ** demand, i.e. to handle the typical situation of a registry which
 ** is initialised lazily, but only released in a controlled fashion.
 ** 
 ** ScopedHolder implements a similar concept for in-place storage of
 ** noncopyable objects within STL containers.
 ** 
 ** @see scopedholdertest.cpp
 ** @see AllocationCluster usage example
 */


#ifndef LIB_SCOPEDHOLDER_H
#define LIB_SCOPEDHOLDER_H

#include <boost/scoped_ptr.hpp>

#include "common/error.hpp"



namespace lib {
  
  
  
    
  /**
   * Extension to boost::scoped_ptr, allowing copy operations
   * as long as the pointer is still null.
   * @throw error::Logic on attempt to copy otherwise
   */
  template<class B>
  class ScopedPtrHolder
    : public boost::scoped_ptr<B>
    {
      typedef boost::scoped_ptr<B> _Parent;
      
      
    public:
      ScopedPtrHolder ()
        : _Parent(0)
        { }
      
      template<class SU>
      explicit ScopedPtrHolder (SU * p)  // never throws
        : _Parent(p)
        { }

      template<class SU>
      explicit ScopedPtrHolder (std::auto_ptr<SU> p)  // never throws
        : _Parent(p.release())
        { }

      ScopedPtrHolder (ScopedPtrHolder const& ref)
        : _Parent(must_be_null (ref))
        { }
      
      ScopedPtrHolder&
      operator= (ScopedPtrHolder const& ref)
        {
          must_be_null (*this);
          must_be_null (ref);
          return *this;
        }
      
      
    private:
      static B* must_be_null (_Parent const& ptr)
      {
        if (ptr)
          throw lumiera::error::Logic("ScopedPtrHolder protocol violation: "
                                      "attempt to copy from non-null.");
        return 0;
      }
    };
    
    
    
  
  template<class TY>
  class ScopedHolder
    {
      char content_[sizeof(TY)];
      char created_;
      
      typedef ScopedHolder<TY> _ThisType;

      
    public:
      ScopedHolder()
        : created_(0)
        { }
      
      TY& 
      create ()
        {
          ASSERT (!created_);
          TY * obj = new(content_) TY();
          ++created_;
          return *obj;
        }
      
      ~ScopedHolder()
        {
          if (created_)
            content_->~TY();
        }

        
      ScopedHolder (ScopedHolder const& ref)
        : created_(must_be_empty (ref))
        { }
      
      ScopedHolder&
      operator= (ScopedHolder const& ref)
        {
          must_be_empty (*this);
          must_be_empty (ref);
          return *this;
        }
        
      
      TY&
      operator* ()  const  // never throws
        {
          ASSERT (created_);
          return (TY&) content_;
        }

      TY* 
      operator-> ()  const // never throws
        {
          ASSERT (created_);
          return (TY*) &content_;
        }

      TY* get()          const { return &content_; }
      bool operator! ()  const { return !created_; }
      
      
      typedef char _ThisType::*unspecified_bool_type;

      /** implicit conversion to "bool" */
      operator unspecified_bool_type()  const // never throws
        {
          return created_?  &_ThisType::created_ : 0;
        }

      
    private:
      static char must_be_empty (_ThisType const& ref)
      {
        if (ref)
          throw lumiera::error::Logic("ScopedHolder protocol violation: "
                                      "attempt after having invoked create().");
        return 0;
      }
    };
  
  
  
  
} // namespace lib
#endif
