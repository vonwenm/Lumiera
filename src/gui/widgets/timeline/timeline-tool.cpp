/*
  timeline-tool.hpp  -  Implementation of the Tool class
 
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

#include "timeline-tool.hpp"
#include "../timeline-widget.hpp"

using namespace Gdk;

namespace gui {
namespace widgets {
namespace timeline {

Tool::Tool(TimelineBody *timeline_body) :
  timelineBody(timeline_body),
  isDragging(false)
{
  REQUIRE(timeline_body != NULL);
}

bool
Tool::apply_cursor()
{ 
  REQUIRE(timelineBody != NULL);
    
  Glib::RefPtr<Window> window = 
    timelineBody->get_window();
  if(!window)
    return false;
  
  window->set_cursor(get_cursor());

  return true;
}

void
Tool::on_button_press_event(GdkEventButton* event)
{
  REQUIRE(event != NULL);
  
  if(event->button == 1)
    isDragging = true;
}

void
Tool::on_button_release_event(GdkEventButton* event)
{
  REQUIRE(event != NULL);
  
  if(event->button == 1)
    isDragging = false;
}

void
Tool::on_motion_notify_event(GdkEventMotion *event)
{
  mousePoint = Point(event->x, event->y);
}

gui::widgets::TimelineWidget*
Tool::get_timeline_widget() const
{
  REQUIRE(timelineBody != NULL);
  gui::widgets::TimelineWidget *timeline_widget =
    timelineBody->timelineWidget;
  REQUIRE(timeline_widget != NULL);
  return timeline_widget;
}

Gdk::Rectangle
Tool::get_body_rectangle() const
{
  REQUIRE(timelineBody != NULL);
  return timelineBody->get_allocation();
}

}   // namespace timeline
}   // namespace widgets
}   // namespace gui