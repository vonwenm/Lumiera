/*
  mmap.h  -  memory mapped acces to files

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

#ifndef LUMIERA_MMAP_H
#define LUMIERA_MMAP_H

#include "lib/llist.h"


typedef struct lumiera_mmap_struct lumiera_mmap;
typedef lumiera_mmap* LumieraMMap;


#include "backend/file.h"
#include "backend/filedescriptor.h"


#include <nobug.h>
#include <sys/mman.h>

NOBUG_DECLARE_FLAG (mmap);


/**
 * @file
 * MMap objects cover a memory maped range in a file
 */



/**
 * mmaped area
 *
 */
struct lumiera_mmap_struct
{
  /** used for the mrucache when checked in the cache OR for attaching owners when checked out **/
  llist cachenode;

  /** all mmaps of a filedescriptor are chained in this list, used to find ranges **/
  llist searchnode;

  off_t start;
  size_t size;
  void* address;

  /** accumulated references, this is 0 when checked into the cache **/
  unsigned long refcnt;

  /** array with refcounters per chunk **/
  unsigned short* refmap;       // TODO flexible array?
};


LumieraMMap
lumiera_mmap_init (LumieraMMap self, LumieraFile file, off_t start, size_t size);


LumieraMMap
lumiera_mmap_new (LumieraFile file, off_t start, size_t size);


static inline void*
lumiera_mmap_address (LumieraMMap self)
{
  return self->address;
}


void
lumiera_mmap_delete (LumieraMMap self);


void*
lumiera_mmap_destroy_node (LList node);


#endif
/*
// Local Variables:
// mode: C
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
*/
