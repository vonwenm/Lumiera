/*
  clip-track.cpp  -  Implementation of the timeline group track object
 
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
 
* *****************************************************/

#include "clip-track.hpp"
#include "timeline-view-window.hpp"

using namespace Gtk;

namespace gui {
namespace widgets {
namespace timeline {
  
ClipTrack::ClipTrack()
{
}

void
ClipTrack::draw_track(Cairo::RefPtr<Cairo::Context> cairo,
    TimelineViewWindow* const window) const
{
  cairo->set_source_rgb(0, 0, 0.5);
  cairo->rectangle(window->time_to_x(0), 10, 20, 20);
  cairo->fill();
}

}   // namespace timeline
}   // namespace widgets
}   // namespace gui
