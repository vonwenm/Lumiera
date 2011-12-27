/*
  FormatString(Test)  -  validate string template formatting (printf style)

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

* *****************************************************/


#include "lib/test/run.hpp"
#include "lib/format-string.hpp"
#include "lib/error.hpp"
#include "lib/util.hpp"

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <cstdlib>

using boost::lexical_cast;
using std::cout;
using std::endl;


namespace util {
namespace test {
  
  namespace error = lumiera::error;
  
  
  
  /*******************************************************************************
   * @test verifies our front-end for printf-style format string based formatting.
   *       - the implementation is based on boost::format
   *       - we create a wrapper-object on-the fly, which is able to hold
   *         the result of a partial invocation
   *       - explicit specialisations feed all primitive types directly
   *         down into the implementation level. If possible, a custom operator string()
   *         will be used for formatting.
   *       - any kind of exception will be logged, but handled gracefully
   * 
   * @see format-string.hpp
   * @see boost::format
   */
  class FormatString_test : public Test
    {
      void
      run (Arg)
        {
          check_simpleInvocation();
          check_delayedInvocation();
          verify_typeHandling();
          verify_customType();
          verify_errorHandling();
          verify_pointerHandling();
        }
      
      
      
      void
      check_simpleInvocation ()
        {
          string formatted = _Fmt("--format-template--int=%+04d--double=%5.2f--string=%-10s--")
                                 % 12
                                 % 1.228
                                 % "Lumiera";
          cout << formatted << endl;
          CHECK (formatted == "--format-template--int=0012--double= 1.23--string=Lumiera   --");
        }
      
      
      void
      check_delayedInvocation ()
        {
          string formatString = "%p %|20T_| %u";
          _Fmt formatter (formatString);
          
          uint val = rand() % 100;
          
          formatter % &val;
          formatter % val;
          
          cout << formatter << endl;
        }
      
      
      void
      verify_typeHandling ()
        {
          int i(-12);
          CHECK (_Fmt("%d") % i     == "-12"    );
          CHECK (_Fmt("%6d") % i    == "-   12" );
          CHECK (_Fmt("%-6d") % i   == "-12   " );
          CHECK (_Fmt("%+-6d") % -i == "+12   " );
          CHECK (_Fmt("%+06d") % -i == "+00012" );
          CHECK (_Fmt("%+06X") % -i == "+0000D" );
          CHECK (_Fmt("%+#X") % -i  == "+Dh"    );
          
          uint u(12);
          CHECK (_Fmt("%d") % u     == "12"     );
          CHECK (_Fmt("%6d") % u    == "    12" );
          CHECK (_Fmt("%-6d") % u   == "12    " );
          CHECK (_Fmt("%+-6d") % u  == "+12   " );
          CHECK (_Fmt("%+06d") % u  == "+00012" );
          
          short sh(-123);
          CHECK (_Fmt("%6d") % sh   == "-  123" );
          
          ushort ush(123);
          CHECK (_Fmt("%6d") % ush  == "   123" );
          
          long l(-123);
          CHECK (_Fmt("%6d") % l    == "-  123" );
          
          ulong ul(123);
          CHECK (_Fmt("%6d") % ul   == "   123" );
          
          int64_t ll(5e+9);
          CHECK (_Fmt("%d") % ll    == "5000000000" );
          CHECK (_Fmt("%10.3d") %ll == "       500" );
          CHECK (_Fmt("%10.3E") %ll == "   5.000E9" );
          
          uint64_t ull(ll);
          CHECK (_Fmt("%d") % ull   == "5000000000" );
          
          float f(12.34);
          CHECK (_Fmt("%g") % f     == "12.34"  );
          CHECK (_Fmt("%d") % f     == "12"     );
          CHECK (_Fmt("%e") % f     == "12.34e0");
          
          double d(-12.34);
          CHECK (_Fmt("%g") % d     == "-12.34" );
          
          char c(0x40);
          CHECK (_Fmt("%d") % c     == "64"     );
          CHECK (_Fmt("%x") % c     == "40"     );
          CHECK (_Fmt("%o") % c     == "100"    );
          CHECK (_Fmt("%c") % c     == "@"      );
          CHECK (_Fmt("%s") % c     == "@"      );
          
          unsigned char uc(0xff);
          CHECK (_Fmt("%d") % uc    == "255"    );
          CHECK (_Fmt("%x") % uc    == "ff"     );
          CHECK (_Fmt("%X") % uc    == "FF"     );
          CHECK (_Fmt("%c") % uc    == "\xFF"   );
          CHECK (_Fmt("%s") % uc    == "\xFF"   );
          
          c = char(uc);
          CHECK (_Fmt("%d") % uc    == "-1"     );
          CHECK (_Fmt("%c") % uc    == "\xFF"   );
          
          string str("Lumiera");
          CHECK (_Fmt("%s") % str   == "Lumiera"    );
          CHECK (_Fmt("%10s") % str == "   Lumiera" );
          CHECK (_Fmt("%8.4s") %str == "   Lumi"    );
          CHECK (_Fmt("%10c") % str == "         L" );
        }
      
      
      /* == some custom types to test with == */
      
      struct Silent
        {
          int i_;
          
          Silent(int i) : i_(i) { }
        };
      
      struct Verbose
        : Silent
        {
          Verbose(int i) : Silent(i) { }
          
          virtual
          operator string()  const
            {
              return _Fmt("Number-%03d") % i_; 
            }
        };
      
      struct Explosive
        : Verbose
        {
          Explosive(int i) : Verbose(i) { }
          
          operator string()  const
            {
              if (23 == i_)
                throw error::State("encountered Fantomas");
              else
                return Verbose::operator string();
            }
        };
      
      
      void
      verify_customType ()
        {
          Silent s(12);
          Verbose v(13);
          Explosive x(23);
          
          Silent& rs1 = v;
          Silent& rs2 = x;
          
          Verbose& rv = x;
          
          string mangledType = _Fmt("%s") % s;
          cout << mangledType << endl;
          CHECK (contains (mangledType, "util"));
          CHECK (contains (mangledType, "test"));
          CHECK (contains (mangledType, "Silent"));
          
          CHECK (_Fmt("!!%s!!") % v  == "!!Number-013!!");
          CHECK (_Fmt("!!%s!!") % x  == "!!unforeseen internal state (encountered Fantomas).!!");
          
          CHECK (contains (_Fmt("%s") % rs1, "Silent"));
          CHECK (contains (_Fmt("%s") % rs2, "Silent"));
          
          CHECK (_Fmt("!!%s!!") % rv  == "!!unforeseen internal state (encountered Fantomas).!!");
          
          x.i_ = 42;
          CHECK (_Fmt("!!%s!!") % rv  == "!!Number-042!!");
          CHECK (_Fmt("!!%s!!") % x   == "!!Number-042!!");
        }
      
      
      void
      verify_errorHandling ()
        {
          cout << _Fmt("__nix_") % 1 % 2 << endl;
          cout << _Fmt("__%d__") << endl;
          cout << _Fmt("__%d__") % 1 << endl;
          cout << _Fmt("__%d__") % 1 % 2 << endl;
          
          cout << _Fmt("__%d__") % "dirt" << endl;
          cout << _Fmt("__%d__") % "1234" << endl;
          cout << _Fmt("__%d__") % "0xff" << endl;
        }
      
      
      void
      verify_pointerHandling ()
        {
          int             i(-12);   int           *  pi = &  i;
          uint             u(12);   uint          *  pu = &  u;
          short         sh(-123);   short         * psh = & sh;
          ushort        ush(123);   ushort        *push = &ush;
          long           l(-123);   long          *  pl = &  l;
          ulong          ul(123);   ulong         * pul = & ul;
          int64_t       ll(5e+9);   int64_t       * pll = & ll;
          uint64_t       ull(ll);   uint64_t      *pull = &ull;
          float         f(12.34);   float         *  pf = &  f;
          double       d(-12.34);   double        *  pd = &  d;
          char           c(0x40);   char          *  pc = &  c;
          unsigned char uc(0xff);   unsigned char * puc = & uc;
          string  str("Lumiera");   string        *pstr = &str;
          
          CHECK (_Fmt("%d") %   i == _Fmt("%d") %   pi);
          CHECK (_Fmt("%d") %   u == _Fmt("%d") %   pu);
          CHECK (_Fmt("%d") %  sh == _Fmt("%d") %  psh);
          CHECK (_Fmt("%d") % ush == _Fmt("%d") % push);
          CHECK (_Fmt("%d") %   l == _Fmt("%d") %   pl);
          CHECK (_Fmt("%d") %  ul == _Fmt("%d") %  pul);
          CHECK (_Fmt("%d") %  ll == _Fmt("%d") %  pll);
          CHECK (_Fmt("%d") % ull == _Fmt("%d") % pull);
          CHECK (_Fmt("%e") %   f == _Fmt("%e") %   pf);
          CHECK (_Fmt("%e") %   d == _Fmt("%e") %   pd);
          CHECK (_Fmt("%g") %   c == _Fmt("%g") %   pc);
          CHECK (_Fmt("%g") %  uc == _Fmt("%g") %  puc);
          CHECK (_Fmt("%s") % str == _Fmt("%s") % pstr);
          
          Verbose v(42);
          Verbose *pv = &v;
          void    *vv = pv;
          CHECK (_Fmt("__%s__") % v  == "__Number-042__");
          CHECK (_Fmt("__%s__") % pv == "__Number-042__");
          CHECK (_Fmt("__%s__") % vv != "__Number-042__");
          
          string address = lexical_cast<string>(pv);
          CHECK (_Fmt("__%s__") % vv == "__"+address+"__");
          
          pv = NULL;
          vv = NULL;
          CHECK (_Fmt("__%s__") % pv == "__(null)__");
          CHECK (_Fmt("__%s__") % pv == "__0__");
        }
    };
  
  LAUNCHER (FormatString_test, "unit common");
  
  
}} // namespace util::test
