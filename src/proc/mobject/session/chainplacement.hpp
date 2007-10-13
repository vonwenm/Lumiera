/*
  CHAINPLACEMENT.hpp  -  Chaining and additionally constraining Placements
 
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


#ifndef MOBJECT_SESSION_CHAINPLACEMENT_H
#define MOBJECT_SESSION_CHAINPLACEMENT_H




namespace mobject
  {
  class Placement;

  namespace session
    {
      class FixedPlacement;
      class RelativePlacement;
      

    template<class MO>
    struct ChainPlacement
      {
      protected:
        /** next additional Placement, if any */
        Placement<MO> * next;
      public:
        
        FixedPlacement& operator() (Time);

      };

  } // namespace mobject::session

} // namespace mobject
#endif
