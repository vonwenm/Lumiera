/*
  STATE.hpp  -  Key Interface representing a render process and encapsulating state
 
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


#ifndef PROC_INTERFACE_STATE_H
#define PROC_INTERFACE_STATE_H


#include "pre.hpp"

#include "proc/lumiera.hpp"
#include "common/frameid.hpp"
#include "proc/engine/buffhandle.hpp"

#include <cstddef>


namespace engine { 
  
  using lumiera::FrameID;

  class StateAdapter;
  
  
  class State
    {
    protected:
      virtual ~State() {};
      
      /** resolves to the State object currently "in charge".
       *  Intended as a performance shortcut to avoid calling
       *  up through a chain of virtual functions when deep down
       *  in chained ProcNode::pull() calls. This allows derived
       *  classes to proxy the state inteface.
       */ 
      virtual State& getCurrentImplementation () =0;
      
      friend class engine::StateAdapter;
      
    public:
      /** allocate a new writable buffer with type and size according to
       *  the BufferDescriptor. The actual provider of this buffer depends
       *  on the State implementation; it could be a temporary, located in
       *  the cache, used for feeding calculated frames over a network, etc.
       *  @return a BuffHandle encapsulating the information necessary to get
       *          at the actual buffer adress and for releasing the buffer.
       */
      virtual BuffHandle allocateBuffer (BufferDescriptor const&)  =0;
      
      /** resign control of the buffer denoted by the handle */
      virtual void releaseBuffer (BuffHandle&)  =0;
      
      /** declare the data contained in the Buffer to be ready.
       *  The caller is required to restrain itself from modifying the data
       *  afterwards, as this buffer now can be used (readonly) by other
       *  calculation processes in parallel.
       */
      virtual void is_calculated (BuffHandle const&)  =0;
      
      /** try to fetch an existing buffer containing the denoted frame from
       *  a cache or similar backing system (e.g. peer over the network).
       *  @return either a handle to a readonly buffer, or a null handle
       *  @note the client is resposible for not modifying the provided data
       */
      virtual BuffHandle fetch (FrameID const&)  =0;
      
    };
  
} // namespace engine



namespace proc_interface {

  using engine::State;
    
    
} // namespace proc_interface
#endif
