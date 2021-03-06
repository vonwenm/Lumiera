/*
  GTK-BASE.hpp  -  GTK includes and basic definitions

  Copyright (C)         Lumiera.org
    2011,               Hermann Vosseler <Ichthyostega@web.de>
    2008,               Joel Holdsworth <joel@airwebreathe.org.uk>

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


/** @file gtk-base.hpp
 ** A set of basic GTK includes.
 ** There are some tricky point to consider when including the
 ** basic GTKmm headers. Especially, GTK tries to shadow the ERROR macro
 ** from Microsoft Windows. Unfortunately this breaks the ERROR macro from NoBug;
 ** thus we need to include NoBug \em after GTK
 ** 
 ** Besides, this header defines the basic NLS. Most parts of the GUI rely either
 ** directly on this header, or through the inclusion of gtk-lumiera.hpp
 ** 
 ** @see GtkLumiera
 */


#ifndef GUI_GTK_BASE_H
#define GUI_GTK_BASE_H

//--------------------tricky special Include sequence
#include "lib/hash-standard.hpp"
#include <locale>
#include <gtkmm.h>
#include <nobug.h>
//--------------------tricky special Include sequence

#include "lib/error.hpp"
#include "lib/util.hpp"


#ifdef ENABLE_NLS
#  include <libintl.h>
#  define _(String) gettext (String)
#  define gettext_noop(String) String
#  define N_(String) gettext_noop (String)
#else
#  define _(String) (String)
#  define N_(String) String
#  define textdomain(Domain)
#  define bindtextdomain(Package, Directory)
#endif




/* ======= Namespace Definitions ======= */

/** Lumiera GTK GUI implementation root. */
namespace gui {
  
  typedef Glib::ustring uString;
  typedef const uString cuString;
  

  /** Dialog box classes. */
  namespace dialogs {}
  
  /** The Lumiera GTK-GUI uses a thin proxy layer data model
   *  on top of the actual "high-level-model", which lives in the
   *  Proc-Layer below. GUI operations interact with these proxy model
   *  entities, which in turn forward the calls to the actual objects
   *  in the Proc-Layer, through the Command system (which enables UNDO).
   *  
   *  @todo: as of 1/2011 this connection between the GUI proxy model and
   *         the Proc-Layer model needs to be set up. Currently, the GUI model
   *         entities are just created standalone and thus dysfunctional.
   */
  namespace model {}
  
  /** The namespace of all video output implementations. */
  namespace output {}
  
  /** Docking panel classes. */
  namespace panels {}
  
  /** Lumiera custom widgets. */
  namespace widgets {}
  
  /** The workspace window and it's helper classes. */
  namespace workspace {}
  
  /** GUI helpers, utility functions and classes. */
  namespace util {}
  
}// namespace gui
#endif
