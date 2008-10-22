/* try.cpp  -  for trying out some language features....
 *             scons will create the binary bin/try
 *
 */ 

// 8/07  - how to control NOBUG??
//         execute with   NOBUG_LOG='ttt:TRACE' bin/try
// 1/08  - working out a static initialisation problem for Visitor (Tag creation)
// 1/08  - check 64bit longs
// 4/08  - comparison operators on shared_ptr<Asset>
// 4/08  - conversions on the value_type used for boost::any
// 5/08  - how to guard a downcasting access, so it is compiled in only if the involved types are convertible
// 7/08  - combining partial specialisation and subclasses 


#include <nobug.h>
#include <iostream>
#include <typeinfo>
#include <boost/format.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

using std::string;
using std::cout;
using boost::format;


  namespace {
      
      boost::format fmt ("<%2i>");
      
  }
  
  struct No {};
  
  template<class TY, class P0=No, class P1=No, class P2=No>
  struct Invoker
    {
      static TY*
      call (P0& p0, P1& p1, P2& p2)
        {
          return new TY (p0,p1,p2);
        }
    };
  
int 
main (int argc, char* argv[])
  {
    
    NOBUG_INIT;
    
    cout <<  "\ngulp\n";
    
    
    return 0;
  }
