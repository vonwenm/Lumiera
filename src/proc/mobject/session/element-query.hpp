/*
  ELEMENT-QUERY.hpp  -  session sub-interface to query and retrieve elements
 
  Copyright (C)         Lumiera.org
    2010,               Hermann Vosseler <Ichthyostega@web.de>
 
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


#ifndef MOBJECT_SESSION_ELEMENT_QUERY_H
#define MOBJECT_SESSION_ELEMENT_QUERY_H


//#include "lib/p.hpp"
//#include "lib/query.hpp"
#include "proc/mobject/placement.hpp"
#include "proc/mobject/mobject-ref.hpp"
#include "proc/mobject/session/specific-contents-query.hpp"
#include "proc/mobject/session/session-service-explore-scope.hpp"

//#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <tr1/functional>



namespace mobject {
namespace session {
  
  
//  using lumiera::P;
//  using boost::scoped_ptr;
  using std::tr1::function;
  
  
  namespace { // type matching helper
                                                             //////////////////////////////TICKET #644  combine/clean up! see also specific-contents-query.hpp
    template<class PRED>
    struct _PickRes;
    
    template<class MO>
    struct _PickRes<function<bool(Placement<MO> const&)> >
      {
        typedef MO Type;
        typedef MORef<MO> Result;
        typedef typename ScopeQuery<MO>::iterator Iterator;
      };
    
    template<class MO>
    struct _PickRes<bool(&)(Placement<MO> const&)>
      {
        typedef MO Type;
        typedef MORef<MO> Result;
        typedef typename ScopeQuery<MO>::iterator Iterator;
      };
    
    template<class MO>
    struct _PickRes<bool(*)(Placement<MO> const&)>
      {
        typedef MO Type;
        typedef MORef<MO> Result;
        typedef typename ScopeQuery<MO>::iterator Iterator;
      };
  }


  /**
   * Access point to session global search and query functions.
   * This sub-component of the public session interface allows to
   * search and retrieve objects from the high-level-model, by type
   * and using additional filter predicates.
   * 
   * WIP-WIP-WIP as of 6/2010  -- this might evolve into an extended 
   * query facility, using specialised sub-indices and dedicated queries.
   * For now the motivation to package this as a separate interface module
   * was just to reduce the includes on the top level session API and to allow
   * for templated search functions, based on function objects.
   */
  class ElementQuery
    : boost::noncopyable
    {
      
    public:
      
      template<typename PRED>
      typename _PickRes<PRED>::Result
      pick (PRED const& searchPredicate)
        {
          typedef typename _PickRes<PRED>::Result ResultRef;
          typedef typename _PickRes<PRED>::Iterator Iterator;
          
          Iterator iter (pickAllSuitable ( SessionServiceExploreScope::getScopeRoot()
                                         , searchPredicate
                                         , SessionServiceExploreScope::getResolver()
                                         ));
          ResultRef found;
          if (iter)                 // in case there is an result
            found.activate(*iter); //  pick the first element found...
          return found;           //   or return an empty MObjectRef else
        }
      
    };

    

}} // namespace mobject::session
#endif
