/*
  SESSION.hpp  -  holds the complete session to be edited by the user
 
  Copyright (C)         CinelerraCV
    2007,               Hermann Vosseler <Ichthyostega@web.de>
 
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
 ** The Interface Session is abstract and only accessible via the
 ** static field Session::current, which actually refers to a SessManager 
 ** singleton instance. The latter acts as smart ptr-to-Impl.
 **
 */


#ifndef MOBJECT_SESSION_H
#define MOBJECT_SESSION_H

#include "proc/mobject/placement.hpp"
#include "common/singleton.hpp"

#include <boost/utility.hpp>
#include <tr1/memory>



namespace mobject
  {
  namespace session 
    { 
    class SessManager;
    class EDL; 
    class Fixture;
    typedef std::tr1::shared_ptr<Fixture> PFix;
    }
  
  typedef session::SessManager& PSess;  ///< acts as a "PImpl" smart ptr

  
  /**
   * The (current) Session holds all the user
   * visible content to be edited and manipulated
   * within the Cinelerra Application. From a users
   * perspective, it is a collection of Media Objects
   * (--> MObject) placed (--> Placement) onto virtual
   * Tracks.
   * 
   * Opening a Session has effectively global consequences, 
   * because the Session defines the available Assets, and some 
   * kinds of Assets define default behaviour. Thus, access to 
   * the Session is similar to a Singleton instance.
   * 
   * @note Any client should be aware that the Session can be closed, 
   *       replaced and loaded. The only way to accees the Session is
   *       via a "PImpl" smart pointer session::PSess (which indeed is
   *       a reference to the SessManager and is accessible as the static
   *       field Session::current). You will never be able to get a direct
   *       pointer or reference to the Session object.
   *  
   */
  class Session : private boost::noncopyable
    {
    protected:
      Session ()  throw();
      virtual ~Session () = 0;

    public:
      static session::SessManager& current;
      
      virtual bool isValid ()              = 0;
      virtual void add (PMO& placement)    = 0;
      virtual bool remove (PMO& placement) = 0;
      
      /// @deprecated Ichthyo doubts it is good design to hand out the EDL??
      virtual session::EDL& currEDL ()     = 0;
      
      virtual session::PFix& getFixture () = 0;
      virtual void rebuildFixture ()       = 0;
      
    };
  
  
  namespace session
    {

    /**
     * creation, access and Session lifecycle Interface.
     * An instance is accessible via Session::current
     */
    class SessManager : private boost::noncopyable
      {
      public:
        /** clear current session contents 
         *  without resetting overall session config.
         *  Afterwards, the session will contain only one 
         *  empty EDL, while all Assets are retained.
         */
        virtual void clear () =0;
        
        /** reset all session config and 
         *  start with a pristine default session.
         */
        virtual void reset () =0;
        
        /** replace the current session by a new
         *  session loaded from serialized state.
         */
        virtual void load () =0;

        /** create a complete, serialized representation
         *  of the current session config and contents.
         *  @todo how to serialize, prameters, return value?
         */
        virtual void save () =0;
        
        /** access to the current session object instance.
         *  This is the sole access path available for clients.
         *  @note there is no operator*
         */
        virtual Session* operator-> ()  throw() =0;
        
        virtual ~SessManager() {};
      };

  } // namespace mobject::session

} // namespace mobject
#endif