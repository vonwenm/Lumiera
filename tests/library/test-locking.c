/*
  test-locking.c  -  test locking functions

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

#include "tests/test.h"
#include "lib/mutex.h"
#include "lib/condition.h"
#include "lib/rwlock.h"

#include <stdio.h>
#include <string.h>

TESTS_BEGIN


TEST ("mutexsection")
{
  lumiera_mutex m;
  lumiera_mutex_init (&m, "mutexsection", &NOBUG_FLAG(NOBUG_ON));

  LUMIERA_MUTEX_SECTION (NOBUG_ON, &m)
    {
      printf ("mutex locked section 1\n");
    }

  LUMIERA_MUTEX_SECTION (NOBUG_ON, &m)
    {
      printf ("mutex locked section 2\n");
    }

  lumiera_mutex_destroy (&m, &NOBUG_FLAG(NOBUG_ON));
}


TEST ("mutexforgotunlock")
{
  lumiera_mutex m;
  lumiera_mutex_init (&m, "mutexforgotunlock", &NOBUG_FLAG(NOBUG_ON));

  LUMIERA_MUTEX_SECTION (NOBUG_ON, &m)
    {
      break;    // MUTEX_SECTIONS must not be left by a jump
    }

  lumiera_mutex_destroy (&m, &NOBUG_FLAG(NOBUG_ON));
}


TEST ("nestedmutexsection")
{
  lumiera_mutex m;
  lumiera_mutex_init (&m, "m_mutexsection", &NOBUG_FLAG(NOBUG_ON));

  lumiera_mutex n;
  lumiera_mutex_init (&n, "n_mutexsection", &NOBUG_FLAG(NOBUG_ON));

  LUMIERA_MUTEX_SECTION (NOBUG_ON, &m)
    {
      printf ("outer mutex locked section\n");

      LUMIERA_MUTEX_SECTION (NOBUG_ON, &n)
        {
          printf ("inner mutex locked section\n");
        }
    }

  lumiera_mutex_destroy (&n, &NOBUG_FLAG(NOBUG_ON));
  lumiera_mutex_destroy (&m, &NOBUG_FLAG(NOBUG_ON));
}

TEST ("chainedmutexsection")
{
  lumiera_mutex m;
  lumiera_mutex_init (&m, "m_mutexsection", &NOBUG_FLAG(NOBUG_ON));

  lumiera_mutex n;
  lumiera_mutex_init (&n, "n_mutexsection", &NOBUG_FLAG(NOBUG_ON));

  LUMIERA_MUTEX_SECTION (NOBUG_ON, &m)
    {
      printf ("outer mutex locked section\n");

      LUMIERA_MUTEX_SECTION_CHAIN (NOBUG_ON, &n)
        {
          printf ("inner but not outer mutex locked section\n");
        }
    }

  lumiera_mutex_destroy (&n, &NOBUG_FLAG(NOBUG_ON));
  lumiera_mutex_destroy (&m, &NOBUG_FLAG(NOBUG_ON));
}

TEST ("recursivemutexsection")
{
  lumiera_mutex m;
  lumiera_recmutex_init (&m, "m_mutexsection", &NOBUG_FLAG(NOBUG_ON));


  LUMIERA_MUTEX_SECTION (NOBUG_ON, &m)
    {
      printf ("mutex locked once\n");

      LUMIERA_MUTEX_SECTION (NOBUG_ON, &m)
        {
          printf ("mutex locked twice\n");
        }
    }

 lumiera_mutex_destroy (&m, &NOBUG_FLAG(NOBUG_ON));
}

TEST ("rwlocksection")
{
  lumiera_rwlock rwlock;
  lumiera_rwlock_init (&rwlock, "rwsection", &NOBUG_FLAG(NOBUG_ON));

  LUMIERA_WRLOCK_SECTION (NOBUG_ON, &rwlock)
    {
      printf ("write locked section 1\n");
    }

  LUMIERA_RDLOCK_SECTION (NOBUG_ON, &rwlock)
    {
      printf ("read locked section 2\n");
    }

  lumiera_rwlock_destroy (&rwlock, &NOBUG_FLAG(NOBUG_ON));
}

TEST ("rwlockforgotunlock")
{
  lumiera_rwlock rwlock;
  lumiera_rwlock_init (&rwlock, "rwlockforgotunlock", &NOBUG_FLAG(NOBUG_ON));

  LUMIERA_RDLOCK_SECTION (NOBUG_ON, &rwlock)
    {
      break;    // LOCK_SECTIONS must not be left by a jump
    }

  lumiera_rwlock_destroy (&rwlock, &NOBUG_FLAG(NOBUG_ON));
}


TEST ("conditionsection")
{
  lumiera_condition cond;
  lumiera_condition_init (&cond, "conditionsection", &NOBUG_FLAG(NOBUG_ON));

  LUMIERA_CONDITION_SECTION (NOBUG_ON, &cond)
    {
      printf ("condition locked section 1\n");
    }

  LUMIERA_CONDITION_SECTION (NOBUG_ON, &cond)
    {
      printf ("condition locked section 2\n");
    }

  lumiera_condition_destroy (&cond, &NOBUG_FLAG(NOBUG_ON));
}


TEST ("conditionforgotunlock")
{
  lumiera_condition cond;
  lumiera_condition_init (&cond, "conditionforgotunlock", &NOBUG_FLAG(NOBUG_ON));

  LUMIERA_CONDITION_SECTION (NOBUG_ON, &cond)
    {
      break;    // CONDITION_SECTIONS must not be left by a jump
    }

  lumiera_condition_destroy (&cond, &NOBUG_FLAG(NOBUG_ON));
}

TESTS_END