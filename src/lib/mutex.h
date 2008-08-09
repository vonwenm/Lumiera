/*
  mutex.h  -  mutal exclusion locking

  Copyright (C)         Lumiera.org
    2008,               Christian Thaeter <ct@pipapo.org>

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

#ifndef LUMIERA_MUTEX_H
#define LUMIERA_MUTEX_H

#include "lib/locking.h"

/**
 * @file
 * Mutual exclusion locking, header.
 */


/**
 * Mutual exclusive section.
 */
#define LUMIERA_MUTEX_SECTION(nobugflag, mtx)                                                                                   \
  for (lumiera_mutexacquirer NOBUG_CLEANUP(lumiera_mutexacquirer_ensureunlocked) lumiera_mutex_section_ = {(LumieraMutex)1};    \
       lumiera_mutex_section_.mutex;)                                                                                           \
    for (                                                                                                                       \
         ({                                                                                                                     \
           lumiera_mutex_section_.mutex = (mtx);                                                                                \
           NOBUG_RESOURCE_HANDLE_INIT (lumiera_mutex_section_.rh);                                                              \
           RESOURCE_ENTER (nobugflag, (mtx)->rh, "acquire mutex", &lumiera_mutex_section_,                                      \
                           NOBUG_RESOURCE_EXCLUSIVE, lumiera_mutex_section_.rh);                                                \
           if (pthread_mutex_lock (&(mtx)->mutex)) LUMIERA_DIE (MUTEX_LOCK);                                                    \
         });                                                                                                                    \
         lumiera_mutex_section_.mutex;                                                                                          \
         ({                                                                                                                     \
           if (lumiera_mutex_section_.mutex)                                                                                    \
             {                                                                                                                  \
               pthread_mutex_unlock (&lumiera_mutex_section_.mutex->mutex);                                                     \
               lumiera_mutex_section_.mutex = NULL;                                                                             \
               RESOURCE_LEAVE(nobugflag, lumiera_mutex_section_.rh);                                                            \
             }                                                                                                                  \
         }))


/**
 * Mutex.
 *
 */
struct lumiera_mutex_struct
{
  pthread_mutex_t mutex;
  RESOURCE_HANDLE (rh);
};
typedef struct lumiera_mutex_struct lumiera_mutex;
typedef lumiera_mutex* LumieraMutex;


/**
 * Initialize a mutex variable
 * @param self is a pointer to the mutex to be initialized
 * @return self as given
 */
LumieraMutex
lumiera_mutex_init (LumieraMutex self);


/**
 * Destroy a mutex variable
 * @param self is a pointer to the mutex to be destroyed
 * @return self as given
 */
LumieraMutex
lumiera_mutex_destroy (LumieraMutex self);


/**
 * mutexacquirer used to manage the state of a mutex.
 */
struct lumiera_mutexacquirer_struct
{
  volatile LumieraMutex mutex;
  RESOURCE_HANDLE (rh);
};
typedef struct lumiera_mutexacquirer_struct lumiera_mutexacquirer;
typedef struct lumiera_mutexacquirer_struct* LumieraMutexacquirer;

/* helper function for nobug */
static inline void
lumiera_mutexacquirer_ensureunlocked (LumieraMutexacquirer self)
{
  ENSURE (!self->mutex, "forgot to unlock mutex");
}

#endif
