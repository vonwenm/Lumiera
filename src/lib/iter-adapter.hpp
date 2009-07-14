/*
  ITER-ADAPTOR.hpp  -  helpers for building simple forward iterators 
 
  Copyright (C)         Lumiera.org
    2009,               Hermann Vosseler <Ichthyostega@web.de>
 
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

/** @file iter-adapter.hpp
 ** Helper template(s) for creating <b>lumiera forward iterators</b>.
 ** This denotes a concept similar to STL's  "forward iterator", with
 ** the addition of an bool check to detect iteration end. The latter
 ** is inspired by the \c hasNext() function found in many current
 ** languages supporting iterators. In a similar vein (inspired from
 ** functional programming), we deliberately don't support the various
 ** extended iterator concepts from STL and boost (random access iterators,
 ** output iterators and the like). According to this concept, <i>an iterator
 ** is a promise for pulling values</i> -- and nothing beyond that.
 ** 
 ** @todo WIP WIP WIP
 ** @todo see Ticket #182
 ** 
 ** @see scoped-ptrvect.hpp
 */


#ifndef LIB_ITER_ADAPTOR_H
#define LIB_ITER_ADAPTOR_H


#include "lib/error.hpp"
#include "lib/bool-checkable.hpp"

#include <boost/type_traits/remove_pointer.hpp>



namespace lib {
  
  using boost::remove_pointer;
  
  
  
  /**
   * Adapter for building an implementation of the lumiera forward iterator concept.
   * The "current position" is represented as an opaque element (usually an nested iterator),
   * with callbacks to the controlling container instance for managing this position. 
   * Basically such an IterAdapter behaves like the similar concept from STL, but
   * - it is not just a disguised pointer (meaning, it's more expensive)
   * - it checks validity on every operation and may throw
   * - it has a distinct back-link to the source container
   * - the source container needs to implement iterStart() and iterInc()
   * - we need friendship to access the callbacks on the container 
   * - the end-of-iteration can be detected by bool check
   * 
   * @see scoped-ptrvect.hpp usage example
   * @see iter-adaptor-test.cpp
   */
  template<class POS, class CON>
  class IterAdapter
    : public lib::BoolCheckable<IterAdapter<POS,CON> >
    {
      const CON* source_;
      mutable POS pos_;
      
    public:
      typedef typename POS::pointer pointer;
      typedef typename POS::reference reference;
      typedef typename POS::value_type value_type;
      
      IterAdapter (const CON* src, const POS& startpos)
        : source_(src)
        , pos_(startpos)
        { 
          CON::iterValid(source_,pos_);
        }
      
      IterAdapter ()
        : source_(0)
        , pos_(0)
        { }
      
      
      /* === lumiera forward iterator concept === */
      
      reference
      operator*() const
        {
          _maybe_throw();
          return *pos_;
        }
      
      pointer
      operator->() const
        {
          _maybe_throw();
          return pos_;
        }
      
      IterAdapter&
      operator++()
        {
          _maybe_throw();
          CON::iterNext (source_,pos_);
          return *this;
        }
      
      IterAdapter
      operator++(int)
        {
          _maybe_throw();
          IterAdapter oldPos(*this);
          CON::iterNext (source_,pos_);
          return oldPos;
        }
      
      bool
      isValid ()  const
        {
          return (source_ && CON::iterValid(source_,pos_));
        }
      
      bool
      empty ()    const
        {
          return !isValid();
        }
      
      
    private:
      
      void
      _maybe_throw()  const
        {
          if (!isValid())
            throw lumiera::error::Invalid ("Can't iterate further",
                  lumiera::error::LUMIERA_ERROR_ITER_EXHAUST);
        }
      
      /// comparison is allowed to access impl iterator
      template<class P1, class P2, class CX>
      friend bool operator== (IterAdapter<P1,CX> const&, IterAdapter<P2,CX> const&);
    };
  
  
  /// Supporting equality comparisons...
  template<class P1, class P2, class CON>
  bool operator== (IterAdapter<P1,CON> const& il, IterAdapter<P2,CON> const& ir)  { return il.pos_ == ir.pos_; }
  
  template<class P1, class P2, class CON>
  bool operator!= (IterAdapter<P1,CON> const& il, IterAdapter<P2,CON> const& ir)  { return !(il == ir); }
  
  
  
  
  
  
  /** wrapper for an existing Iterator type,
   *  automatically dereferencing the output of the former.
   *  For this to work, the "source" iterator is expected
   *  to be declared on \em pointers rather than on values.
   *  @note bool checkable if and only if source is...
   */
  template<class IT>
  class PtrDerefIter
    : public lib::BoolCheckable<PtrDerefIter<IT> >
    {
      IT i_;
      
    public:
      typedef typename IT::value_type pointer;
      typedef typename remove_pointer<pointer>::type value_type;
      typedef value_type& reference; 
      
      
      PtrDerefIter (IT srcIter)
        : i_(srcIter)
        { }
      
      
      /* === lumiera forward iterator concept === */
      
      reference
      operator*() const
        {
          return *(*i_);
        }
      
      pointer
      operator->() const
        {
          return *i_;
        }
      
      PtrDerefIter&
      operator++()
        {
          ++i_;
          return *this;
        }
      
      PtrDerefIter
      operator++(int)
        {
          return PtrDerefIter (i_++);
        }
      
      bool
      isValid ()  const
        {
          return bool(i_);
        }
      
      bool
      empty ()    const
        {
          return !isValid();
        }
      
      
      /// comparison operator is allowed to access the underlying impl iterator
      template<class I1, class I2>
      friend bool operator== (PtrDerefIter<I1> const&, PtrDerefIter<I2> const&);
    };
  
  
  /// Supporting equality comparisons...
  template<class I1, class I2>
  bool operator== (PtrDerefIter<I1> const& il, PtrDerefIter<I2> const& ir)  { return il.i_ == ir.i_; }
    
  template<class I1, class I2>
  bool operator!= (PtrDerefIter<I1> const& il, PtrDerefIter<I2> const& ir)  { return !(il == ir); }
  
  
} // namespace lib
#endif
