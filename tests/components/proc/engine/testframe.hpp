/*
  TESTFRAME.hpp  -  test data frame (stub) for checking Render engine functionality

  Copyright (C)         Lumiera.org
    2011,               Hermann Vosseler <Ichthyostega@web.de>

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


#ifndef PROC_ENGINE_TESTFRAME_H
#define PROC_ENGINE_TESTFRAME_H


//#include "lib/time/timevalue.hpp"

//#include <string>


//using std::tr1::shared_ptr;
//using std::string;


namespace engine {
namespace test   {
  
  
//class TestPlacement;
  
  /**
   * Mock data frame for simulated rendering.
   * A test frame can be created and placed instead of a real data frame.
   * It doesn't depend on any external libraries and will be self-maintaining.
   * Placeholder functions are provided for assignment (simulating the actual
   * calculations); additional diagnostic functions allow to verify the
   * performed operations after-the fact
   *  
   * @todo WIP-WIP-WIP 9/11
   * 
   */
  class TestFrame
    {
      
    public:
    };
  
  
  
  TestFrame
  testData (uint seqNr)
  {
    UNIMPLEMENTED ("build, memorise and expose test data frames on demand");
  }
  
  TestFrame
  testData (uint chanNr, uint seqNr)
  {
    UNIMPLEMENTED ("build, memorise and expose test data frames on demand (multi-channel)");
  }
  
  
  
  /* == some test data to check == */
  // extern const lib::time::Duration LENGTH_TestClip;
  
  
}} // namespace engine::test
#endif
