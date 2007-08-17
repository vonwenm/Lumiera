/*
  errortest.c  -  test error handling

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

#include <error.h>
#include <stdio.h>
#include <string.h>

CINELERRA_ERROR_DEFINE(TEST, "test error");

int
main (int argc, char** argv)
{
  if (argc == 1)
    return 0;

  if (!strcmp(argv[1], "set"))
    {
      cinelerra_error_set (CINELERRA_ERROR_TEST);
    }

  if (!strcmp(argv[1], "get_no"))
    {
      const char* err;
      err = cinelerra_error ();
      printf ("%p\n", err);
    }

  if (!strcmp(argv[1], "get"))
    {
      cinelerra_error_set (CINELERRA_ERROR_TEST);
      const char* err;
      err = cinelerra_error ();
      printf ("%s\n", err);
    }

  if (!strcmp(argv[1], "get2"))
    {
      cinelerra_error_set (CINELERRA_ERROR_TEST);
      const char* err;
      err = cinelerra_error ();
      printf ("%s\n", err);
      err = cinelerra_error ();
      printf ("%p\n", err);
    }

  return 0;
}
