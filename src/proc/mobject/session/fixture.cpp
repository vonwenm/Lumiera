/*
  Fixture  -  the (low level) representation of the EDL with explicit placement data
 
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
 
* *****************************************************/


#include "proc/mobject/session/fixture.hpp"
#include "nobugcfg.h"

namespace mobject
  {
  namespace session
    {



    list<ExplicitPlacement*> &
    Fixture::getPlaylistForRender ()
    {
      UNIMPLEMENTED ("get Playlist For Render");
    }


    /** TODO: a placeholder for the Operation needed for
     *  wiring the Automation providers in the Build process
     */
    Auto<double>*
    Fixture::getAutomation ()
    {
      UNIMPLEMENTED ("getAutomation from Fixture");
      return 0;
    }



  } // namespace mobject::session

} // namespace mobject
