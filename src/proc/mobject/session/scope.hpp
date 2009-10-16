/*
  SCOPE.hpp  -  nested search scope for properties of placement

 
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


#ifndef MOBJECT_SESSION_SCOPE_H
#define MOBJECT_SESSION_SCOPE_H

//#include "proc/mobject/mobject.hpp"
#include "proc/mobject/placement.hpp"
#include "proc/mobject/placement-ref.hpp"
#include "lib/singleton.hpp"

//#include <vector>
//#include <string>

//using std::vector;
//using std::string;

namespace mobject {
namespace session {
  
  
  class ScopeLocator;
  

  /**
   * TODO type comment
   */
  class Scope
    {
      RefPlacement anchor_;
      
    public:
      Scope (PlacementMO const& constitutingPlacement);
      
    };
  
  
  class ScopeLocator
    {
      
    public:
      ScopeLocator();
      
    };
///////////////////////////TODO currently just fleshing the API
  
  
}} // namespace mobject::session
#endif
