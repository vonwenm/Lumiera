/*
  UTIL-QUANT.hpp  -  helper functions to deal with division and quantisation

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

*/


#ifndef LIB_UTIL_QUANT_H
#define LIB_UTIL_QUANT_H

#include <cstdlib>



namespace util {
  
  
  /** helper to treat int or long division uniformly */
  template<typename I>
  struct IDiv
    {
      I quot;
      I rem;
      
      IDiv (I num, I den)
        : quot(num/den)
        , rem(num - quot*den)
        { }
    };
  
  template<>
  struct IDiv<int>
    : div_t
    {
      IDiv<int> (int num, int den)
        : div_t(div (num,den))
        { }
    };
  
  template<>
  struct IDiv<long>
    : ldiv_t
    {
      IDiv<long> (long num, long den)
        : ldiv_t(ldiv (num,den))
        { }
    };
  
  template<>
  struct IDiv<long long>
    : lldiv_t
    {
      IDiv<long long> (long long num, long long den)
        : lldiv_t(lldiv (num,den))
        { }
    };
  
  
  /** floor function for integer arithmetics.
   *  Unlike the built-in integer division, this function
   *  always rounds towards the next \em smaller integer,
   *  even for negative numbers.
   * @warning floor on doubles performs way better
   * @see UtilFloordiv_test
   */
  template<typename I>
  inline I
  floordiv (I num, I den)
  {
    if (0 < (num^den))
      return num/den;
    else
      { // truncate similar to floor()
        IDiv<I> res(num,den);
        return (res.rem)? res.quot-1   // negative results truncated towards next smaller int
                        : res.quot;   //..unless the division result not truncated at all
      }
  }
  
  /** scale wrapping operation.
   *  Quantises the numerator value into the scale given by the denominator.
   *  Unlike the built-in integer division, this function always rounds towards
   *  the next \em smaller integer and also relates the remainder (=modulo) to
   *  this next lower scale grid point.
   * @return quotient and remainder packed into a struct
   * @see UtilFloorwarp_test
   */
  template<typename I>
  inline IDiv<I>
  floorwrap (I num, I den)
  {
    IDiv<I> res(num,den);
    if (0 > (num^den) && res.rem)
      {  // negative results
        //  wrapped similar to floor()
        --res.quot;
        res.rem = den - (-res.rem);
      }
    return res;
  }
  
} // namespace util
#endif /*UTIL_QUANT_H*/
