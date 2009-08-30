/*
  RENDER-INVOCATION.hpp  -  initiate the rendering of a single frame
 
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

/** @file render-invocation.hpp
 ** Initiate a single calculation unit within the renderengine.
 ** Usually, this will cause the rendering of a single frame or sub-frame.
 **
 ** @see engine::ProcNode
 ** @see State
 ** @see node-basic-test.cpp
 **
 */

#ifndef ENGINE_RENDER_INVOCATION_H
#define ENGINE_RENDER_INVOCATION_H


#include "proc/state.hpp"
//#include "proc/engine/procnode.hpp"
//#include "proc/engine/buffhandle.hpp"
//#include "proc/engine/bufftable.hpp"




namespace engine {
  
  
  /**
   * TODO: type comment
   */
  class RenderInvocation
    : public State
    {
    protected:
//      State& parent_;
//      State& current_;
      
      RenderInvocation()
        { }
      
      virtual State& getCurrentImplementation () { return current_; }
      
      
      
    public: /* === proxying the State interface === */
      
//      virtual void releaseBuffer (BuffHandle& bh)       { current_.releaseBuffer (bh); }
      
//      virtual void is_calculated (BuffHandle const& bh) { current_.is_calculated (bh); }
      
//      virtual BuffHandle fetch (FrameID const& fID)     { return current_.fetch (fID); }
      
      // note: allocateBuffer()  is chosen specifically based on the actual node wiring
      
    };
  
  ///////////////////////////////////TODO: currently just fleshing out the API
  
  
  
  
  
} // namespace engine
#endif
