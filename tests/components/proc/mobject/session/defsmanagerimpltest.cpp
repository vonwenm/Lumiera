/*
  DefsManagerImpl(Test)  -  checking implementation details of the defaults manager
 
  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>
 
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


#include "pre_a.hpp"

#include "common/test/run.hpp"
#include "common/util.hpp"

#include "proc/asset.hpp"
#include "proc/asset/pipe.hpp"
#include "common/configrules.hpp"
#include "proc/assetmanager.hpp"
#include "proc/mobject/session.hpp"

#include <boost/format.hpp>
#include <cstdlib>

using boost::format;
using util::isnil;
using std::string;


namespace asset
  {
  namespace test
    {
    using mobject::Session;
    using lumiera::Symbol;
    using lumiera::Query;
    using lumiera::query::normalizeID;
    
    using lumiera::ConfigRules;
    using lumiera::query::QueryHandler;
    
    
    
    /** shortcut: query for given Pipe-ID */
    bool 
    find (const string& pID) 
    { 
      return Session::current->defaults.search (Query<Pipe> ("pipe("+pID+")")); 
    }
    
    
    format pattern ("dummy_%s_%i");
    
    /** create a random new ID */
    string 
    newID (Symbol prefix)
    {
      return str (pattern % prefix % std::rand());
    }
    
    
    
    
    /************************************************************************
     * @test verify some general assumptions regarding implementation details
     *       of the the defaults manager.
     * @see  DefsManager_test for the "big picture"
     */
    class DefsManagerImpl_test : public Test
      {
        virtual void run(Arg arg) 
          {
            define_and_search();
            string pipeID = create();
            forget(pipeID);
          }
        
        
        
        
        void define_and_search ()
          {
            string sID = newID ("stream");
            
             // create Pipes explicitly 
            //  (without utilizing default queries)
            PPipe pipe1 = Struct::create (newID("pipe"), newID("stream")); 
            PPipe pipe2 = Struct::create (newID("pipe"), sID            );
            
            ASSERT (pipe1 != pipe2);
            ASSERT (sID == pipe2->getProcPatt()->queryStreamID());
            
            ASSERT (!find (pipe1->getPipeID()), "accidental clash of random test-IDs");
            ASSERT (!find (pipe2->getPipeID()), "accidental clash of random test-IDs");
            
            // now declare that these objects should be considered "default"
lumiera::query::setFakeBypass("");  /////////////////////////////////////////////////TODO mock resolution            
            ASSERT (Session::current->defaults.define (pipe1, Query<Pipe> (""))); // unrestricted default

lumiera::query::setFakeBypass("stream("+sID+")"); ///////////////////////////////////TODO mock resolution            
            ASSERT (Session::current->defaults.define (pipe2, Query<Pipe> ("stream("+sID+")")));
            
            ASSERT ( find (pipe1->getPipeID()), "failure declaring object as default");
            ASSERT ( find (pipe2->getPipeID()), "failure declaring object as default");
            
            ASSERT (sID != pipe1->getProcPatt()->queryStreamID(), "accidental clash");
            ASSERT (!Session::current->defaults.define (pipe1, Query<Pipe> ("stream("+sID+")")));
                    // can't be registered with this query, due to failure caused by wrong stream-ID
          }
        
        
        const string& create ()
          { 
            string sID = newID ("stream");
            Query<Pipe> query_for_streamID ("stream("+sID+")");
            
            // issue a ConfigQuery directly, without involving the DefaultsManager
            QueryHandler<Pipe>& typeHandler = ConfigRules::instance();  
            PPipe pipe1;
            typeHandler.resolve (pipe1, query_for_streamID);
            ASSERT (pipe1);
            
            ASSERT (!find (pipe1->getPipeID()));
            PPipe pipe2 = Session::current->defaults.create (query_for_streamID);
            ASSERT (pipe2);
            ASSERT (pipe2 == pipe1);
            ASSERT ( find (pipe1->getPipeID())); // now declared as "default Pipe" for this stream-ID
            
            return pipe1->getPipeID();
          }
        
        
        void forget(string pID)
          { 
            PPipe pipe = Pipe::query ("pipe("+pID+")");
            REQUIRE (find (pipe->getPipeID()), "need an object registered as default");
            long cnt = pipe.use_count();
            
            // now de-register the pipe as "default Pipe"
            ASSERT (Session::current->defaults.forget (pipe));
            ASSERT (!find (pipe->getPipeID()));
            ASSERT (cnt == pipe.use_count());   // indicates DefaultsManager holding only a weak ref.
          }
      };
    
    
    /** Register this test class... */
    LAUNCHER (DefsManagerImpl_test, "function session");
    
    
    
  } // namespace test

} // namespace asset