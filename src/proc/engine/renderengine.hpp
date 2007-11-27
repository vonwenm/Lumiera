/*
  RENDERENGINE.hpp  -  a complete network of processing nodes usable for rendering
 
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


#ifndef ENGINE_RENDERENGINE_H
#define ENGINE_RENDERENGINE_H

#include <list>

#include "proc/engine/processor.hpp"


using std::list;


namespace engine
  {


  class RenderEngine : public Processor
    {
    public:
      /**
       * TODO: will probably be handled differently (see Cehteh)
       */
      void play () ;

    private:
      list<Processor> renderSegments;

    };

} // namespace engine
#endif
