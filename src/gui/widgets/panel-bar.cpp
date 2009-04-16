/*
  panel-bar.hpp  -  Declaration of the panel bar widget
 
  Copyright (C)         Lumiera.org
    2009,               Joel Holdsworth <joel@airwebreathe.org.uk>
 
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

#include "panel-bar.hpp"
#include "../workspace/workspace-window.hpp"
#include "../workspace/panel-manager.hpp"
#include "../panels/panel.hpp"
#include "../util/rectangle.hpp"

#include <nobug.h>
#include <algorithm>

using namespace Gtk;
using namespace Glib;
using namespace sigc;
using namespace std;
using namespace gui::workspace;

namespace gui {
namespace widgets {

PanelBar::PanelBar(panels::Panel &owner_panel, const gchar *stock_id) :
  Box(),
  panel(owner_panel),
  panelButton(StockID(stock_id)),
  lockItem(NULL)
{
  set_border_width(1);
  
  panelButton.set_relief(RELIEF_NONE);
  panelButton.unset_flags(CAN_FOCUS);
  panelButton.show();
  pack_start(panelButton, PACK_SHRINK);
  
  setup_panel_button();
}

void
PanelBar::setup_panel_button()
{
  REQUIRE(lockItem == NULL);
  
  Menu& menu = panelButton.get_menu();
  Menu::MenuList& list = menu.items();
  
  // Add items for each type of panel
  for(int i = 0; i < PanelManager::get_panel_description_count(); i++)
    {
      list.push_back( Menu_Helpers::StockMenuElem(
        StockID(PanelManager::get_panel_stock_id(i)),
        bind(mem_fun(*this, &PanelBar::on_panel_type), i) ));
    }

  list.push_back( Menu_Helpers::SeparatorElem() );

  // Add extra commands
  list.push_back( Menu_Helpers::MenuElem(_("_Hide"),
    mem_fun(*this, &PanelBar::on_hide) ) );

  list.push_back( Menu_Helpers::CheckMenuElem(_("_Lock"),
    mem_fun(*this, &PanelBar::on_lock) ) );
  lockItem = dynamic_cast<CheckMenuItem*>(&list.back());
  ENSURE(lockItem);
  lockItem->set_active(panel.is_locked());
  
  list.push_back( Menu_Helpers::MenuElem(_("Split _Horizontal"),
    bind(mem_fun(*this, &PanelBar::on_split_panel),
      ORIENTATION_HORIZONTAL) ) );
  list.push_back( Menu_Helpers::MenuElem(_("Split _Vertical"),
    bind(mem_fun(*this, &PanelBar::on_split_panel),
      ORIENTATION_VERTICAL) ) );
}

void
PanelBar::on_realize()
{
  set_flags(Gtk::NO_WINDOW);
  
  // Call base class:
  Gtk::Container::on_realize();
  
  // Create the GdkWindow:
  GdkWindowAttr attributes;
  memset(&attributes, 0, sizeof(attributes));

  const Allocation allocation(get_allocation());

  // Set initial position and size of the Gdk::Window:
  attributes.x = allocation.get_x();
  attributes.y = allocation.get_y();
  attributes.width = allocation.get_width();
  attributes.height = allocation.get_height();

  attributes.event_mask = GDK_ALL_EVENTS_MASK; 
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.wclass = GDK_INPUT_OUTPUT;

  window = Gdk::Window::create(get_window(), &attributes,
          GDK_WA_X | GDK_WA_Y);
  
  window->set_user_data(gobj());
  window->set_cursor(Gdk::Cursor(Gdk::LEFT_PTR));
  
  set_window(window);
  unset_flags(Gtk::NO_WINDOW);
  
  unset_bg(STATE_NORMAL);
}

void
PanelBar::on_size_request(Gtk::Requisition* requisition)
{
  REQUIRE(requisition);
    
  requisition->width = 0;
  requisition->height = 0;
  
  Box::BoxList &list = children();
  Box::BoxList::const_iterator i;
  for(i = list.begin(); i != list.end(); i++)
    {
      Widget *widget = (*i).get_widget();
      REQUIRE(widget);
      
      Requisition child_requisition = widget->size_request();
      requisition->width += child_requisition.width;
      requisition->height = max(requisition->height,
        child_requisition.height);
    }
  
  ENSURE(requisition->width >= 0);
  ENSURE(requisition->height >= 0);
}

void
PanelBar::on_size_allocate(Gtk::Allocation& allocation)
{
  // Use the offered allocation for this container
  set_allocation(allocation);
  
  // Lay out the child widgets
  int offset = 0;
  Box::BoxList &list = children();
  Box::BoxList::const_iterator i;
  
  for(i = list.begin(); i != list.end(); i++)
    {
      Widget *widget = (*i).get_widget();
      REQUIRE(widget);
     
      const Requisition child_requisition = widget->size_request();
      const Gtk::Allocation child_allocation(
          offset, 0,
          min(child_requisition.width, allocation.get_width() - offset),
          allocation.get_height());
          
      offset += child_requisition.width;
      
      widget->size_allocate(child_allocation);
    }
  
  // Resize the window
  int width = allocation.get_width();
  if(window)
    {
      width = max(min(allocation.get_width(), offset), 0);
      window->move_resize(allocation.get_x(), allocation.get_y(),
        width, allocation.get_height());
    }
}

void
PanelBar::on_panel_type(int type_index)
{  
  panel.get_panel_manager().switch_panel(panel, type_index);
}

void
PanelBar::on_hide()
{
  panel.show(false);
}

void
PanelBar::on_lock()
{
  static bool is_locking = false;
 
  REQUIRE(lockItem);
  
  if(!is_locking)
    {
      is_locking = true;
      
      const bool lock = !panel.is_locked();
      panel.lock(lock);
      lockItem->set_active(lock);
      
      is_locking = false;
    }
}

void
PanelBar::on_split_panel(Gtk::Orientation split_direction)
{
  panel.get_panel_manager().split_panel(panel, split_direction);
}

} // widgets
} // gui
