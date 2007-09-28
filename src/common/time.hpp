/*
  TIME.hpp  -  unified representation of a time point, including conversion functions
 
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


#ifndef CINELERRA_TIME_H
#define CINELERRA_TIME_H



namespace cinelerra
  {


  /**
   * denotes a temporal position (time point), based on timeline start.
   * 
   * @todo currently (9/07) this is a dummy implementation to find out
   *       what interface the Proc layer needs. Cehteh has already written
   *       elaborate timehandling functions in the backend and the goal
   *       is for class Time to be just a thin wrapper!
   */
  class Time
    {
      int dummy;
    public:
      Time(int dum=0) : dummy(dum) {}
      operator int () { return dummy; }
    };

} // namespace cinelerra
#endif
