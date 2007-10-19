/*
  ABSTRACTMO.hpp  -  abstract base class for all Media Objects
 
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


#ifndef MOBJECT_SESSION_ABSTRACTMO_H
#define MOBJECT_SESSION_ABSTRACTMO_H

#include "proc/mobject/mobject.hpp"



namespace mobject
  {
  namespace session
    {


    class AbstractMO : public MObject
      {
        ////////////// TODO: work out common services to provide!!!!
        shared_ptr<Placement> placement_;
        
      public:
        
        /* some dummy implementations used to make the code compile... */
        
        virtual Time& getLength()                      { return length; }
        virtual PAsset getMedia ()                        
          { 
            UNIMPLEMENTED ("how to relate MObjects and media assets...");
            return AssetManager::instance().getAsset(IDA(0)); // KABOOM! (just to make it compile)
          }
      protected:
        /** custom deleter func allowing class Placement 
          *  to take ownership of MObjct instances
          */
        static void destroy (MObject* o) { delete o; }
        
        friend class Placement;

     };



  } // namespace mobject::session

} // namespace mobject
#endif
