/*
  QUERY-FOCUS.hpp  -  management of current scope within the Session

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


#ifndef MOBJECT_SESSION_QUERY_FOCUS_H
#define MOBJECT_SESSION_QUERY_FOCUS_H

#include "proc/mobject/session/scope-path.hpp"
#include "proc/mobject/session/scope-query.hpp"
#include "proc/mobject/session/scope-locator.hpp"
#include "proc/mobject/placement-ref.hpp"

#include <boost/intrusive_ptr.hpp>
#include <string>

namespace proc {
namespace mobject {
namespace session {
  
  
  
  /**
   * Current focus location to use as point-of reference
   * for contents and location discovery queries. This is the
   * frontend to be used by client code: a smart-handle, internally
   * linked through the ScopeLocaor singleton to a stack of current
   * focus path locations. The intention is for this current location
   * to follow the ongoing query/discovery operations mostly automatically.
   * 
   * \par usage
   * 
   * A QueryFocus (frontend handle) can be default constructed, in which
   * case it will automatically connect to what is currently the focus
   * location for any further queries. Here, the current focus location
   * is defined as the most recently used location which is still referred
   * by some QueryFocus handle.
   * 
   * Alternatively, through the static factory function #push(), a new
   * focus location may be opened, thereby pushing the currently used
   * focus location aside. This new focus location will remain the
   * current focus, until all handles referring to it go out of scope.
   * 
   * Using an existing QueryFocus (handle), the current focus may be 
   * shifted to another scope within the current session. This
   * »navigating« operation will use the current focus position as
   * point of departure, thus retaining a similar access path to any
   * nested sequences. (These might be attached multiple times within
   * the same session, each attachment constituting a different
   * context scope. Navigating tries to retain the current context)
   * 
   * The templated query functions allow to issue specifically typed
   * queries to retrieve all children (immediately contained in a
   * given scope), or do discover depth-first any content within
   * this scope. The result set of these queries will be filtered
   * to yield only placements compatible to the specified kind of
   * MObject. E.g, you may query all Clip objects within a given Track.
   * 
   * The implementation of these query operations is backed by the
   * PlacementIndex in the current session. The link to the session
   * is established the moment these query functions are invoked.
   * The returned iterator (Lumiera Forward Iterator) contains a
   * smart-ptr to keep the hidden result set alive. The results
   * are delivered without any defined order (implementation is
   * hashtable based)
   * 
   * @see QueryFocus_test
   * @see scope-path.hpp (concept: path of scopes)
   */
  class QueryFocus
    {
      boost::intrusive_ptr<ScopePath> focus_;
      
    public:
      QueryFocus();
      
      ScopePath const& currentPath()  const;
      RefPlacement     currentPoint() const;
      operator Scope()                const;
      operator string()               const;
      
      QueryFocus&     attach (Scope const&);
      static QueryFocus push (Scope const&);
      static QueryFocus push ();
      QueryFocus& reset ();
      QueryFocus& pop ();
      
      
      template<class MO>
      typename ScopeQuery<MO>::iterator
      query()  const;
      
      template<class MO>
      typename ScopeQuery<MO>::iterator
      explore()  const;
      
      lib::IterSource<const Scope>::iterator
      locate (Scope const& toTarget);
      
      
    private:
      QueryFocus (ScopePath&);
      static ScopePath& currPath();
    };
  
  
  
  
  
  
  /** allowing direct conversion to Scope.
   *  Implemented by copying the scope at
   *  leaf position of the current focus path
   */
  inline QueryFocus::operator Scope() const
  {
    return focus_->getLeaf();
  }
  
  /** @return ref to internal datastructure
   *  @warning don't store it directly. Rather
   *        copy it or store a QueryFocus instance. 
   *  @note use #attach if the intention is
   *        to \em manipulate the current
   *        focus location */
  inline ScopePath const&
  QueryFocus::currentPath()  const
  {
    return *focus_;
  }
  
  /** @return placement-ref to the object the
   *          QueryFocus is currently pointing at
   */
  inline RefPlacement
  QueryFocus::currentPoint() const
  {
    return RefPlacement (focus_->getLeaf().getTop());
  }
  
  
  /** discover depth-first any matching object
   *  within \em current focus. Resolution is
   *  delegated to the \em current session */
  template<class MO>
  inline typename ScopeQuery<MO>::iterator
  QueryFocus::query()  const
  {
    return ScopeLocator::instance().query<MO> (*this);
  }
  
  
  /** discover any matching object contained
   *  as immediate Child within \em current focus.
   *  Resolution through \em current session */
  template<class MO>
  inline typename ScopeQuery<MO>::iterator
  QueryFocus::explore()  const
  {
    return ScopeLocator::instance().explore<MO> (*this);
  }
  
  
  /** shift or navigate the current focus to point at
   *  the given target scope. In case of multiple possible
   *  access paths, the \em current location is taken into
   *  account, trying to reach the new location in a
   *  \em similar fashion as much as possible.
   *  @note moves the current focus as side-effect
   *  @return the effective / virtual new access path
   *          leading to the new target focus scope */
  inline lib::IterSource<const Scope>::iterator
  QueryFocus::locate (Scope const& toTarget)
  {
    return ScopeLocator::instance().locate (toTarget);
  }
  
  
  
}}} // namespace proc::mobject::session
#endif
