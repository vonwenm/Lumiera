/*
  MOBJECT-REF.hpp  -  active external reference to an MObject within the Session
 
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


/** @file mobject-ref.hpp
 ** A reference handle pointing at an MObject, attached (placed) to the session.
 ** Allows client code to access both the object and the placement, even from a
 ** separate Layer or from within plugin code.
 ** 
 ** @see MObject
 ** @see Session
 ** @see PlacementRef
 ** @see PlacementIndex
 **
 */



#ifndef MOBJECT_MOBJECT_REF_H
#define MOBJECT_MOBJECT_REF_H

//#include "pre.hpp"
//#include "proc/mobject/session/locatingpin.hpp"
//#include "proc/asset/pipe.hpp"
#include "lib/handle.hpp"
#include "proc/mobject/placement.hpp"
#include "proc/mobject/placement-ref.hpp"

//#include <tr1/memory>

///////////////////////////////////////////TODO: define an C-API representation here, make the header multilingual!

namespace mobject {
  
//  using std::tr1::shared_ptr;
  
  
  class MObject;
  
  
  /**
   * An active (smart-ptr like) external reference
   * to a specifically placed MObject "instance" within the session.
   * Implemented as a smart-ptr sharing ownership of the pointee MObject
   * with the corresponding placement within the session. Additionally,
   * a PlacementRef is incorporated, allowing to re-access this placement
   * with the help of PlacementIndex within the current session.
   */
  template<class MO =MObject>
  class MORef
    : public lib::Handle<MO>
    {
      typedef lib::Handle<MO> _Par;
      
      PlacementRef<MO> pRef_; ////////////////////////////////////////////////////////////////////TODO: how to create an "inactive" PlacementRef???
      
      using _Par::smPtr_;
      
    public:
      
      MO*
      operator-> ()  const
        {
          REQUIRE (pRef_ && smPtr_, "Lifecycle-Error: not activated");
          ENSURE (INSTANCEOF (MO, smPtr_.get()));
          return smPtr_.operator-> ();
        }
      
      Placement<MO>& getPlacement()  const
        {
          REQUIRE (pRef_ && smPtr_, "Lifecycle-Error: not activated");
          ENSURE (INSTANCEOF (MO, smPtr_.get()));
          return *pRef_;
        }
      
      
      /* === Lifecycle === */
      
      /** activate an MObject reference, based on an existing placement,
       *  which needs to be contained (added to) the session. After checking
       *  the validity of the placement, this MObjectRef shares ownership
       *  of the referred MObject with the denoted placement.
       *  @note STRONG exception safety guarantee
       *  @see #close() for detaching this MObjectRef
       */
      MORef&
      activate (Placement<MO> const& placement)
        {
          ASSERT (placement.isValid());
          pRef_ = placement;                       // STRONG exception safe
          placement.extendOwnershipTo(smPtr_);     // never throws
          return *this;
        }
      
      /** build and activate an MObject reference, based on anything
       *  which might be assigned to an PlarementRef<MO> :
       *  - any Placement
       *  - any Placement-ID
       *  - any Placement-Ref
       *  - a plain LUID
       * @throws error::Invalid when the (directly or indirectly 
       *         referred placement isn't known to the session PlacementIndex,
       *         or when the placement actually found has an incompatible dynamic type 
       */
      template<typename REF>
      MORef&
      activate (REF const& pRefID)
        {
          PlacementRef<MO> newRef (pRefID);
          if (newRef.isValid()
             && pRef_ != newRef )
            {
              return activate (*newRef); // STRONG exception safe
            }
          else return *this;
        }
      
      /** build and activate an MObject reference based on
       *  an existing reference of the same pointee type
       *  @note STRONG exception safety guarantee
       *  @throws error::Invalid when the referred placement 
       *          isn't known to the current session's PlacementIndex
       */
      MORef&
      activate (MORef const& oRef)
        {
          return activate (oRef.getPlacement());
        }
      
      template<typename MOX>
      MORef&
      activate (MORef<MOX> const& oRef)
        {
          return activate (oRef.getPlacement().getID());
        }
      
      
      
      
      /* == diagnostics == */
      size_t
      use_count()  const
        {
          return pRef_.use_count();
        }

      
      /* == equality comparisons == */
      
      template<class MOX>
      bool
      operator== (MORef<MOX> const& oRef)  const
        {
          return oRef == this->pRef_;
        }
      
      template<class MOX>
      bool
      operator!= (MORef<MOX> const& oRef)  const
        {
          return oRef != this->pRef_;
        }
      
      template<class MOX>
      friend bool
      operator== (MORef const& oRef, PlacementRef<MOX> const& pRef)
        {
          return oRef.pRef_ == pRef;
        }
      
      template<class MOX>
      friend bool
      operator!= (MORef const& oRef, PlacementRef<MOX> const& pRef)
        {
          return oRef.pRef_ != pRef;
        }
      
      template<class MOX>
      friend bool
      operator== (PlacementRef<MOX> const& pRef, MORef const& oRef)
        {
          return pRef == oRef.pRef_;
        }
      
      template<class MOX>
      friend bool
      operator!= (PlacementRef<MOX> const& pRef, MORef const& oRef)
        {
          return pRef != oRef.pRef_;
        }
      
      bool
      operator== (PlacementMO::ID const& pID)  const
        {
          return PlacementMO::ID (pRef_) == pID;
        }
      
      bool
      operator!= (PlacementMO::ID const& pID)  const
        {
          return PlacementMO::ID (pRef_) != pID;
        }
      
    };
  ////////////////TODO currently just fleshing  out the API....
  
  
  typedef MORef<MObject> MObjectRef;
  
  
} // namespace mobject
#endif
