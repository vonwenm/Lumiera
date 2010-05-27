/*
  Advice  -  generic loosely coupled interaction guided by symbolic pattern
 
  Copyright (C)         Lumiera.org
    2010,               Hermann Vosseler <Ichthyostega@web.de>
 
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


#include "lib/advice.hpp"
#include "lib/advice/index.hpp"


namespace lib {
namespace advice {
  
//  LUMIERA_ERROR_DEFINE (MISSING_INSTANCE, "Existing ID registration without associated instance");

  
  /* ohlolololohaha */
  void
  PointOfAdvice::publishProvision()
  {
    UNIMPLEMENTED ("change advice provision registration");
  }
  
  
  void
  PointOfAdvice::discardSolutions ()
  {
    UNIMPLEMENTED ("notify index of retracted advice");
  }
  
  
  void
  PointOfAdvice::publishBindingChange ()
  {
    UNIMPLEMENTED ("propagate binding change to index");
  }
      
      
  void
  PointOfAdvice::publishRequestBindingChange()
  {
    UNIMPLEMENTED ("propagate binding change to index");
  }
  
  
  void
  PointOfAdvice::registrateRequest()
  {
    UNIMPLEMENTED ("registrate request with the index");
  }
  
  
  void
  PointOfAdvice::deregistrateRequest()
  {
    UNIMPLEMENTED ("detach request from index");
  }


  
  
  
}} // namespace lib::advice
