/*
  PlacementHierarchy(Test)  -  cooperating hierarchical placement types
 
  Copyright (C)         Lumiera.org
    2009,               Hermann Vosseler <Ichthyostega@web.de>
 
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


#include "lib/test/run.hpp"
#include "lib/test/test-helper.hpp"
#include "proc/mobject/test-dummy-mobject.hpp"
#include "proc/mobject/session/clip.hpp"
#include "proc/mobject/placement.hpp"
#include "proc/asset/media.hpp"
#include "lib/util.hpp"

#include <tr1/memory>
#include <iostream>

using lib::HashIndexed;

using std::tr1::shared_ptr;
using std::string;
using std::cout;
using std::endl;


namespace mobject {
namespace session {
namespace test    {
  
  using session::Clip;
  using lib::test::showSizeof;
  using namespace mobject::test;
  
  
  /***************************************************************************************
   * @test creating placements specifically typed, forming an hierarchy of placement types
   *       which loosely mirrors the hierarchy of the pointee types. Assignments between
   *       placement of differing type, while still sharing ownership.
   * @see  mobject::Placement
   * @see  mobject::MObject#create
   * @see  lib::hash::HashIndexed
   */
  class PlacementHierarchy_test : public Test
    {
      
      virtual void
      run (Arg) 
        {
          typedef Placement<MObject>                    PMObj;
          typedef TestPlacement<>                       PDummy;
          typedef TestPlacement<TestSubMO1>             PSub1;
          typedef TestPlacement<TestSubMO2>             PSub2;
          typedef TestPlacement<TestSubMO21,TestSubMO2> PSub21;
          
          PSub1  pSub1(*new TestSubMO1);
          PSub2  pSub2(*new TestSubMO2);
          PSub21 pSub3(*new TestSubMO21);
          
          PDummy pSubM (pSub3);
          
          PMObj  pClip = asset::Media::create("test-1", asset::VIDEO)->createClip();
          
          ASSERT (INSTANCEOF (Placement<MObject>, &pSub1));
          ASSERT (INSTANCEOF (Placement<MObject>, &pSub2));
          ASSERT (INSTANCEOF (Placement<MObject>, &pSub3));
          ASSERT (INSTANCEOF (Placement<MObject>, &pSubM));
          
          ASSERT (INSTANCEOF (Placement<DummyMO>, &pSub1));
          ASSERT (INSTANCEOF (Placement<DummyMO>, &pSub2));
          ASSERT (INSTANCEOF (Placement<DummyMO>, &pSub3));
          
          ASSERT (INSTANCEOF (TestPlacement<DummyMO>, &pSub1));
          ASSERT (INSTANCEOF (TestPlacement<DummyMO>, &pSub2));
          ASSERT (INSTANCEOF (TestPlacement<DummyMO>, &pSub3));
          
          ASSERT ( INSTANCEOF (TestPlacement<TestSubMO2>,  &pSub3));
          
          // the following won't succeed...
//        ASSERT (INSTANCEOF (TestPlacement<TestSubMO21>, &pSub2)); // parent not instance of subclass
//        ASSERT (INSTANCEOF (TestPlacement<TestSubMO2>,  &pSub1)); // separate branch in the hierarchy
          
          cout << showSizeof(pSub1) << endl;
          cout << showSizeof(pSub2) << endl;
          cout << showSizeof(pSub3) << endl;
          cout << showSizeof(pSubM) << endl;
          cout << showSizeof(pClip) << endl;
          
          ASSERT (sizeof(pSub1) == sizeof(pSub3));
          ASSERT (sizeof(pClip) == sizeof(pSub3));
          
          cout << string(pSub1) << endl;
          cout << string(pSub2) << endl;
          cout << string(pSub3) << endl;
          cout << string(pSubM) << endl;
          cout << string(pClip) << endl;
          cout << pSub1->operator string() << endl;
          cout << pSub2->operator string() << endl;
          cout << pSubM->operator string() << endl;
          
          pSub3->specialAPI();
          
          ASSERT (2 == pSubM.use_count());
          ASSERT (2 == pSub3.use_count());
          pClip = pSubM;                      // slicing and shared ownership
          ASSERT (3 == pSubM.use_count());
          ASSERT (3 == pSub3.use_count());
          ASSERT (3 == pClip.use_count());
          
          
          // now do a brute-force re-interpretation
          // note this is still protected by an ASSERT on the operator->()
          PSub21& hijacked = reinterpret_cast<PSub21&> (pClip);
          
          hijacked->specialAPI();
          ASSERT (3 == hijacked.use_count());
          ASSERT (hijacked.getID() == pSub3.getID());
          
          cout << format_PlacementID(pSub1) << endl;
          cout << format_PlacementID(pSub2) << endl;
          cout << format_PlacementID(pSub3) << endl;
          cout << format_PlacementID(pSubM) << endl;
          cout << format_PlacementID(pClip) << endl;
          
          pClip = pSub1;
          ASSERT (2 == pSubM.use_count());
          ASSERT (2 == pSub3.use_count());
          
          ASSERT (2 == pClip.use_count());
          ASSERT (2 == pSub1.use_count());
          
///////////////////////////////////////////////////////////////////////////////TODO: find a way to configure NoBug to throw in case of assertion
///////////////////////////////////////////////////////////////////////////////TODO: configure NoBug specifically for the testsuite
//        try
//          {
//            hijacked->specialAPI();
//            NOTREACHED;
//          }
//        catch (...)
//          {
//            ASSERT (lumiera_error () == error::LUMIERA_ERROR_ASSERTION);
//          }
        }
    };
  
  
  /** Register this test class... */
  LAUNCHER (PlacementHierarchy_test, "unit session");
  
  
}}} // namespace mobject::session::test