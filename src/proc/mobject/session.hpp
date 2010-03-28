/*
  SESSION.hpp  -  holds the complete session to be edited by the user
 
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


/** @file session.hpp
 ** Primary Interface to the current Session.
 ** The session interface can be used to discover session's contents.
 ** Mostly, these objects within the session are MObject subclasses, but they
 ** are attached into the session by a Placement. Usually, you'd want to use
 ** the discovered objects to invoke operations on them; in most cases,
 ** invoking any mutating operation should be wrapped into a Command.
 ** 
 ** The Interface Session is abstract and only accessible via the
 ** static field Session::current, which actually refers to a SessManager 
 ** singleton instance. The latter acts as smart ptr-to-Impl for accessing the
 ** current session, but at the same time exposes a lifecycle/management API.
 ** 
 ** @note if interested in the interplay of Session, SessManager and the
 **       internal service APIs (SessionServices), you should have a look
 **       at session-service-access-test.cpp, as this test creates a complete
 **       but simplified mock setup of the session and session manager, without
 **       any access and synchronisation and similar concerns, to read top down.
 **
 ** @see session-structure-test.cpp
 ** @see timeline-sequence-handling-test.cpp
 ** @see session-modify-parts-test.cpp
 */


#ifndef MOBJECT_SESSION_H
#define MOBJECT_SESSION_H

#include "proc/mobject/placement.hpp"
#include "proc/mobject/mobject-ref.hpp"
#include "proc/mobject/session/defsmanager.hpp"
#include "lib/ref-array.hpp"
#include "lib/singleton.hpp"
#include "lib/symbol.hpp"
#include "lib/p.hpp"

#include <boost/noncopyable.hpp>
#include <tr1/memory>



namespace asset {
  class Timeline;    typedef lumiera::P<Timeline> PTimeline;
  class Sequence;    typedef lumiera::P<Sequence> PSequence;
}

namespace mobject {
  
  namespace session { 
    class SessManager;
    class Fixture;
    typedef std::tr1::shared_ptr<Fixture> PFix;
  }
  
  typedef session::SessManager& PSess;  ///< acts as a "PImpl" smart ptr

  
  /**
   * The (current) Session holds all the user
   * visible content to be edited and manipulated
   * within the Lumiera Application. From a users
   * perspective, it is a collection of Media Objects
   * (--> MObject) placed (--> Placement) onto virtual
   * Tracks.
   * 
   * Opening a Session has effectively global consequences, 
   * because the Session defines the available Assets, and some 
   * kinds of Assets define default behaviour. Thus, access to the
   * Session is similar to a Singleton, through \c Session::current
   * Besides the SessionManager, several sub-interfaces are exposed
   * as embedded components: DefaultsManger, timelines and sequences.
   * 
   * @note Any client should be aware that the Session can be closed, 
   *       replaced and loaded. The only way to access the Session is
   *       via a "PImpl" smart pointer session::PSess (which indeed is
   *       a reference to the SessManager and is accessible as the static
   *       field Session::current). Clients shouldn't try to get a direct
   *       pointer or reference to the Session object.
   *  
   */
  class Session 
    : boost::noncopyable
    {
    protected:
      typedef session::DefsManager&            DefaultsAccess;
      typedef lib::RefArray<asset::PTimeline>& TimelineAccess;
      typedef lib::RefArray<asset::PSequence>& SequenceAccess;
      
      
      Session (DefaultsAccess
              ,TimelineAccess
              ,SequenceAccess)  throw();
      virtual ~Session ();
      
      
    public:
      static bool initFlag;                     ///////////////TICKET #518  yet another hack; actually need to care for session manager startup.
      
      static session::SessManager& current;
      
      DefaultsAccess defaults;                  ///< manages default configured objects
      TimelineAccess timelines;                 ///< collection of timelines (top level)
      SequenceAccess sequences;                 ///< collection of sequences
      
      virtual bool isValid ()                    = 0;
      virtual MObjectRef attach (PMO& placement) = 0;
      virtual bool       detach (PMO& placement) = 0;
      
      virtual MObjectRef getRoot()               = 0;
      
      virtual session::PFix& getFixture ()       = 0;
      virtual void rebuildFixture ()             = 0;
      
    };
  
  
  extern const char* ON_SESSION_START;  ///< triggered before loading any content into a newly created session
  extern const char* ON_SESSION_INIT;   ///< triggered when initialising a new session, after adding content
  extern const char* ON_SESSION_READY;  ///< triggered after session is completely functional and all APIs are open.
  extern const char* ON_SESSION_END;    ///< triggered before discarding an existing session
  
  
  namespace session {

    /**
     * creation, access and Session lifecycle Interface.
     * An instance is accessible via Session::current
     */
    class SessManager : private boost::noncopyable
      {
      public:
        /** diagnostics: session interface opened? */
        virtual bool isUp ()  =0;
        
        /** clear current session contents 
         *  without resetting overall session config.
         *  Afterwards, the session will contain only one 
         *  empty Sequence, while all Assets are retained.
         */
        virtual void clear () =0;
        
        /** reset all session config and 
         *  start with a pristine default session.
         */
        virtual void reset () =0;
        
        /** replace the current session by a new
         *  session loaded from serialised state.
         */
        virtual void load ()  =0;

        /** create a complete, serialised representation
         *  of the current session config and contents.
         *  @todo how to serialise, parameters, return value?
         */
        virtual void save ()  =0;
        
        /** access to the current session object instance.
         *  This is the sole access path available for clients.
         *  @note there is no operator*
         */
        virtual Session* operator-> ()  throw() =0;
        
        virtual ~SessManager();
      };

      
      LUMIERA_ERROR_DECLARE (CREATE_SESSION);  ///< unable to create basic session.


  } // namespace mobject::session

} // namespace mobject
#endif
