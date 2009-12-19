/*
  TRAIT.hpp  -  type handling and type detection helpers
 
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


#ifndef LUMIERA_META_TRAIT_H
#define LUMIERA_META_TRAIT_H


#include "lib/meta/util.hpp"

#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/ref.hpp>
#include <string>


namespace lumiera {
namespace typelist {
  
  
  
  /** Trait template for detecting if a type can be converted to string.
   *  For example, this allows to write specialisations with the help of
   *  boost::enable_if
   */
  template <typename TY>
  struct can_ToString
    {
      enum { value = boost::is_convertible<TY, std::string>::value
           };
    };
  
  
  /** Trait template for guarding \c lexical_cast<..> expressions.
   *  Such an expression won't even compile for some types, because of
   *  missing or ambiguous output operator(s).
   *  Ideally, there would be some automatic detection (relying on the
   *  existence of an operator<< for the given type. But I couldn't make
   *  this work, so I fell back on just declaring types which are known
   *  to work with lexical_cast to string
   *  @note this compile-time trait can't predict if such an conversion
   *        to string will be successful at runtime; indeed it may throw,
   *        so you should additionally guard the invocation with try-catch!
   */
  template<typename X>
  struct can_lexical2string
    {
      enum { value = boost::is_arithmetic<X>::value
           };
    };
  
  
  /** Extension to boost::reference_wrapper: 
   *  Allows additionally to re-bind to another reference,
   *  almost like a pointer. For example this allows to cache
   *  results returned from an API call by reference.
   *  @warning potentially dangerous 
   */
  template<typename TY>
  class AssignableRefWrapper
    : public boost::reference_wrapper<TY>
    {
      typedef boost::reference_wrapper<TY> RefWrapper;
    public:
      
      explicit AssignableRefWrapper(TY& ref)
        : RefWrapper(ref)
        { }
      
      AssignableRefWrapper&
      operator= (RefWrapper const& o)
        {
          RefWrapper::operator= (o);
          return *this;
        }
      
      AssignableRefWrapper&
      operator= (TY& newRef)
        {
          (*this) = RefWrapper(newRef);
          return *this;
        }
    };
  
  
  /** Type definition helper for pointer and reference types.
   *  Allows to create a member field and to get the basic type
   *  irrespective if the given type is plain, pointer or reference
   */
  template<typename TY>
  struct RefTraits
    {
      typedef TY* pointer;
      typedef TY& reference;
      typedef TY  value_type;
      typedef value_type member_type;
    };
  
  template<typename TY>
  struct RefTraits<TY *>
    {
      typedef TY* pointer;
      typedef TY& reference;
      typedef TY  value_type;
      typedef pointer member_type;
    };
  
  template<typename TY>
  struct RefTraits<const TY *>
    {
      typedef TY value_type;
      typedef const TY* pointer;
      typedef const TY& reference;
      typedef pointer member_type;
    };
  
  template<typename TY>
  struct RefTraits<TY &>
    {
      typedef TY* pointer;
      typedef TY& reference;
      typedef TY  value_type;
      typedef AssignableRefWrapper<TY> member_type;
    };
  
  
}} // namespace lumiera::typelist
#endif
