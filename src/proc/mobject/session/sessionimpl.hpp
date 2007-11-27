/*
  SESSIONIMPL.hpp  -  holds the complete session data to be edited by the user
 
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


/** @file sessionimpl.hpp
 ** Session and SessionManager Implemention classes.
 ** These are primary Interfaces and we hide all implementaion complexities,
 **
 */


#ifndef MOBJECT_SESSION_SESSIONIMPL_H
#define MOBJECT_SESSION_SESSIONIMPL_H

#include "proc/mobject/session.hpp"
#include "proc/mobject/session/edl.hpp"
#include "proc/mobject/session/fixture.hpp"

#include <boost/scoped_ptr.hpp>
#include <vector>

using std::vector;



namespace mobject
  {
  namespace session
    {


    /**
     * Implementation class for the Session interface
     */
    class SessionImpl : public mobject::Session
      {
      protected:
        uint focusEDL_;
        vector<EDL> edls;
        PFix fixture;
        
        SessionImpl ()  throw();
        friend class SessManagerImpl;
        
        void clear ();
        
      public:
        virtual bool isValid ();
        virtual void add (PMO& placement);
        virtual bool remove (PMO& placement);
        
        virtual EDL&  currEDL ();
        
        virtual PFix& getFixture ();
        virtual void rebuildFixture ();

      };
      
      
    /**
     * Session manager implementation class holding the
     * actual smart pointer to the current Session impl.
     */
    class SessManagerImpl : public SessManager
      {
        boost::scoped_ptr<SessionImpl> pImpl_;
        
        SessManagerImpl()  throw();
        friend class cinelerra::singleton::StaticCreate<SessManagerImpl>;
        
      public:
        virtual void clear () ;
        virtual void reset () ;
        virtual void load ()  ;
        virtual void save ()  ;
        virtual Session* operator-> ()  throw() { return pImpl_.get(); }
      };

      

  } // namespace mobject::session

} // namespace mobject
#endif
