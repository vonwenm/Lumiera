/*
  track.hpp  -  Declaration of the timeline track object
 
  Copyright (C)         Lumiera.org
    2008,               Joel Holdsworth <joel@airwebreathe.org.uk>
 
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
/** @file widgets/timeline/track.hpp
 ** This file contains the definition of timeline track object
 */

#ifndef TRACK_HPP
#define TRACK_HPP

#include "../../gtk-lumiera.hpp"

namespace gui {
namespace widgets {
namespace timeline {

class Track
{
public:
  Track();

  Glib::ustring get_title();

  Gtk::Widget& get_header_widget();

  int get_height();

  void draw_track(Cairo::RefPtr<Cairo::Context> cairo);

protected:
  Gtk::VBox headerWidget;
  Gtk::Label label1;
  Gtk::Label label2;
  Gtk::CheckButton label3;
  Gtk::Button label4;
};


}   // namespace timeline
}   // namespace widgets
}   // namespace gui

#endif // TRACK_HPP
