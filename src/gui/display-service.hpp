/*
  DISPLAY-SERVICE.hpp  -  service providing access to a display for outputting frames

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

*/

/** @file display-service.hpp
 ** A public service provided by the GUI, implementing the lumiera::Display facade interface.
 ** It serves two purposes: 
 ** - It maintains a collection of DisplayerSlot objects, which are the actual connection points
 **   and allow to receive frames and dispatch them to the GTK main event loop thread.
 **   Conceptually, creating such a slot means providing a possible display for output.
 ** - It provides the actual implementation of the Display facade interface, i.e. the function
 **   which is to invoked periodically by the playback processes to dispose a new frame into
 **   the display.
 ** 
 ** This service is the implementation of a layer separation facade interface. This header defines
 ** the interface used to \em provide this service, not to access it. Clients get a specific
 ** LumieraDisplaySlot passed as parameter when initiating a playback process from the GUI. Using
 ** this LumieraDisplaySlot handle, clients should then use  lumiera::DummyPlayer#facade to access
 ** an implementation instance of this service in order to push actual frames up.
 **
 ** @see lumiera::Display
 ** @see lumiera::DummyPlayer
 ** @see gui::PlaybackController usage example 
 */


#ifndef GUI_DISPLAY_SERVICE_H
#define GUI_DISPLAY_SERVICE_H


#include "include/display-facade.h"
#include "common/instancehandle.hpp"
#include "lib/singleton-ref.hpp"
#include "lib/scoped-ptrvect.hpp"
#include "include/logging.h"

#include <glibmm.h>
#include <sigc++/sigc++.h>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>


namespace gui {
  
  using std::string;
  using std::vector;
  using lumiera::Display;
  using Glib::Dispatcher;
  
  
  typedef sigc::slot<void, void*> FrameDestination;
  typedef sigc::signal<void, void*> FrameSignal;
  
  
  
  /****************************************************************//**
   * Actual implementation of a single displayer slot. Internally,
   * it is connected via the Glib::Dispatcher for outputting frames 
   * to a viewer widget, which executes within the GTK event thread.
   * @note must be created from the GTK event thread.
   */
  class DisplayerSlot
    : public lumiera_displaySlot,
      boost::noncopyable
    {
      Dispatcher dispatcher_;
      FrameSignal hasFrame_;
      
      LumieraDisplayFrame currBuffer_;
      
      
    public:
      DisplayerSlot (FrameDestination const&) ;
     ~DisplayerSlot () ;
      
      /* Implementation-level API to be used by DisplayService */
      
      /** receive a frame to be displayed */
      inline void put (LumieraDisplayFrame);
      
      
    private:
      /** internal: activated via Dispatcher
       *  and running in GTK main thread */ 
      void displayCurrentFrame();
      
    };
  
  typedef lib::ScopedPtrVect<DisplayerSlot> DisplayerTab;
  
  
  
  /**************************************************//**
   * Actual implementation of the DisplayService.
   * Creating an instance of this class automatically
   * registers the interface lumieraorg_Display with
   * the Lumiera Interface/Plugin system and creates
   * a forwarding proxy within the application core to
   * route calls through this interface.
   * \par
   * In addition to the Display interface, this class 
   * implements an additional service for the GUI, 
   * allowing actually to set up display slots, which
   * then can be handed out to client code in the
   * course of the play process for outputting frames.
   */
  class DisplayService
    : boost::noncopyable
    {
      
      string error_;
      DisplayerTab slots_;
      
      
      /* === Interface Lifecycle === */
      
      typedef lumiera::InstanceHandle< LUMIERA_INTERFACE_INAME(lumieraorg_Display, 0)
                                     , lumiera::Display
                                     > ServiceInstanceHandle;
      
      lib::SingletonRef<DisplayService> implInstance_;
      ServiceInstanceHandle serviceInstance_;
      
      
    public:
      DisplayService();
     ~DisplayService() {
                      INFO (proc_dbg, "Display service dying...");

     }
      
      
      /** open a new display, sending frames to the given output destination
       *  @return handle for this slot, can be used to start a play process.
       *          NULL handle in case of any error. */
      static LumieraDisplaySlot setUp (FrameDestination const&);
      
      
      /** prepare and the given slot for output
       *  @param doAllocate allocate when true, else release it
       *  @throw lumiera::error::Logic when already in use */ 
      void allocate (LumieraDisplaySlot, bool doAllocate);
      
      
      /** resolve the given display slot handle to yield a ref
       *  to an actual implementation object. In order to be resolvable,
       *  the DisplayerSlot needs to be locked (=allocated) for output use. */
      DisplayerSlot& resolve (LumieraDisplaySlot);
      
    };
  
  
  
  
  void
  DisplayerSlot::put(LumieraDisplayFrame newFrame)
  {
    if (newFrame != currBuffer_)
      {
        currBuffer_ = newFrame;
        dispatcher_.emit();
      }
    else
      {
        TRACE (render, "frame dropped?");
      }
  }
  
  
  
} // namespace gui
#endif
