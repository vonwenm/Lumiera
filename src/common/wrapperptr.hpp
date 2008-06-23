/*
  WRAPPERPTR.hpp  -  variant record able to hold a pointer to some smart-ptr/wrapper types, providing conversions
 
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



#ifndef LUMIERA_WRAPPERPTR_H
#define LUMIERA_WRAPPERPTR_H


#include "common/variant.hpp"
#include "common/accesscasted.hpp"

#include "common/typelist.hpp"
#include "proc/mobject/placement.hpp"
#include "common/p.hpp"


namespace asset   { class Asset; }
namespace mobject { class MObject; }

namespace lumiera {
  
  
  typedef typelist::Types < mobject::Placement<mobject::MObject>*
                          , P<asset::Asset>*
                          > ::List
                          WrapperTypes;
  
  /** 
   * helper to treat various sorts of smart-ptrs uniformly.
   * Implemented as a variant-type value object, it is preconfigured
   * with the possible hierarchy-base classes used within this application.
   * Thus, when passing in an arbitrary smart-ptr, the best fitting smart-ptr
   * type pointing to the corresponding base class is selected for internal storage.
   * Later on, stored values can be retrieved either utilitzing static or dynamic casts;
   * error reporting is similar to the bahaviour of dynamic_cast<T>: when retrieving
   * a pointer, in case of mismatch NULL is returned.
   */
  typedef lumiera::Variant<WrapperTypes, util::AccessCasted> WrapperPtr;
  
  
  
} // namespace lumiera 
#endif