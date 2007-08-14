/*
  FIXTURE.hpp  -  the (low level) representation of the EDL with concrete placement data
 
  Copyright (C)         CinelerraCV
    2007,               Christian Thaeter <ct@pipapo.org>
 
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


#ifndef MOBJECT_SESSION_FIXTURE_H
#define MOBJECT_SESSION_FIXTURE_H

#include <list>

#include "proc/mobject/session/edl.hpp"
#include "proc/mobject/session/track.hpp"
#include "proc/mobject/explicitplacement.hpp"
#include "proc/mobject/session/auto.hpp"


using std::list;



namespace mobject
  {
  namespace session
    {



    class Fixture : public EDL
      {
      protected:
        Track tracks;
        list<ExplicitPlacement*> timeline;


      public:
        list<ExplicitPlacement*> & getPlaylistForRender () ;
        Auto<double>* getAutomation () ; /////TODO: just a placeholder at the moment!!!
      };



  } // namespace mobject::session

} // namespace mobject
#endif