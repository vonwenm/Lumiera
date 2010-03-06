/*
  Track  -  A grouping device within the Session.
 
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
 
* *****************************************************/


#include "proc/mobject/session/track.hpp"
#include "proc/asset/track.hpp"


namespace mobject {
namespace session {
  
  /** new track-MO linked with the given asset::Track.
   *  Initially, the reference (zero-point) time of this track
   *  will be set to 0 
   */
  Track::Track (PTrackAsset& trackDef)
    : start_(0),
      trackDef_(trackDef)
  {
    throwIfInvalid();
  }
  
  
  bool 
  Track::isValid()  const
  {
    return bool(trackDef_);
  }
  
  
  
}} // namespace mobject::session
