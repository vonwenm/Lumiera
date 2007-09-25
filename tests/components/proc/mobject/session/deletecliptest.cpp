/*
  DeleteClip(Test)  -  removing an Clip-MObject from the Session
 
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
 
* *****************************************************/


#include "common/test/run.hpp"
#include "proc/assetmanager.hpp"
#include "proc/mobject/session/session.hpp"
#include "proc/mobject/session/testsession1.hpp"
//#include "common/util.hpp"

//#include <boost/format.hpp>
#include <iostream>

//using boost::format;
using std::string;
using std::cout;


namespace mobject
  {
  namespace session
    {
    namespace test
      {
      
      
      
      
      /*******************************************************************
       * @test removing a test clip from the EDL.
       * @see  mobject::session::Clip
       * @see  mobject::session::EDL
       */
      class DeleteClip_test : public Test
        {
          virtual void run(Arg arg) 
            {
              buildTestseesion1();
              Session& sess = Session::getCurrent();
              AssetManager& aMang = AssetManager::instance();
              
              PPla clipPlacement = sess.getEDL().find(SESSION1_CLIP); // global Var asigned in buildTestsession1()
              PAsset clipAsset = aMang.getAsset(clipPlacement->subject->getMedia());
              IDA clipAID = clipAsset->getID();
              ASSERT (clipPlacement);
              
              sess.remove (clipPlacement);
              
              ASSERT (!sess.getEDL().find(SESSION1_CLIP));                      // EDL forgot the Clip/Placement
              ASSERT (!aMang.known (clipAID));                                  // corresponding Clip Asset has disappeared 
              ASSERT (!aMang.getAsset(clipPlacement->subject->getMedia()));     // internal cross-links removed
            } 
        };
      
      
      /** Register this test class... */
      LAUNCHER (DeleteClip_test, "function session");
      
      
      
    } // namespace test
  
  } // namespace session

} // namespace mobject
