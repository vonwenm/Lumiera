/*
  timeline-track.cpp  -  Implementation of the timeline track object

  Copyright (C)         Lumiera.org
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

* *****************************************************/

#include "timeline-track.hpp"
#include "../timeline-widget.hpp"
#include "../../window-manager.hpp"
#include "../../dialogs/name-chooser.hpp"
#include "include/logging.h"

using namespace boost;
using namespace Gtk;
using namespace sigc;

namespace gui {
namespace widgets {
namespace timeline {

const float Track::ExpandAnimationPeriod = 0.15;
  
Track::Track(TimelineWidget &timeline_widget,
  shared_ptr<model::Track> track) :
  timelineWidget(timeline_widget),
  model_track(track),
  enabled(true),
  expanded(true),
  expandDirection(None),
  locked(false),
  headerWidget(*this),
  enableButton(Gtk::StockID("track_enabled"), WindowManager::MenuIconSize),
  lockButton(Gtk::StockID("track_unlocked"), WindowManager::MenuIconSize)
{
  REQUIRE(model_track);
  
  titleMenuButton.set_relief(RELIEF_HALF);
  titleMenuButton.unset_flags(CAN_FOCUS);
    
  buttonBar.append(enableButton, mem_fun(this, &Track::on_enable));
  buttonBar.append(lockButton, mem_fun(this, &Track::on_lock));
  
  headerWidget.set_child_widget(headerBox);

  headerBox.pack_start(titleMenuButton, PACK_SHRINK);
  headerBox.pack_start(buttonBar, PACK_SHRINK);
  
  headerWidget.show_all();
  
  // Setup the title menu button
  Menu::MenuList& title_list = titleMenuButton.get_menu().items();
  title_list.push_back( Menu_Helpers::MenuElem(_("_Name..."),
    mem_fun(this, &Track::on_set_name) ) );
  title_list.push_back( Menu_Helpers::SeparatorElem() );
  title_list.push_back( Menu_Helpers::MenuElem(_("_Remove"),
    mem_fun(this, &Track::on_remove_track) ) );
    
  update_name();
  
  // Setup tooltips
  enableButton.set_tooltip_text(_("Disable track"));
  lockButton.set_tooltip_text(_("Lock track"));

  // Setup the context menu
  Menu::MenuList& context_list = contextMenu.items();
  //context_list.push_back( Menu_Helpers::MenuElem(_("_Add Track"),
  //  mem_fun(timelineWidget, &TimelineWidget::on_add_track_command) ) );
  context_list.push_back( Menu_Helpers::MenuElem(_("_Remove Track"),
    mem_fun(this, &Track::on_remove_track) ) );
    
  // Connect to the model
  model_track->signal_name_changed().connect(sigc::mem_fun(this,
    &Track::on_name_changed));
    
}

Track::~Track()
{
  headerWidget.remove(headerBox);
}

Gtk::Widget&
Track::get_header_widget()
{
  return headerWidget;
}

shared_ptr<model::Track>
Track::get_model_track() const
{
  return model_track;
}

int
Track::get_height() const
{
  return 100;
}

bool
Track::get_expanded() const
{
  return expanded;
}

void
Track::expand_collapse(ExpandDirection direction)
{
  REQUIRE(direction != None);
  
  expandDirection = direction;
  if(direction == Expand)
    {
      expanded = true;
      expandAnimationState = 0.0;
    }
  else
    {
      expanded = false;
      expandAnimationState = 1.0;
    }
  
  // Create a timer if we don't already have one
  if(!expand_timer)
    {
      expand_timer.reset(new Glib::Timer());
      expand_timer->start();
    }
  else  // Reset the timer if we do
    expand_timer->reset();
}

float
Track::get_expand_animation_state() const
{
  ENSURE(expandAnimationState >= 0.0 &&
    expandAnimationState <= 1.0);
  return expandAnimationState;
}

bool
Track::is_expand_animating() const
{
  return expandDirection != None;
}
  
void
Track::tick_expand_animation()
{
  REQUIRE(expandDirection != None); // tick_expand_animation should not
                                    // be unless is_expand_animating
                                    // returns true
  REQUIRE(expand_timer);
  const float delta = 
    (float)expand_timer->elapsed() / ExpandAnimationPeriod;
  expand_timer->reset(); // reset the timer to t=0

  if(expandDirection == Expand)
    {
      expandAnimationState += delta;
      if(expandAnimationState >= 1.0)
        expandDirection = None;
    }
  else
    {
      expandAnimationState -= delta;
      if(expandAnimationState <= 0.0)
        expandDirection = None;
    }
    
  if(expandDirection == None)
    expand_timer.reset();  // We've finished with the timer - delete it
}

Gtk::ExpanderStyle
Track::get_expander_style() const
{
  if(expanded)
    {
      if(expandDirection == None)
        return EXPANDER_EXPANDED;
      else if(expandAnimationState >= 2.0 / 3.0)
        return EXPANDER_SEMI_EXPANDED;
      else if(expandAnimationState >= 1.0 / 3.0)
        return EXPANDER_SEMI_COLLAPSED;
      else
        return EXPANDER_COLLAPSED;
    }
  else
    {
      if(expandDirection == None)
        return EXPANDER_COLLAPSED;
      else if(expandAnimationState >= 2.0 / 3.0)
        return EXPANDER_EXPANDED;
      else if(expandAnimationState >= 1.0 / 3.0)
        return EXPANDER_SEMI_EXPANDED;
      else
        return EXPANDER_SEMI_COLLAPSED;
    }
    
  NOBUG_ERROR(gui, "Track::get_expander_style() final return reached");

  return EXPANDER_COLLAPSED;   // This should never happen
}

void
Track::show_header_context_menu(guint button, guint32 time)
{
  contextMenu.popup(button, time);
}

void
Track::update_name()
{
  REQUIRE(model_track);
  titleMenuButton.set_label(model_track->get_name());
}

void
Track::on_enable()
{
  enabled = !enabled;
  if (enabled)
    {
      enableButton.set_stock_id(Gtk::StockID("track_enabled"), WindowManager::MenuIconSize);
      enableButton.set_tooltip_text(_("Disable track"));
    }
  else
    {
      enableButton.set_stock_id(Gtk::StockID("track_disabled"), WindowManager::MenuIconSize);
      enableButton.set_tooltip_text(_("Enable track"));
    }
}

void
Track::on_lock()
{
  locked = !locked;
  if (locked)
    {
      lockButton.set_stock_id(Gtk::StockID("track_locked"), WindowManager::MenuIconSize);
      lockButton.set_tooltip_text(_("Unlock track"));
    }
  else
    {
      lockButton.set_stock_id(Gtk::StockID("track_unlocked"), WindowManager::MenuIconSize);
      lockButton.set_tooltip_text(_("Lock track"));
    }
}

void
Track::on_set_name()
{
  REQUIRE(model_track);
  
  Gtk::Window *window = dynamic_cast<Window*>(
    timelineWidget.get_toplevel());
  REQUIRE(window != NULL); 
    
  dialogs::NameChooser dialog(*window,
    _("Set Track Name"), model_track->get_name());
    
  if(dialog.run() == RESPONSE_OK)
    model_track->set_name(dialog.get_name());
}

void
Track::on_name_changed(std::string)
{
  update_name();
}

void
Track::on_remove_track()
{
  REQUIRE(model_track);
  boost::shared_ptr<TimelineState> state = timelineWidget.get_state();
  REQUIRE(state);
  
  state->get_sequence()->remove_descendant_track(model_track);
}

}   // namespace timeline
}   // namespace widgets
}   // namespace gui

