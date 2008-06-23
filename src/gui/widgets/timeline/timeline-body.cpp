/*
  timeline.cpp  -  Implementation of the timeline widget
 
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

#include <cairomm-1.0/cairomm/cairomm.h>
#include <boost/foreach.hpp>

#include "timeline-body.hpp"
#include "../timeline-widget.hpp"
#include "../../window-manager.hpp"

using namespace Gtk;
using namespace std;
using namespace lumiera::gui;
using namespace lumiera::gui::widgets;
using namespace lumiera::gui::widgets::timeline;

namespace lumiera {
namespace gui {
namespace widgets {
namespace timeline {

TimelineBody::TimelineBody(lumiera::gui::widgets::TimelineWidget *timeline_widget) :
    Glib::ObjectBase("TimelineBody"),
    timelineWidget(timeline_widget)
{
  REQUIRE(timelineWidget != NULL);
      
  // Connect up some events  
  timelineWidget->horizontalAdjustment.signal_value_changed().connect(
    sigc::mem_fun(this, &TimelineBody::on_scroll) );
  timelineWidget->verticalAdjustment.signal_value_changed().connect(
    sigc::mem_fun(this, &TimelineBody::on_scroll) );
  
  // Install style properties
  gtk_widget_class_install_style_property(
    GTK_WIDGET_CLASS(G_OBJECT_GET_CLASS(gobj())), 
    g_param_spec_boxed("background",
      "Track Background",
      "The background colour of timeline tracks",
      GDK_TYPE_COLOR, G_PARAM_READABLE));

}

void
TimelineBody::on_scroll()
{
  queue_draw();
}
  
bool
TimelineBody::on_scroll_event (GdkEventScroll* event)
{
  REQUIRE(timelineWidget != NULL);
  
  if(event->state & GDK_CONTROL_MASK)
  {
    switch(event->direction)
    {
    case GDK_SCROLL_UP:
      // User scrolled up. Zoom in
      timelineWidget->zoom_view(event->x, 1);
      break;
      
    case GDK_SCROLL_DOWN:
      // User scrolled down. Zoom out
      timelineWidget->zoom_view(event->x, -1);
      break;    
    }
  }
  else
  {
    switch(event->direction)
    {
    case GDK_SCROLL_UP:
      // User scrolled up. Shift 1/16th left
      timelineWidget->shift_view(-1);
      break;
      
    case GDK_SCROLL_DOWN:
      // User scrolled down. Shift 1/16th right
      timelineWidget->shift_view(1);
      break;    
    }
  }
}
  
void
TimelineBody::on_realize()
{
  Widget::on_realize();
  
  // We wish to receive all event notifications
  add_events(Gdk::ALL_EVENTS_MASK);
}

bool
TimelineBody::on_expose_event(GdkEventExpose* event)
{
  // This is where we draw on the window
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(!window)
    return false;
  
  // Makes sure the widget styles have been loaded
  read_styles();
  
  // Prepare to render via cairo
  Glib::RefPtr<Style> style = get_style();
  Gtk::Allocation allocation = get_allocation();
  Cairo::RefPtr<Cairo::Context> cairo = window->create_cairo_context();

  // Translate the view by the scroll distance
  cairo->translate(0,
    -(int)timelineWidget->verticalAdjustment.get_value());
  
  // Interate drawing each track
  BOOST_FOREACH( Track* track, timelineWidget->tracks )
    {
      ASSERT(track != NULL);

      const int height = track->get_height();
      ASSERT(height >= 0);
    
      // Draw the track background
      cairo->rectangle(0, 0, allocation.get_width(), height);
      gdk_cairo_set_source_color(cairo->cobj(), &background);
      cairo->fill();
    
      // Render the track
      cairo->save();
      track->draw_track(cairo);
      cairo->restore();
      
      // Shift for the next track
      cairo->translate(0, height + TimelineWidget::TrackPadding);
    }   

  return true;
}
  
void
TimelineBody::read_styles()
{
  background = WindowManager::read_style_colour_property(
    *this, "background", 0, 0, 0);
}

}   // namespace timeline
}   // namespace widgets
}   // namespace gui
}   // namespace lumiera