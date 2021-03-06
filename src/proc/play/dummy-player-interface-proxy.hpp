/*
  DummyPlayer(Proxy)  -  access point and service implementing a dummy test player

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

* *****************************************************/



/** @file dummy-player-interface-proxy.hpp
 ** This is an implementation fragment, intended to be included into common/interfaceproxy.cpp
 ** 
 ** The purpose is to define a proxy implementation of lumiera::DummyPlayer, in order to
 ** redirect any calls through the associated C Language Interface "lumieraorg_DummyPlayer"
 ** 
 ** @remarks the implementation of this interface proxy for the DummyPlayer highlighted
 **          some interesting design issues. The way we're defining our interfaces and
 **          bindings seems to have some shortcomings when attempting to express an
 **          interface (here DummyPlayer) in terms of another abstraction
 **          (here the DummyPlayer::Process), since the implementation of this
 **          abstraction has to be mapped and indirected via the interface-system
 **          as well. This forces us to duplicate all of these secondary interface
 **          functions several times, and incurs a further forwarding through the
 **          smart-Handle, since our interface system doesn't support suitable
 **          lifecycle support out of the box and instead places this burden
 **          onto the client code (or, as in this case here, the intermediary
 **          proxy used by the client code to access the interface).
 ** 
 ** @see dummy-player-facade.hpp
 ** @see dummy-player-service.hpp actual implementation within the Proc-Layer 
 */






    /* ==================== DummyPlayer ======================================= */
    
//#include "proc/play/dummy-player-service.hpp"
#include "include/dummy-player-facade.h"

namespace lumiera {
  
  /** storage for the DummyPlayer facade proxy factory... */
  lumiera::facade::Accessor<DummyPlayer> DummyPlayer::facade;
  
  // emit the vtable here into this translation unit within liblumieracommon.so ...
  DummyPlayer::~DummyPlayer()  { }
  
  
  
  
  
  namespace facade {
    
    typedef lumiera::InstanceHandle< LUMIERA_INTERFACE_INAME(lumieraorg_DummyPlayer, 0)
                                   , lumiera::DummyPlayer
                                   > IHandle_DummyPlayer;
    
    
    template<>
    class Proxy<IHandle_DummyPlayer>
      : public Holder<IHandle_DummyPlayer>
      {
        //----Proxy-Implementation-of-DummyPlayer--------
        typedef lumiera::DummyPlayer::Process Process;
        typedef proc::play::ProcessImpl ProcessImpl;
        
        /** @note as an optimisation we hand out a direct reference
         *  to the implementing process object. While this ref could
         *  still be passed as handle to the C Language interface, using
         *  it directly within the client (=GUI) bypasses the C interface
         *  and thus leaves us only with one level of indirection,
         *  irrespective if using the C or C++ interface.
         *  @note in hindsight this turned out as a very bad idea,
         *  since it complicated the definition of the facade proxy
         *  and created quite involved library dependency problems.
         */
        Process start(LumieraDisplaySlot viewerHandle)
          {
            ProcessImplementationLink* pP = static_cast<ProcessImplementationLink*> (_i_.startPlay (viewerHandle));
            
            if (!pP)
              throw lumiera::error::State("failed to start DummyPlayer", lumiera_error());
            
            return pP->createHandle();
          }
        
        
        
      public:
        Proxy (IHandle const& iha) : THolder(iha) {}
      };
    
    
    template  void openProxy<IHandle_DummyPlayer>  (IHandle_DummyPlayer const&);
    template  void closeProxy<IHandle_DummyPlayer> (void);
    
    
  } // namespace facade
  
  
   /* === Forwarding function(s) on the Process handle === */
  
  void
  DummyPlayer::Process::play (bool yes)
    {
      // access the implementation via smart-Handle
      impl().doPlay(yes);
    }
  
  // emit the VTable and typeinfo for this interface here (in liblumieracommon.so)
  DummyPlayer::ProcessImplementationLink::~ProcessImplementationLink() { };
  
  
  
} // namespace lumiera
