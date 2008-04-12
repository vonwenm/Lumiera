/* try.cpp  -  for trying out some language features....
 *             scons will create the binary bin/try
 *
 */ 

// 8/07  - how to control NOBUG??
//         execute with   NOBUG_LOG='ttt:TRACE' bin/try
// 1/08  - working out a static initialisation problem for Visitor (Tag creation)
// 1/08  - check 64bit longs
// 4/08  - comparison operators on shared_ptr<Asset>


#include <nobug.h>
#include <iostream>
#include <typeinfo>



using std::string;
using std::cout;




int main (int argc, char* argv[])
  {
    
    NOBUG_INIT;
    
    cout <<  "\ngulp\n";
    
    
    return 0;
  }
