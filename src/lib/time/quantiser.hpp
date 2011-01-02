/*
  QUANTISER.hpp  -  aligning time values to a time grid

  Copyright (C)         Lumiera.org
    2010,               Hermann Vosseler <Ichthyostega@web.de>

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


#ifndef LIB_TIME_QUANTISER_H
#define LIB_TIME_QUANTISER_H

#include "lib/time/timevalue.hpp"
#include "lib/time/formats.hpp"

//#include <boost/operators.hpp>
#include <string>


namespace lib {
namespace time {
  
  
  /**
   * Facility to create grid-aligned time values.
   * 
   * @todo WIP-WIP-WIP
   */
  class Quantiser
    {
      
    public:
    };
  
  
  
  /**
   * Simple stand-alone Quantiser implementation for debugging and test.
   * This is a self-contained quantiser implementation without any implicit
   * referral to the Lumiera session. It is mainly intended for simplified unit testing.
   * @warning real GUI and Proc-Layer code should always prefer to build a real quantiser,
   * which referres some TimeGrid definition within the session. Basically, the overall
   * purpose of the time-quantisation framework is to enforce such a link to a specific
   * time and quantisation scale and to prevent "wild and uncoordinated" rounding attempts.
   */
  class FixedFrameQuantiser
    : public Quantiser
    {
      
    public:
      FixedFrameQuantiser (TimeFract frames_per_second);
    };
  
  
  
}} // lib::time
#endif
