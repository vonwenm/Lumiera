/*
  CONMANAGER.hpp  -  manages the creation of additional ProcNode connections for the Renderengine
 
  Copyright (C)         CinelerraCV
    2007,               Christian Thaeter <ct@pipapo.org>
 
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


#ifndef MOBJECT_BUILDER_CONMANAGER_H
#define MOBJECT_BUILDER_CONMANAGER_H



namespace mobject
  {
  namespace builder
    {


    /**
     * Connection Manager: used to build the connections between render engine nodes
     * if these nodes need to cooperate besides the normal "data pull" operation. 
     * Esp. the Connection Manager knows how to wire up the effect's parameters 
     * with the corresponding ParamProviders (autmation) in the Session.
     */
    class ConManager
      {
      public:
        /**
         * TODO just a design sketch, work out signatures etc...
         */
        int getConnection () ;
      };



  } // namespace mobject::builder

} // namespace mobject
#endif
