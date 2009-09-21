/*
  COMMAND-IMPL.hpp  -  Proc-Layer command implementation (top level)
 
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


/** @file command-impl.hpp
 ** Top level of the command implementation. CommandImpl holds together
 ** the various data and sub-objects involved into the inner workings of a
 ** Proc-Layer command. It serves to implement a "command definition" (prototype)
 ** as well as a concrete command instance. It is a data holder with a well defined
 ** identity and usually located within the (pooled) storage managed by the
 ** CommandRegistry. Client code gets access to a specific CommandImpl through
 ** a Command instance, which is a small (refcounting smart-ptr) handle.
 ** 
 ** //TODO 
 **
 ** @see Command
 ** @see ProcDispatcher
 **
 */



#ifndef CONTROL_COMMAND_IMPL_H
#define CONTROL_COMMAND_IMPL_H

#include "proc/control/command.hpp"
#include "proc/control/command-closure.hpp"
#include "proc/control/command-mutation.hpp"
#include "proc/control/typed-allocation-manager.hpp"
#include "lib/bool-checkable.hpp"

#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>

#include <tr1/memory>
#include <tr1/functional>


namespace control {
  
  using std::tr1::function;
  using std::tr1::shared_ptr;
  
  
  
  /**
   * Proc-Layer Command implementation.
   * Data record holding together the parts necessary for command execution
   * - command operation functor
   * - a functor to UNDO the command effect
   * - closure holding actual parameters and UNDO state
   */
  class CommandImpl
    : public lib::BoolCheckable<CommandImpl
           , boost::noncopyable
           >
    {
      Mutation do_;
      UndoMutation undo_;
      
      shared_ptr<CmdClosure> pClo_;
      
      HandlingPattern::ID defaultPatt_;
      
      
      template<typename ARG>
      struct _Type
        {
          typedef typename ARG::SIG_op SIG_op;
          typedef typename ARG::SIG_cap SIG_cap;
          typedef typename ARG::SIG_undo SIG_undo;
          
          typedef function<SIG_op> Func_op;
          typedef function<SIG_cap> Func_cap;
          typedef function<SIG_undo> Func_undo;
        };
#define _TY(_ID_) typename _Type<ARG>::_ID_
      
    public:
      /** build a new implementation frame, and do the initial wiring.
       *  On the interface the specific type is discarded afterwards.
       *  This information is still kept though, as encoded into the vtable
       *  of the embedded FunErasure objects holding the command operation
       *  and undo functors, and the vtable of the embedded CmdClosure */
      template<typename ARG>
      CommandImpl (shared_ptr<ARG> pArgHolder
                  ,_TY (Func_op) const& operFunctor
                  ,_TY (Func_cap) const& captFunctor
                  ,_TY (Func_undo) const& undoFunctor
                  )
        : do_(operFunctor)
        , undo_(pArgHolder->tie (undoFunctor, captFunctor))
        , pClo_(pArgHolder)
        , defaultPatt_(HandlingPattern::defaultID())
        { }
      
#undef _TY
      
      
     ~CommandImpl();
      
      
      /** cloning service for the CommandRegistry:
       *  effectively this is a copy ctor, but as we rely
       *  on a argument holder (without knowing the exact type),
       *  we need to delegate the cloning of the arguments down
       *  while providing a means of allocating storage for the clone */
      CommandImpl (CommandImpl const& orig, TypedAllocationManager& storageManager)
        : do_(orig.do_)
        , undo_(orig.undo_)
        , pClo_(orig.pClo_->createClone(storageManager))
        , defaultPatt_(orig.defaultPatt_)
        { }
      
      
      void
      setArguments (Arguments& args)
        {
          pClo_->bindArguments(args);
        }
      
      void invokeOperation() { do_(*pClo_); }
      void invokeCapture()   { undo_.captureState(*pClo_); }
      void invokeUndo()      { undo_(*pClo_); }
      
      
      
      typedef HandlingPattern::ID PattID;
      
      PattID
      getDefaultHandlingPattern()  const
        {
          return defaultPatt_;
        }
      
      /** define a handling pattern to be used by default
       *  @return ID of the currently defined default pattern */
      PattID
      setHandlingPattern (PattID newID)
        {
          PattID currID = defaultPatt_;
          defaultPatt_ = newID;
          return currID;
        }
      
      
      
      /* === diagnostics === */
      
      bool
      isValid()  const    ///< validity self-check: is basically usable.
        {
          return bool(pClo_) 
              && HandlingPattern::get(defaultPatt_).isValid();
        }
      
      bool
      canExec()  const    ///< state check: sufficiently defined to be invoked 
        {
          return isValid()
              && *pClo_;
        }
      
      bool
      canUndo()  const    ///< state check: has undo state been captured? 
        {
          return isValid() && hasUndoState(*pClo_);
        }
      
      
      ////////////////////////////////////////////////////////////////////////////////////TODO comparisons
      
    private:
      
      static bool hasUndoState (CmdClosure const&);
      
    };
  
  
  
  
  
} // namespace control
#endif
