/*
  TEST.h  -  support macros for plain-C tests

  Copyright (C)         Lumiera.org
    2008, 2010          Christian Thaeter <ct@pipapo.org>

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

*/

/** @file test.h
 ** Helpers and support macros for defining test executables in C.
 ** These macros provide some building blocks to assemble a \c main() function,
 ** which checks a test name parameter and invokes the matching embedded code block.
 ** 
 ** @see test-mpool.c    C   test example
 ** @see HelloWorld_test C++ test example
 */


#ifndef LIB_TEST_TEST_H
#define LIB_TEST_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nobug.h>



#define TESTS_BEGIN                             \
int                                             \
main (int argc, const char** argv)              \
{                                               \
  NOBUG_INIT;                                   \
  unsigned testcnt=0;                           \
  int ret = 0;                                  \
                                                \
  if (argc == 1)                                \
    fprintf (stderr, "supported tests:\n");

#define TEST(name)                              \
  if (argc == 1)                                \
    fprintf (stderr, "  "#name"\n");            \
  else if (!strcmp(argv[1], #name) && ++testcnt)

#define PLANNED_TEST(name)                      \
  if (argc == 1)                                \
    fprintf (stderr, "  "#name" (planned)\n");  \
  else if (!++testcnt)

#define TESTS_END                                   \
  if (!testcnt && argc !=1)                         \
    fprintf (stderr,"no such test: %s\n", argv[1]); \
  return ret;                                       \
}


#endif

/*
// Local Variables:
// mode: C
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
*/
