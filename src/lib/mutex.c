/*
  mutex.c  -  mutex

  Copyright (C)         Lumiera.org
    2008, 2009,         Christian Thaeter <ct@pipapo.org>

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

#include "lib/mutex.h"

/**
 * @file
 * Mutual exclusion locking.
 */

LUMIERA_ERROR_DEFINE (LOCK_ACQUIRE, "locking failed");
LUMIERA_ERROR_DEFINE (LOCK_RELEASE, "unlocking failed");
LUMIERA_ERROR_DEFINE (LOCK_DESTROY, "lock destroy failed");


LumieraMutex
lumiera_mutex_init (LumieraMutex self, const char* purpose, struct nobug_flag* flag)
{
  if (self)
    {
      pthread_mutex_init (&self->mutex, NULL);
      NOBUG_RESOURCE_HANDLE_INIT (self->rh);
      NOBUG_RESOURCE_ANNOUNCE_RAW (flag, "mutex", purpose, self, self->rh);
    }
  return self;
}


LumieraMutex
lumiera_mutex_destroy (LumieraMutex self, struct nobug_flag* flag)
{
  if (self)
    {
      NOBUG_RESOURCE_FORGET_RAW (flag,  self->rh);
      if (pthread_mutex_destroy (&self->mutex))
        LUMIERA_DIE (LOCK_DESTROY);
    }
  return self;
}

int lumiera_mutex_unlock_cb (void* mutex)
{
  return pthread_mutex_unlock (&((LumieraMutex)mutex)->mutex);
}

/*
// Local Variables:
// mode: C
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
*/
