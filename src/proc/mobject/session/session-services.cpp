/*
  SessionServices  -  accessing Proc-Layer internal session implementation services
 
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
 
* *****************************************************/


#include "proc/mobject/session/session-service-fetch.hpp"
#include "proc/mobject/session/session-service-explore-scope.hpp"
#include "proc/mobject/session/session-service-mock-index.hpp"
#include "proc/mobject/session/session-service-defaults.hpp"

#include "proc/mobject/session/session-services.hpp"
#include "proc/mobject/session/session-impl.hpp"

namespace mobject {
namespace session {
  
  /** TODO */
  bool
  SessionServiceFetch::isRegisteredID (PlacementMO::ID const& placementID)
  {
    return SessionImplAPI::current->isRegisteredID (placementID);
  }
  
  
  PlacementMO&
  SessionServiceFetch::resolveID (PlacementMO::ID const& placementID)
  {
    return SessionImplAPI::current->resolveID (placementID);
  }
  
  
  
}} // namespace mobject::session
