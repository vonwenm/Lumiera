/*
  ERROR-EXCEPTION  -  Lumiera exception classes

  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>

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


/** @file error-exception.cpp
 ** implementation of C++-style error handling.
 ** This compilation unit defines a custom exception class hierarchy,
 ** and is tightly integrated with the C-style error handling.
 ** 
 ** @see error-state.c
 ** @see error.hpp
 **
 */


#include "lib/error.hpp"
#include "lib/util.hpp"

#include <exception>
#include <typeinfo>
#include <iostream>

using util::isnil;
using std::exception;


namespace lumiera {
  
  typedef const char*       CStr;
  typedef const char* const CCStr;
  
  
  namespace error {
    
    /** the message shown to the user per default
     *  if an exception reaches one of the top-level
     *  catch clauses.
     *  @todo to be localised
     */
    inline const string
    default_usermsg (Error* exception_obj)  throw() 
    {
      return string("Sorry, Lumiera encountered an internal error. (")
           + typeid(*exception_obj).name() + ")";
    }
    
    inline CStr
    default_or_given (CCStr id)
    {
      return id? id : LUMIERA_ERROR_STATE;
    }
    
    CStr
    detailInfo ()
    {
      CCStr detailinfo = lumiera_error_extra();
      return isnil (detailinfo)? "Lumiera errorstate detected"
                               : detailinfo;
    }
    
    
    /* constants to be used as error IDs */
    LUMIERA_ERROR_DEFINE (LOGIC    , "internal logic broken");
    LUMIERA_ERROR_DEFINE (FATAL    , "floundered");
    LUMIERA_ERROR_DEFINE (CONFIG   , "misconfiguration");
    LUMIERA_ERROR_DEFINE (STATE    , "unforeseen state");
    LUMIERA_ERROR_DEFINE (FLAG     , "non-cleared lumiera errorstate");
    LUMIERA_ERROR_DEFINE (INVALID  , "invalid input or parameters");
    LUMIERA_ERROR_DEFINE (EXTERNAL , "failure in external service");
    LUMIERA_ERROR_DEFINE (ASSERTION, "assertion failure");
    
    /* some further generic error situations */
    LUMIERA_ERROR_DEFINE (LIFECYCLE, "Lifecycle assumptions violated");
    LUMIERA_ERROR_DEFINE (WRONG_TYPE, "runtime type mismatch");
    LUMIERA_ERROR_DEFINE (ITER_EXHAUST, "end of sequence reached");
    LUMIERA_ERROR_DEFINE (CAPACITY,     "predefined fixed storage capacity");
    LUMIERA_ERROR_DEFINE (INDEX_BOUNDS, "index out of bounds");
    LUMIERA_ERROR_DEFINE (BOTTOM_VALUE, "invalid or NIL value");
    LUMIERA_ERROR_DEFINE (UNCONNECTED, "missing connection");
    LUMIERA_ERROR_DEFINE (UNIMPLEMENTED, "using a feature not yet implemented....");

    
    
  } // namespace error
  
  LUMIERA_ERROR_DEFINE (EXCEPTION, "generic Lumiera exception");
  
  
  
  
  /** @note we set the C-style errorstate as a side effect */
  Error::Error (string description, CCStr id) throw()
    : std::exception (),
      id_ (error::default_or_given (id)),
      msg_ (error::default_usermsg (this)),
      desc_ (description),
      cause_ ("")
  {
    lumiera_error_set (this->id_, description.c_str ());
  }
  
  
  Error::Error (std::exception const& cause, 
                string description, CCStr id) throw()
    : std::exception (),
      id_ (error::default_or_given (id)),
      msg_ (error::default_usermsg (this)),
      desc_ (description),
      cause_ (extractCauseMsg(cause))
  {
    lumiera_error_set (this->id_, description.c_str ());
  }
  
  
  /** @note copy ctor behaves like chaining, i.e setting the cause_. */
  Error::Error (const Error& ref) throw()
    : std::exception (),
      id_ (ref.id_),
      msg_ (ref.msg_),
      desc_ (ref.desc_),
      cause_ (extractCauseMsg(ref))
  { }
  
  
  
  /** Description of the problem, including the internal char constant
   *  in accordance to Lumiera's error identification scheme.
   *  If a root cause can be obtained, this will be included in the
   *  generated output as well. 
   */
  CStr
  Error::what() const  throw()
  {
    if (isnil (this->what_))
      {
        what_ = string(id_);
        if (!isnil (desc_))  what_ += " ("+desc_+").";
        if (!isnil (cause_)) what_ += string(" -- caused by: ") + cause_;
      }
    return what_.c_str(); 
  }
  
  
  /** @internal get at the description message of the 
   *  first exception encountered in a chain of exceptions
   */
  const string
  Error::extractCauseMsg (const exception& cause)  throw()
  {
    const Error* err=dynamic_cast<const Error*> (&cause);
    if (err)
      {
        if (isnil (err->cause_))
          return cause.what(); // cause is root cause
        else
          return err->cause_; // cause was caused by another exception
      }
    
    // unknown other exception type
    return cause.what ();
  }
  
  
  
  
  
  
  
  namespace error
  {
    
    void lumiera_unexpectedException ()  throw()
    {
      CCStr is_halted 
        = "### Lumiera halted due to an unexpected Error ###";
      
      std::cerr << "\n" << is_halted << "\n\n";
      ERROR (NOBUG_ON, "%s", is_halted);
      
      if (CCStr errorstate = lumiera_error ())
        ERROR (NOBUG_ON, "last registered error was....\n%s", errorstate);
      
      std::terminate();
    }
    
    void assertion_terminate (const string& location)
    {
      throw Fatal (location, LUMIERA_ERROR_ASSERTION)
                 .setUsermsg("Program terminated because of violating "
                             "an internal consistency check.");
    }
    
    
    void install_unexpectedException_handler ()
    {
      std::set_unexpected (lumiera_unexpectedException);
    }
    
    namespace {
      LifecycleHook schedule_ (ON_BASIC_INIT, &install_unexpectedException_handler);
    }
  
  
  } // namespace error

} // namespace lumiera
