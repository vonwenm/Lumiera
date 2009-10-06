/*
  TYPED-COUNTER.hpp  -  maintain a set of type based contexts 
 
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


/** @file typed-counter.hpp
 ** Creating sets of type-based contexts.
 ** The idea is to get a "slot" for any given type, so we can build
 ** tables or families of implementations based on these types. Currently,
 ** the slot allocation is based on static variables and thus is global.
 ** This leads to a waste of slots, as the various clients of this service
 ** typically will utilise different sets of types.
 ** @todo WIP WIP... this is the first, preliminary version of a facility,
 **       which is expected to get quite important for custom allocation management.
 ** 
 ** @see typed-counter-test.cpp
 ** @see TypedAllocationManager
 ** @see AllocationCluster (custom allocation scheme using a similar idea inline)
 **
 */



#ifndef LIB_TYPED_COUNTER_H
#define LIB_TYPED_COUNTER_H

//#include "pre.hpp"
#include "lib/error.hpp"
#include "lib/sync-classlock.hpp"

#include <vector>



namespace lib {
  
  using std::vector;
  
  
  /** 
   * Providing type-IDs for a specific context.
   * This facility allows to access a numeric ID for each
   * provided distinct type. Type-IDs may be used e.g. for
   * dispatcher tables or for custom allocators. 
   * The type-IDs generated here are not completely global though.
   * Rather, they are tied to a specific type context, e.g. a class
   * implementing a custom allocator.
   */
  template<class CX>
  class TypedContext
    {
      static size_t lastGeneratedTypeID;
      
    public:
      static size_t
      newTypeID (size_t& typeID)
        {
          ClassLock<TypedContext> synchronised();
          if (!typeID)
            typeID = ++lastGeneratedTypeID;
          return typeID;
        }
      
      /** type-ID */
      template<typename TY>
      class ID
        {
          static size_t typeID;
          
        public:
          static size_t
          get()
            {
              if (typeID)
                return typeID;
              else
                return newTypeID(typeID);
            }
        };
    };
  
  /** storage for the type-ID generation mechanism */
  template<class CX>
  size_t TypedContext<CX>::lastGeneratedTypeID (0);
  
  /** table holding all the generated type-IDs */
  template<class CX>
  template<typename TY>
  size_t TypedContext<CX>::ID<TY>::typeID (0);
  
  
  
  
  /** 
   * Helper providing a set of counters, each tied to a specific type.
   */
  class TypedCounter
    : public Sync<>
    {
      mutable vector<long> counters_;
      
      template<typename TY>
      size_t
      slot()  const
        {
          size_t typeID = TypedContext<TypedCounter>::ID<TY>::get();
          if (size() < typeID)
            counters_.resize (typeID);
          
          ENSURE (counters_.capacity() >= typeID);
          return (typeID - 1);
        }
      
      
    public:
      TypedCounter()
        {
          counters_.reserve(5);  // pre-allocated 5 slots
        }
      
      
      template<class X>
      long 
      get()  const
        {
          Lock sync(this);
          return counters_[slot<X>()];
        }
      
      template<class X>
      long 
      inc()
        {
          Lock sync(this);
          return ++counters_[slot<X>()];
        }
      
      template<class X>
      long 
      dec()
        {
          Lock sync(this);
          return --counters_[slot<X>()];
        }
      
      
      /* == diagnostics == */
      
      size_t size() const { return counters_.size(); }
      bool empty()  const { return counters_.empty();}
    };
  
  
  
} // namespace lib
#endif