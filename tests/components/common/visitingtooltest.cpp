/*
  VisitingTool(Test)  -  check the standard visitor use case
 
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


#include "common/test/run.hpp"
#include "common/visitor.hpp"
//#include "common/util.hpp"

#include <boost/format.hpp>
#include <iostream>

using boost::format;
using std::string;
using std::cout;


namespace cinelerra
  {
  namespace visitor
    {
    namespace test
      {
      typedef visitor::Tool<> VisitingTool;
      
      class VerboseVisitor
        : public VisitingTool
        {
        protected:
          void talk_to (string guy)
            {
              cout << format ("Hello %s, nice to meet you...\n") % guy;
            }
        };

        
      class HomoSapiens : public Visitable<>
        {
        public:
          DEFINE_PROCESSABLE_BY (VisitingTool);
        };
      
      class Boss : public HomoSapiens
        {
        public:
          DEFINE_PROCESSABLE_BY (VisitingTool);
        };
        
      // the classes above comprise the standard visitor use case,
      // now we'll extend the hierarchy a bit...
      

      class BigBoss : public Boss
        {
        public:
          DEFINE_PROCESSABLE_BY (VerboseVisitor);
        };

        
      class Babbler
        : public Applicable<Boss,Babbler>,
          public Applicable<BigBoss,Babbler>,
          public ToolType<Babbler, VerboseVisitor>
        {
        public:
          void treat (Boss&)    { talk_to("Boss"); }
          void treat (BigBoss&) { talk_to("Big Boss"); }
        };

      
      class Leader : public Boss
        {
        };
        
      class Visionary : public Leader
        {
        };
        
      // note the following details:
      // - Babbler "forgot" to declare being applicable to HomoSapiens
      // - BigBoss accepts only the subclass (VerboseVisitor)
      // - we have new derived classes without separate "apply()"-implementation 
        
      
      
      
      /*************************************************************************
       * @test our lib implementation of the visitor pattern.
       *       Defines a hierarchy of test classes to check the following cases
       *       <ul><li>calling the correct visiting tool specialized function
       *               for given concrete hierarchy classes</li>
       *           <li>visiting tool not declaring to visit some class 
       *               is silently ignored by default</li>
       *           <li>newly added classes will be handled by existing
       *               functions according to inheritance relations</li>
       *       </ul>
       */
      class VisitingTool_test : public Test
        {
          virtual void run(Arg arg) 
            {
              known_visitor_known_class();
              visiting_extended_hierarchy();
            } 
          
          void known_visitor_known_class()
            {
              Boss x1;
              BigBoss x2;
              
              // masquerade as HomoSapiens...
              HomoSapiens& homo1 (x1);
              HomoSapiens& homo2 (x2);
              
              cout << "=== Babbler meets Boss and BigBoss ===\n";
              Babbler bab;
              VisitingTool& vista (bab);
              homo1.apply (vista);
              homo2.apply (vista);
            }
          
          void visiting_extended_hierarchy()
            {
              HomoSapiens x1;
              Visionary x2;
              
              HomoSapiens& homo1 (x1);
              HomoSapiens& homo2 (x2);
              
              cout << "=== Babbler meets HomoSapiens and Visionary ===\n";
              Babbler bab;
              VisitingTool& vista (bab);
              homo1.apply (vista);  // error handler (not Applicable to HomoSapiens)
              homo2.apply (vista); //  treats Visionary as Boss
            }
            
        };
      
      
      /** Register this test class... */
      LAUNCHER (VisitingTool_test, "unit common");
      
      
      
    } // namespace test
    
  } // namespace visitor

} // namespace cinelerra
