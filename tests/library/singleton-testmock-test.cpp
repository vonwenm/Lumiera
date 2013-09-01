/*
  SingletonTestMock(Test)  -  using Singleton for injecting Test-Mocks

  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>

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
#include "lib/singleton.hpp"
#include "lib/util.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <iostream>

using boost::lexical_cast;
using boost::format;
using util::isnil;
using std::string;
using std::cout;


namespace lib {
namespace test{
  
  
  /**
   * Client Class normally to be instantiated as Singleton.
   * But for tests, this class should be replaced by a Mock....
   */
  class TestSingletonO
    {
      int callCnt;
      Symbol typid;
      format msg;
      
    public:
      TestSingletonO(Symbol ty="TestSingletonO")
          : callCnt (0), typid(ty), msg ("%s::doIt() call=%d\n")
      {
        TRACE (test, "ctor %s", typid.c());
      }
      virtual ~TestSingletonO()
      {
        TRACE (test, "dtor %s", typid.c());
      }
      
      void doIt ()
      {
        ++callCnt;
        cout << msg % typid % callCnt;
      }
      int getCnt ()
      {
        return callCnt;
      }
      
    };
  
  
  /**
   * Mock-1 to replace the Client Class...
   */
  struct Mock_1 : TestSingletonO
    {
      Mock_1() : TestSingletonO("Mock_1")
      {};
    };
  
  /**
   * Mock-2 to replace the Client Class...
   */
  struct Mock_2 : TestSingletonO
    {
      Mock_2() : TestSingletonO("Mock_2")
      {};
    };
  
  
  
  
  
  
  
  
  
  
  /*******************************************************************
   * @test inject a Mock object into the Singleton Factory,
   *       to be returned and used in place of the original object.
   * Expected results: Mock(s) called, no memory leaks.
   * @see  lib::Singleton
   * @see  lib::singleton::Static
   */
  class SingletonTestMock_test : public Test
    {
      
      Singleton<TestSingletonO> sing;
      
      void
      run (Arg arg)
        {
          string scenario = isnil(arg)? "default" : arg[1];
          
          if (scenario == "default")
            injectBoth();
          else
            if (scenario == "noMock")
              noMock();
            else
              if (scenario == "onlyMock")
                onlyMock();
              else
                if (scenario == "firstMock")
                  firstMock();
        }
      
      
      /** @test complete use sequence: first access the Client Object,
       *        then replace it by two different mocks, and finally
       *        restore the original Client Object 
       */
      void
      injectBoth ()
        {
          sing().doIt();
          sing().doIt();
          CHECK (sing().getCnt() == 2);
          
          sing.injectSubclass (new Mock_1);
          sing().doIt();
          sing().doIt();
          sing().doIt();
          sing().doIt();
          sing().doIt();
          CHECK (sing().getCnt() == 5);
          
          sing.injectSubclass (new Mock_2);
          sing().doIt();
          CHECK (sing().getCnt() == 1);
          
          sing.injectSubclass (0);  // un-shadowing original instance
          CHECK (sing().getCnt() == 2);
          sing().doIt();
          CHECK (sing().getCnt() == 3);
        }
      
      
      /** @test just use Singleton Factory normally without any Mock. */
      void
      noMock ()
        {
          sing().doIt();
        }
      
      
      /** @test inject the Mock prior to using the Singleton Factory,
       *        thus the original Client Object shouldn't be created.*/
      void
      onlyMock ()
        {
          sing.injectSubclass (new Mock_1);
          sing().doIt();
        }
      
      
      /** @test inject the Mock prior to using the Singleton Factory,
       *        but then reset the Mock, so following calls should
       *        create the original Client Object. 
       */
      void
      firstMock ()
        {
          sing.injectSubclass (new Mock_1);
          sing().doIt();
          sing().doIt();
          CHECK (sing().getCnt() == 2);
          
          sing.injectSubclass (0);
          sing().doIt();
          CHECK (sing().getCnt() == 1);
        }
    };
  
  
  
  /** Register this test class... */
  LAUNCHER (SingletonTestMock_test, "unit common");
  
  
  
}} // namespace lib::test