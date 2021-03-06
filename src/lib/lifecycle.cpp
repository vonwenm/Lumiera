/*
  Lifecycle  -  registering and triggering lifecycle callbacks 

  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>

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

* *****************************************************/


#include "lib/error.hpp"
#include "include/lifecycle.h"
#include "lib/lifecycleregistry.hpp"
#include "lib/util.hpp"

using util::cStr;



namespace lumiera {
  
  
  // ==== implementation LifecycleHook class =======
  
  typedef LifecycleRegistry::Hook Callback;
  
  
  LifecycleHook::LifecycleHook (Symbol eventLabel, Callback callbackFun)
  {
    add (eventLabel,callbackFun);
  }
  
  void
  LifecycleHook::add (Symbol eventLabel, Callback callbackFun)
  {
    bool isNew = LifecycleRegistry::instance().enrol (eventLabel,callbackFun);
    
    if (isNew && !strcmp(ON_BASIC_INIT, eventLabel))
      callbackFun();  // when this code executes,
                     //  then per definition we are already post "basic init"
                    //   (which happens in the AppState ctor); thus fire it immediately
  }
  
  
  void
  LifecycleHook::trigger (Symbol eventLabel)
  {
    LifecycleRegistry::instance().execute (eventLabel);
  }
  
  
  
  const char * ON_BASIC_INIT      ("ON_BASIC_INIT");
  const char * ON_GLOBAL_INIT     ("ON_GLOBAL_INIT");
  const char * ON_GLOBAL_SHUTDOWN ("ON_GLOBAL_SHUTDOWN");
  
  const char * ON_EMERGENCY       ("ON_EMERGENCY");
  
  
} // namespace lumiera


extern "C" { /* ==== implementation C interface for lifecycle hooks ======= */
  
  
  const char * lumiera_ON_BASIC_INIT       = lumiera::ON_BASIC_INIT;
  const char * lumiera_ON_GLOBAL_INIT      = lumiera::ON_GLOBAL_INIT;
  const char * lumiera_ON_GLOBAL_SHUTDOWN  = lumiera::ON_GLOBAL_SHUTDOWN;
  
  const char * lumiera_ON_EMERGENCY        = lumiera::ON_EMERGENCY;
  
  
  
  void 
  lumiera_LifecycleHook_add (const char* eventLabel, void callbackFun(void))
  {
    lumiera::LifecycleHook (eventLabel, callbackFun);
  }
  
  
  void
  lumiera_Lifecycle_trigger (const char* eventLabel)
  {
    lumiera::LifecycleRegistry::instance().execute (eventLabel);
  }
  
}
