/*
  EFFECT.hpp  -  Model representation of a pluggable and automatable effect.
 
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
 
*/


#ifndef MOBJECT_SESSION_EFFECT_H
#define MOBJECT_SESSION_EFFECT_H


#include "proc/mobject/session/abstractmo.hpp"
#include "proc/asset/effect.hpp"

#include <string>


using std::string;


namespace mobject {
namespace session {
  
  using asset::PProc;
  
  
  class Effect : public AbstractMO
    {
    protected:
      /** Identifier of the Plug-in to be used */
      string pluginID;
      
    public:
              //////////////////////////////TICKET #566
      
      
      /** access the underlying processing asset */
      asset::Proc const& getProcAsset ()  const;
                                                //////////////////////TODO: clarify asset->mobject relation and asset dependencies; Ticket #255
      
    };
  
  
  
}} // namespace mobject::session
#endif
