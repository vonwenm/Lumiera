/*
  UTIL-FOREACH.hpp  -  helpers for doing something for each element

  Copyright (C)         Lumiera.org
    2009,               Hermann Vosseler <Ichthyostega@web.de>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/


/** @file util-foreach.hpp
 ** Perform operations "for each element" of a collection.
 ** This header defines various flavours of these active iteration functions,
 ** which all take a functor and invoke it in some way over the collection's elements.
 ** - basic constructs
 **   - for_each
 **   - and_all (all quantisation)
 **   - has_any (existence quantisation)
 ** - kinds of collection
 **   - STL collections and anything which has an iterator, \c begin() and \c end()
 **   - "Lumiera Forward Iterator" instance to yield all the elements of the collection
 ** - support for on-the-fly binding up to 4 arguments of the functor
 ** 
 ** @warning in the standard case (STL container) the collection to operate on is
 **   taken by \c const& -- but the <b>const is stripped</b> silently.
 **   
 ** Thus, within the iteration, the function passed in can \em modify the original collection.
 ** If you pass in a ref to a temporary, the compiler won't complain. Moreover, several kinds
 ** of wrappers are also <b>stripped silently</b>, including reference_wrapper, shared_ptr and
 ** lumiera::P. The rationale for this non-standard behaviour is to allow convenient writing
 ** of in-line iteration, where even the collection to iterate is yielded by function call.
 ** 
 ** @see util-foreach-test.cpp
 ** 
 */


#ifndef UTIL_FOREACH_H
#define UTIL_FOREACH_H

#include "lib/util.hpp"
#include "lib/meta/trait.hpp"

#include <boost/utility/enable_if.hpp>
#include <functional>
#include <algorithm>



namespace util {
  
  using boost::enable_if;
  using boost::disable_if;
  
  using lib::meta::can_STL_ForEach;
  using lib::meta::can_IterForEach;
  
  
  
  /** All quantification: check if all elements of a collection
   *  satisfy the given predicate. Actually a short-circuit
   *  evaluation is performed.
   */
  template <typename IT, typename FUN>
  inline bool
  and_all (IT i, IT end, FUN predicate)
  {
    for ( ; i!=end; ++i )
      if (!predicate(*i))
        return false;
    
    return true;
  }
  
  
  /** Existential quantification: check if any element
   *  of a collection satisfies the given predicate.
   *  Actually, a short-circuit evaluation is performed.
   */
  template <typename IT, typename FUN>
  inline bool
  has_any (IT i, IT end, FUN predicate)
  {
    for ( ; i!=end; ++i )
      if (predicate(*i))
        return true;
    
    return false;
  }
  
  
  /* === specialisations for STL containers and Lumiera Forward Iterators === */
  
  /** operating on all elements of a STL container.
   *  @note the container is taken by \c const& and
   *        the \c const is \em stripped before iteration.
   *  @note this case is the default and kicks in
   *        \em unless we detect a Lumiera iterator.
   *        The handling is different for \c and_all
   */
  template <typename Container
           ,typename FUN
           >
  inline   typename disable_if< can_IterForEach<Container>,
  FUN      >::type
  for_each (Container const& coll, FUN doIt)
  {
    using lib::meta::unwrap;
    
    return std::for_each (unwrap(coll).begin()
                         ,unwrap(coll).end()
                         ,             doIt);
  }
  
  
  /** operating on a Lumiera Forward Iterator until exhaustion. */
  template <typename IT
           ,typename FUN
           >
  inline   typename enable_if< can_IterForEach<IT>,
  FUN      >::type
  for_each (IT const& ii, FUN doIt)
  {
    return std::for_each (ii, IT(), doIt);
  }
  
  
  
  
  template <typename Container
           ,typename FUN
           >
  inline   typename enable_if< can_STL_ForEach<Container>,
  bool     >::type
  and_all (Container const& coll, FUN predicate)
  {
    using lib::meta::unwrap;
    
    return and_all (unwrap(coll).begin()
                   ,unwrap(coll).end()
                   ,             predicate);
  }
  
  
  template <typename IT
           ,typename FUN
           >
  inline   typename enable_if< can_IterForEach<IT>,
  bool     >::type
  and_all (IT const& ii, FUN predicate)
  {
    return and_all (ii, IT(), predicate);
  }
  
  
  
  template <typename Container
           ,typename FUN
           >
  inline   typename enable_if< can_STL_ForEach<Container>,
  bool     >::type
  has_any (Container const& coll, FUN predicate)
  {
    using lib::meta::unwrap;
    
    return has_any (unwrap(coll).begin()
                   ,unwrap(coll).end()
                   ,             predicate);
  }
  
  
  template <typename IT
           ,typename FUN
           >
  inline   typename enable_if< can_IterForEach<IT>,
  bool     >::type
  has_any (IT const& ii, FUN predicate)
  {
    return has_any (ii, IT(), predicate);
  }
  
  
  
  
  /* === allow creating argument binders on-the-fly === */
  
  
  template < typename CON, typename FUN
           , typename P1
           >
  inline void                                                                            //________________________________
  for_each (CON const& elements, FUN function, P1 bind1)                                ///< Accept binding for 1 Argument 
  {
    for_each (elements, std::bind (function, bind1));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           >
  inline void                                                                            //________________________________
  for_each (CON const& elements, FUN function, P1 bind1, P2 bind2)                      ///< Accept binding for 2 Arguments
  {
    for_each (elements, std::bind (function, bind1, bind2));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           , typename P3
           >
  inline void                                                                            //________________________________
  for_each (CON const& elements, FUN function, P1 bind1, P2 bind2, P3 bind3)            ///< Accept binding for 3 Arguments
  {
    for_each (elements, std::bind (function, bind1, bind2, bind3));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           , typename P3
           , typename P4
           >
  inline void                                                                            //________________________________
  for_each (CON const& elements, FUN function, P1 bind1, P2 bind2, P3 bind3, P4 bind4)  ///< Accept binding for 4 Arguments
  {
    for_each (elements, std::bind (function, bind1, bind2, bind3, bind4));
  }
  
  
  
  
  
  template < typename CON, typename FUN
           , typename P1
           >
  inline bool                                                                            //________________________________
  and_all (CON const& elements, FUN function, P1 bind1)                                 ///< Accept binding for 1 Argument 
  {
    return and_all (elements, std::bind<bool> (function, bind1));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           >
  inline bool                                                                            //________________________________
  and_all (CON const& elements, FUN function, P1 bind1, P2 bind2)                       ///< Accept binding for 2 Arguments
  {
    return and_all (elements, std::bind<bool> (function, bind1, bind2));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           , typename P3
           >
  inline bool                                                                            //________________________________
  and_all (CON const& elements, FUN function, P1 bind1, P2 bind2, P3 bind3)             ///< Accept binding for 3 Arguments
  {
    return and_all (elements, std::bind<bool> (function, bind1, bind2, bind3));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           , typename P3
           , typename P4
           >
  inline bool                                                                            //________________________________
  and_all (CON const& elements, FUN function, P1 bind1, P2 bind2, P3 bind3, P4 bind4)   ///< Accept binding for 4 Arguments
  {
    return and_all (elements, std::bind<bool> (function, bind1, bind2, bind3, bind4));
  }
  
  
  
  
  
  template < typename CON, typename FUN
           , typename P1
           >
  inline bool                                                                            //________________________________
  has_any (CON const& elements, FUN function, P1 bind1)                                 ///< Accept binding for 1 Argument 
  {
    return has_any (elements, std::bind<bool> (function, bind1));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           >
  inline bool                                                                            //________________________________
  has_any (CON const& elements, FUN function, P1 bind1, P2 bind2)                       ///< Accept binding for 2 Arguments
  {
    return has_any (elements, std::bind<bool> (function, bind1, bind2));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           , typename P3
           >
  inline bool                                                                            //________________________________
  has_any (CON const& elements, FUN function, P1 bind1, P2 bind2, P3 bind3)             ///< Accept binding for 3 Arguments
  {
    return has_any (elements, std::bind<bool> (function, bind1, bind2, bind3));
  }
  
  
  template < typename CON, typename FUN
           , typename P1
           , typename P2
           , typename P3
           , typename P4
           >
  inline bool                                                                            //________________________________
  has_any (CON const& elements, FUN function, P1 bind1, P2 bind2, P3 bind3, P4 bind4)   ///< Accept binding for 4 Arguments
  {
    return has_any (elements, std::bind<bool> (function, bind1, bind2, bind3, bind4));
  }
  
  
  
  
} // namespace util
#endif /*UTIL_FOREACH_H*/
