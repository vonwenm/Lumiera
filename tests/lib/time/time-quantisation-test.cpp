/*
  TimeQuantisation(Test)  -  handling of virtually grid aligned time values

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

* *****************************************************/


#include "lib/test/run.hpp"
#include "lib/test/test-helper.hpp"
#include "lib/time/timequant.hpp"
#include "proc/asset/meta/time-grid.hpp"
#include "lib/util.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <iostream>
//#include <cstdlib>

using boost::lexical_cast;
using util::isnil;
using util::contains;
//using std::rand;
using std::cout;
using std::endl;

using boost::algorithm::join;


namespace lib {
namespace time{
namespace test{
  
  using asset::meta::TimeGrid;
  
  
  /********************************************************
   * @test verify handling of quantised time values.
   *       - creating times and time intervals
   *       - comparisons
   *       - time arithmetics
   */
  class TimeQuantisation_test : public Test
    {
      virtual void
      run (Arg arg) 
        {
          long refval= isnil(arg)?  1 : lexical_cast<long> (arg[1]);
          
          TimeValue ref (refval);
          CHECK (Time(0) < ref);
          
          checkSimpleUsage (ref);
          check_theFullStory (ref);
          checkMultipleGrids (ref);
        } 
      
      
      void
      checkSimpleUsage (TimeValue org)
        {
          TimeGrid::build("my_simple_grid", 25);    // "someone" has defined a time grid
          
          QuTime qVal (org, "my_simple_grid");      // create time quantised to this grid
          
          FrameNr count(qVal);                      // materialise this quantised time into..
          int n = count;                            // frame count, accessible as plain number
          
          CHECK (TimeFract(n-1, 25) < org);         // verify quantisation: the original time
          CHECK (org < TimeFract(n+1, 25));         // is properly bracketed by (n-1, n+2)
        }
      
      
      void
      check_theFullStory (TimeValue org)
        {
          FixedFrameQuantiser fixQ(25);
          QuTime qVal (org, fixQ);
          
          CHECK (contains (qVal.supportedFormats(), format::FRAMES));
          CHECK (contains (qVal.supportedFormats(), format::SMPTE));
          
          TCode<format::Smpte> smpteTCode = qVal.formatAs<format::Smpte>();
          showTimeCode (smpteTCode);
          
          TCode<format::Frames> frameTCode = qVal.formatAs<format::Frames>();
          showTimeCode (frameTCode);
          
          FrameNr count(frameTCode);
          CHECK (string(count) == *(frameTCode.components()));
        }
      
      template<class TC>
      void
      showTimeCode (TC timecodeValue)
        {
          cout << timecodeValue.describe() << " = " << join (timecodeValue.components(), ":") << endl;
        }
      
      
      void
      checkMultipleGrids (TimeValue org)
        {
          TimeGrid::build("my_alternate_grid", TimeFract(30000,1001));
          
          QuTime palVal (org, "my_simple_grid");
          QuTime ntscVal (org, "my_alternate_grid");
          
          CHECK (org == palVal);
          CHECK (org == ntscVal);
          
          FrameNr palNr (palVal);
          FrameNr ntscNr(ntscVal);
          CHECK (palNr < ntscNr);
        }
      
      
      void
      checkGridLateBinding (TimeValue org)
        {
          QuTime funny (org, "special_funny_grid");      // refer a not yet existing grid
          CHECK (org == funny);                          // no problem, unless we request quantisation
          
          VERIFY_ERROR (UNKNOWN_GRID, funny.formatAs<format::Frames>() );
          
          TimeGrid::build("special_funny_grid", 1);      // provide the grid's definition (1 frame per second)
          
          int cnt = funny.formatAs<format::Frames>();    // and now performing quantisation is OK 
          SmpteTC smpte (funny);                         // also converting into SMPTE (which implies frame quantisation)
          CHECK (0 == smpte.getFrames());                // we have 1fps, thus the frame part is always zero!
          CHECK (cnt % 60 == smpte.getSecs());           // and the seconds part will be in sync with the frame count
        }
    };
  
  
  /** Register this test class... */
  LAUNCHER (TimeQuantisation_test, "unit common");
  
  
  
}}} // namespace lib::time::test
