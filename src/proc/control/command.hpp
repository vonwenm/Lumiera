/*
  COMMAND.hpp  -  Key abstraction for proc/edit operations and UNDO management
 
  Copyright (C)         Lumiera.org
    2009,               Hermann Vosseler <Ichthyostega@web.de>
 
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


/** @file command.hpp
 ** //TODO 
 **
 ** @see ProcDispatcher
 ** @see Session
 **
 */



#ifndef CONTROL_COMMAND_H
#define CONTROL_COMMAND_H

//#include "pre.hpp"
#include "include/symbol.hpp"
#include "proc/control/command-mutation.hpp"
#include "proc/control/command-closure.hpp"
#include "lib/bool-checkable.hpp"

//#include <tr1/memory>

///////////////////////////////////////////TODO: define an C-API representation here, make the header multilingual!



namespace control {
  
  using lumiera::Symbol;
//  using std::tr1::shared_ptr;
  
  
  class HandlingPattern;
  
  
  /**
   * @todo Type-comment
   */
  class Command
    : public lib::BoolCheckable<Command>
    {
      
    public:
      /* === command registry === */
      static Command& get (Symbol cmdID);
      static bool  remove (Symbol cmdID); 
      static bool   undef (Symbol cmdID); 
      
      
      
     ~Command();
      
      void operator() () ;
      void undo () ;
      
      
      /** core operation: invoke the command
       *  @param execPattern describes the individual steps
       *         necessary to get this command invoked properly
       */
      void exec (HandlingPattern const& execPattern);
      
      HandlingPattern const& getDefaultHandlingPattern()  const;
      
      
      /* === diagnostics === */
      
      static size_t definition_count();
      static size_t instance_count();
      
      bool isValid()  const;
      bool canExec()  const;
      bool canUndo()  const;
    };
  ////////////////TODO currently just fleshing  out the API....
  

  inline void
  Command::operator() ()
  {
    exec (getDefaultHandlingPattern());
  }

  
  
} // namespace control
#endif
