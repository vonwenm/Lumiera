/*
  SessionImpl  -  holds the complete session data to be edited by the user
 
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


#include "proc/mobject/session/session-impl.hpp"
#include "proc/mobject/session/mobjectfactory.hpp"
#include "proc/mobject/placement.hpp"
#include "proc/mobject/mobject.hpp"
#include "lib/error.hpp"

namespace mobject {
namespace session {
  
  /////////////////////////////////////////TODO temporary hack: use Meyer's singleton
  namespace {
    DefsManager&
    getDummyDefaultsManager()
    {
      static scoped_ptr<DefsManager> dummyInstance(0);
      if (!dummyInstance) dummyInstance.reset (new DefsManager);
      
      return *dummyInstance;
    }
  }
  /////////////////////////////////////////TODO temporary hack
  
  
  /** create a new empty session with default values.
   *  @note any exception arising while creating this
   *        default session will inevitably halt the
   *        system (and this is desirable)
   */
  SessionImpl::SessionImpl ()
    : Session( getDummyDefaultsManager() )             ///////TODO temporary hack
    , pIdx_( MObject::create (getDummyDefaultsManager())) ////TODO temporary hack
    , fixture(new Fixture)
    {
      INFO (session, "new Session created.");
    }
  
  
  /** @internal used by SessionManager#clear
   *            discard all Session content,
   *            without touching global configuration.
   */
  void 
  SessionImpl::clear ()
  {
    try
      {
                          //////////////////////////////////////////////////////////////////////////TICKET #496
      }
    catch (...)
      {
        throw lumiera::error::Fatal ("unexpected exception while clearing the session"); ///////////TODO still required??
      }
  }
  
  
  bool 
  SessionImpl::isValid ()
  {
    UNIMPLEMENTED ("session self test");
    return false; // TODO
  }
  
  
  void 
  SessionImpl::add (PMO& placement)
  {
    UNIMPLEMENTED ("add Placement to the current Session");
  }
  
  
  bool 
  SessionImpl::remove (PMO& placement)
  {
    UNIMPLEMENTED ("search and remove a given Placement from current Session");
    return false; // TODO
  }
  
  
  PFix&
  SessionImpl::getFixture ()
  {
    return fixture;
  }
  
  
  void
  SessionImpl::rebuildFixture ()
  { 
    UNIMPLEMENTED ("rebuild Fixture");
  }
  
  
  
}} // namespace mobject::session
