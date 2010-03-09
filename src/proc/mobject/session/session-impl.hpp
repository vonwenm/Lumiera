/*
  SESSION-IMPL.hpp  -  holds the complete session data to be edited by the user
 
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


/** @file session-impl.hpp
 ** Session and SessionServices Implementation classes.
 ** Session and the corresponding Manager are primary Interfaces
 ** to control the behaviour of the editing part of the application.
 ** All all implementation complexities are hidden behind a "PImpl".
 ** 
 ** This file contains the implementation level API, it should never
 ** be included by client code. Besides the actual SessionImpl, a set
 ** of further implementation level services is provided for use by
 ** Proc-Layer's internals. These additional SessionServices are to be
 ** accessed through dedicated headers and interface classes (typically
 ** through static access functions), thereby abstracting from the actual
 ** session implementation. Within this file, the implementation of these
 ** SessionServices is wired up with the SessionImpl object.
 ** 
 ** @see Session public API
 ** @see session-services.hpp
 ** @see session-service-access-test.cpp for a complete simplified mock session manager 
 **
 */


#ifndef MOBJECT_SESSION_SESSIONIMPL_H
#define MOBJECT_SESSION_SESSIONIMPL_H

#include "proc/mobject/session.hpp"
#include "proc/mobject/session/fixture.hpp"
#include "proc/mobject/session/placement-index.hpp"
#include "proc/mobject/session/session-services.hpp"
#include "proc/mobject/session/session-interface-modules.hpp"

#include "proc/mobject/session/session-service-fetch.hpp"
#include "proc/mobject/session/session-service-explore-scope.hpp"
#include "proc/mobject/session/session-service-mock-index.hpp"
#include "proc/mobject/session/session-service-defaults.hpp"

#include "proc/mobject/session/placement-index-query-resolver.hpp"





namespace mobject {
namespace session {
  
  
  
  /**
   * Implementation class for the Session interface
   */
  class SessionImpl
    : protected SessionInterfaceModules
    , public mobject::Session
    {
      PlacementIndex contents_;
      
      PFix fixture_;
      
      
      
      
      /* ==== Session API ==== */
      virtual bool isValid ();
      virtual void attach (PMO& placement);
      virtual bool detach (PMO& placement);
      
      virtual PFix& getFixture ();
      virtual void rebuildFixture ();
      
    protected: /* == management API === */
      SessionImpl ();
      
      void clear ();
      friend class SessManagerImpl;
      
      PlacementIndex&
      getPlacementIndex()
        {
          ENSURE (contents_.isValid());
          return contents_;
        }
      
    };
  
  
  
  /* ===== providing internal services for Proc ===== */
  
  template<class IMPL>
  struct ServiceAccessPoint<SessionServiceFetch, IMPL>
    : IMPL
    {
      bool
      isRegisteredID (PMO::ID const& placementID)
        {
          return IMPL::getPlacementIndex().contains (placementID); //never throws
        }
      
      PMO&
      resolveID (PMO::ID const& placementID)
        {
          return IMPL::getPlacementIndex().find (placementID); //may throw
        }
    };
  
  
  
  
  template<class IMPL>
  struct ServiceAccessPoint<SessionServiceExploreScope, IMPL>
    : IMPL
    {
      QueryResolver&
      getScopeQueryResolver()
        {
          return resolvingWrapper_;
        }
      
      PlacementMO& 
      getScopeRoot()
        {
          return IMPL::getPlacementIndex().getRoot();
        }
      
    private:
      PlacementIndexQueryResolver resolvingWrapper_;
      
      /** indirection to use the \em currently defined
       *  index access point (might be a test mock) */
      struct
      AccessCurrentIndex
        {
          IMPL& accessPoint_;  
          PlacementIndex& operator() (void) { return accessPoint_.getPlacementIndex(); }
          
          AccessCurrentIndex (IMPL& impl) : accessPoint_(impl) { }
        };
      
    protected:
      ServiceAccessPoint<SessionServiceExploreScope, IMPL>()
        : resolvingWrapper_(AccessCurrentIndex (*this))
        { }
    };
  
  
  
  
  
  template<class IMPL>
  struct ServiceAccessPoint<SessionServiceMockIndex, IMPL>
    : IMPL
    {
      PlacementIndex&
      getPlacementIndex()
        {
          if (mockIndex_ && mockIndex_->isValid())
            return *mockIndex_;
          else
            return IMPL::getPlacementIndex();
        }
      
      void
      reset_PlacementIndex (PlacementIndex* alternativeIndex =0)
      {
        mockIndex_ = alternativeIndex;
      }
    
    protected:
      ServiceAccessPoint<SessionServiceMockIndex, IMPL>()
        : mockIndex_(0)
        { }
      
    private:
      PlacementIndex* mockIndex_;
    };
  
  
  
  template<class IMPL>
  struct ServiceAccessPoint<SessionServiceDefaults, IMPL>
    : IMPL
//    , SessionServiceDefaults
    {
      ////////////////////////////TODO
    };
  
  
  
  
  
  
  class SessManagerImpl;
  
  /** 
   * actual configuration of the session implementation compound:
   * forming an inheritance chain of all internal SesssionServices
   * stacked on top of the SessionImpl class.
   * @note SessionImplAPI is actually used within the SessManagerImpl
   *       to create "the session" instance and expose it through the
   *       global Session PImpl
   */
  typedef SessionServices< Types< SessionServiceFetch
                                , SessionServiceExploreScope
                                , SessionServiceMockIndex
                                , SessionServiceDefaults
                                >              // List of the APIs to provide
                         , SessManagerImpl    //  frontend for access
                         , SessionImpl       //   implementation base class
                         >                  //
                         SessionImplAPI;
  
  
  
  
}} // namespace mobject::session
#endif
