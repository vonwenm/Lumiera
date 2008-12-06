/*
  timeline-track.hpp  -  Declaration of the timeline group track object
 
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
/** @file widgets/timeline/timeline-track.hpp
 ** This file contains the definition of timeline track object
 */

#include "../../gtk-lumiera.hpp"
#include "../../model/track.hpp"

#ifndef TIMELINE_TRACK_HPP
#define TIMELINE_TRACK_HPP

namespace gui {
namespace widgets { 
namespace timeline {
  
class TimelineViewWindow;

class Track
{
public:
  Track();
  
  Gtk::Widget& get_header_widget();
  
  int get_height() const;
  
  bool get_expanded() const;
  
  void set_expanded(bool expanded);

  virtual void draw_track(Cairo::RefPtr<Cairo::Context> cairo,
    TimelineViewWindow* const window)
    const = 0;

private:

  bool expanded;

  //----- Header Widgets ------//
  
  Gtk::VBox headerWidget;
  
  Gtk::ToolButton enableButton;
  Gtk::ToolButton lockButton;
  
  Gtk::Entry titleBox;
  Gtk::Toolbar buttonBar;
};


}   // namespace timeline
}   // namespace widgets
}   // namespace gui

#endif // TIMELINE_TRACK_HPP