/*
  TestClip  -  test clip (stub) for checking EDL/Session functionality
 
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


#include "proc/mobject/session/testclip.hpp"
#include "backend/mediaaccessfacade.hpp"
#include "backend/mediaaccessmock.hpp"
#include "proc/asset/media.hpp"
#include "proc/asset/clip.hpp"
#include "common/singleton.hpp"

namespace mobject
  {
  namespace session
    {
    namespace test
      {
      typedef shared_ptr<asset::Media> PM;
      typedef backend_interface::MediaAccessFacade MAF;
      using backend_interface::test::MediaAccessMock;
      using asset::VIDEO;

      
      
      asset::Media & 
      createTestMedia ()
      {
        // install Mock-Interface to cinelerra backend
        MAF::instance.injectSubclass (new MediaAccessMock);
        PM media = asset::Media::create("test-2", VIDEO); // query magic filename
        MAF::instance.injectSubclass (0); // remove Mock-Interface
        
        return *media;
      }
      
      asset::Clip & 
      createTestClipAsset (asset::Media& media)
      {
        return *(asset::Media::create(media)); 
      }
      
    
      struct Testbed
        {
          asset::Media & media_;
          asset::Clip & clipA_;
          
          Testbed() 
            : media_ (createTestMedia()),
              clipA_ (createTestClipAsset(media_))
            { }
        };
          
      cinelerra::Singleton<Testbed> testbed_1; // invoke ctor when creating first TestClip... 

      
      
      

      TestClip::TestClip ()
        : Clip(testbed_1().clipA_,
               testbed_1().media_)
      {
        ASSERT (isValid());
      }
      


    } // namespace test
  
  } // namespace session

} // namespace mobject