/*
  Buildable  -  marker interface denoting any MObject able to be treated by Tools
 
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


#include "proc/mobject/buildable.hpp"
#include "proc/mobject/builder/buildertool.hpp"

namespace mobject
  {


  /** typically the provided actual Tool class will
   *  contain overloaded fuctions for treating
   *  different Buildable subclasses specifically
   */
  Buildable::ReturnType 
  Buildable::apply (builder::BuilderTool& tool)
  {
    return dispatchOp (*this, tool);
  }



} // namespace mobject
