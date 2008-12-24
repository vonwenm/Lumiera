/*
  CONCURRENCY.hpp  -  generic helper for object based locking and synchronisation
 
  Copyright (C)         Lumiera.org
    2008,               Christian Thaeter <ct@pipapo.org>
                        Hermann Vosseler <Ichthyostega@web.de>
 
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

/** @file sync.hpp
 ** Collection of helpers and wrappers to support dealing with concurrency issues.
 ** Actually, everything is implemented either by the Lumiera backend, or directly
 ** by pthread. The purpose is to support and automate the most common use cases
 ** in object oriented style.
 **
 ** @see mutex.h
 ** @see sync-locking-test.cpp
 ** @see sync-waiting-test.cpp
 ** @see asset::AssetManager::reg() usage example
 ** @see subsystemrunner.hpp usage example
 */


#ifndef LIB_SYNC_H
#define LIB_SYNC_H

#include "include/nobugcfg.h"
#include "lib/util.hpp"
#include "include/error.hpp"

extern "C" {
#include "lib/mutex.h"
#include "lib/condition.h"
}

#include <boost/scoped_ptr.hpp>
#include <cerrno>
#include <ctime>


namespace lib {

    using boost::scoped_ptr;
    
    
    /** Helpers and building blocks for Monitor based synchronisation */
    namespace sync {
      
      class Timeout;
    
      class RecMutex 
        {
          lumiera_mutex mtx_;
          
          pthread_mutex_t* get () { return &mtx_.mutex; }
          friend class Condition;
          
        public:
          RecMutex()  { lumiera_recmutex_init (&mtx_, "Obj.Monitor RecMutex", &NOBUG_FLAG(sync)); }
          ~RecMutex() { lumiera_mutex_destroy (&mtx_, &NOBUG_FLAG(sync)); }
          
            void 
            acquire() 
              {
                TODO ("Record we may block on mutex");
                
                 if (pthread_mutex_lock (get()))
                   throw lumiera::error::State("Mutex acquire failed.");  ///////TODO capture the error-code
  
                TODO ("Record we successfully acquired the mutex");
              }
            
            void 
            release() 
              { 
                TODO ("Record we are releasing the mutex");
                pthread_mutex_unlock (get());
              }
            
        };
      
      
      class Condition
        {
          lumiera_condition cond_;
          
        public:
          Condition()   { lumiera_condition_init    (&cond_, "Obj.Monitor Condition", &NOBUG_FLAG(sync) ); }
          ~Condition()  { lumiera_condition_destroy (&cond_, &NOBUG_FLAG(sync) ); }
          
          void
          signal (bool wakeAll=false) 
            {
              if (wakeAll)
                  pthread_cond_broadcast (&cond_.cond);
              else
                  pthread_cond_signal (&cond_.cond);
            }
          
          
          template<class BF, class MTX>
          bool
          wait (BF& predicate, MTX& mtx, Timeout& waitEndTime)
            {
              int err=0;
              while (!predicate() && !err)
                if (waitEndTime)
                  err = pthread_cond_timedwait (&cond_.cond, mtx.get(), &waitEndTime);
                else
                  err = pthread_cond_wait (&cond_.cond, mtx.get());
              
              if (!err)           return true;
              if (ETIMEDOUT==err) return false;
              
              throw lumiera::error::State ("Condition wait failed."); ///////////TODO extract error-code
            }
        };
      
      /** helper for specifying an optional timeout
       *  for an timed wait. It wraps a timespec-struct
       *  and allows for easy initialisation by a given
       *  relative offset.
       */
      struct Timeout
        : timespec
        {
          Timeout() { tv_sec=tv_nsec=0; }
          
          /** initialise to NOW() + offset (in milliseconds) */
          Timeout&
          setOffset (ulong offs)
            {
              if (offs)
                {
                  clock_gettime(CLOCK_REALTIME, this);
                  tv_sec   += offs / 1000;
                  tv_nsec  += 1000000 * (offs % 1000);
                  if (tv_nsec > 1000000000)
                    {
                      tv_sec += tv_nsec / 1000000000;
                      tv_nsec %= 1000000000;
                }   }
              return *this;
            }
          
          operator bool() { return 0 != tv_sec; } // allows if (timeout_)....
        };
      
      
      typedef volatile bool& Flag;
      
      class Monitor
        {
          RecMutex mtx_;
          Condition cond_;
          
          Timeout timeout_;
          
          //////TODO my intention is to make two variants of the monitor, where the simple one leaves out the condition part
          
        public:
          Monitor() {}
          ~Monitor() {}
          
          void acquireLock() { mtx_.acquire(); }
          void releaseLock() { mtx_.release(); }
          
          void signal(bool a){ cond_.signal(a);}
          
          inline bool wait (Flag, ulong);
          inline void setTimeout(ulong);
          inline bool isTimedWait();
        };
     
      
      
      struct BoolFlagPredicate
        {
          Flag flag_;
          BoolFlagPredicate (Flag f) : flag_(f) {}
         
          bool operator() () { return flag_; }
        };
      
     
      bool
      Monitor::wait (Flag flag, ulong timedwait)
      {
        BoolFlagPredicate checkFlag(flag);
        return cond_.wait(checkFlag, mtx_, timeout_.setOffset(timedwait));                                   
      }
      
      void
      Monitor::setTimeout (ulong relative) {timeout_.setOffset(relative);}
      
      bool
      Monitor::isTimedWait () {return (timeout_);}
      
      
    } // namespace sync
    
    
    
    /**
     * Facility for monitor object based locking. 
     * To be attached either on a per class base or per object base.
     * Typically, the client class will inherit from this template (but it
     * is possible to use it stand-alone, if inheriting isn't an option).
     * The interface for clients to access the functionality is the embedded
     * Lock template, which should be instantiated as an automatic variable
     * within the scope to be protected.
     * 
     * @todo actually implement this facility using the Lumiera backend.
     */
    struct Sync
      {
        typedef sync::Monitor Monitor;
        
        Monitor objectMonitor_;
        
        /////////////////////////////////////////////////////////////////////////TODO: factor out the recursive/non-recursive mutex case as policy...
        
        template<class X>
        static inline Monitor& getMonitor();
        
        static inline Monitor& getMonitor(Sync* forThis);
        
        
        class Lock
          {
            Monitor& mon_;
            
          public:
            template<class X>
            Lock(X* it) : mon_(getMonitor(it)){ mon_.acquireLock(); }
            Lock(Monitor& m) : mon_(m)        { mon_.acquireLock(); }
            ~Lock()                           { mon_.releaseLock(); }
            
            template<typename C>
            bool wait (C& cond, ulong time=0) { return mon_.wait(cond,time);}
            void setTimeout(ulong time)       { mon_.setTimeout(time); }
            
            void notifyAll()                  { mon_.signal(true); }
            void notify()                     { mon_.signal(false);}
            
          };
        
        
        template<class X>
        struct ClassLock : Lock
          {
            ClassLock() : Lock (getMonitor<X>()) {}
          };
        
      };
    
      
    
    
    
    Sync::Monitor&
    Sync::getMonitor(Sync* forThis)
    {
      REQUIRE (forThis);
      return forThis->objectMonitor_;
    }
    
    template<class X>
    Sync::Monitor&
    Sync::getMonitor()
    {
      //TODO: a rather obscure race condition is hidden here:
      //TODO: depending on the build order, the dtor of this static variable may be called, while another thread is still holding an ClassLock.
      //TODO: An possible solution would be to use an shared_ptr to the Monitor in case of a ClassLock and to protect access with another Mutex.
      //TODO. But I am really questioning if we can't ignore this case and state: "don't hold a ClassLock when your code maybe still running in shutdown phase!"
      //TODO: probably best Idea is to detect this situation in DEBUG or ALPHA mode
      
      static scoped_ptr<Monitor> classMonitor_ (0);
      if (!classMonitor_) classMonitor_.reset (new Monitor ());
      return *classMonitor_;
    }
    
  
} // namespace lumiera
#endif
