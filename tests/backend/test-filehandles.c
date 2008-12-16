/*
  test-filehandles.c  -  test filehandle management

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
#include "lib/llist.h"
#include "lib/safeclib.h"

#include "backend/config.h"
#include "backend/backend.h"
#include "backend/filehandlecache.h"

#include "tests/test.h"

TESTS_BEGIN

TEST ("basic")
{
  lumiera_config_init ("./");
  lumiera_backend_init ();
  LumieraFile file = lumiera_file_new (",tmp_testfile", LUMIERA_FILE_CREATE);

  /* get the filehandle */
  int fd = lumiera_file_handle_acquire (file);

  /* we now 'own'it and can use it */
  ENSURE (fd > -1);
  printf ("got filehandle #%d\n", fd);

  /* put it into aging, can't use anymore */
  lumiera_file_handle_release (file);

  lumiera_file_delete (file);
  lumiera_backend_destroy ();
  lumiera_config_destroy ();
}


TEST ("more")
{
  lumiera_config_init ("./");
  lumiera_backend_init ();

  LumieraFile files[100];
  int fds[100];

  /*create 100 files*/
  for (int i=0; i<100; ++i)
    {
      files[i]= lumiera_file_new (lumiera_tmpbuf_snprintf (256, ",tmpdir/testfile%d", i), LUMIERA_FILE_CREATE);
    }

  /* get the filehandles, this gross overallocates filehandles */
  for (int i=0; i<100; ++i)
    {
      fds[i] = lumiera_file_handle_acquire (files[i]);
      ENSURE (fds[i] > -1);
      printf ("got filehandle #%d\n", fds[i]);
    }

  /* put them into aging, can't use anymore */
  for (int i=0; i<100; ++i)
    {
      lumiera_file_handle_release (files[i]);
    }

  /* cleanup */
  for (int i=0; i<100; ++i)
    {
      lumiera_file_delete (files[i]);
    }

  lumiera_backend_destroy ();
  lumiera_config_destroy ();
}

TESTS_END
