/*
  panel-manager.cpp  -  Definition of the panel manager object
 
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

#include "panel-manager.hpp"

#include "../panels/resources-panel.hpp"
#include "../panels/viewer-panel.hpp"
#include "../panels/timeline-panel.hpp"

#include "include/logging.h"

using namespace boost;
using namespace std;

namespace gui {
namespace workspace {

const PanelManager::PanelDescription
  PanelManager::panelDescriptionList[] = {
  PanelManager::Panel<TimelinePanel>(),
  PanelManager::Panel<ViewerPanel>(),
  PanelManager::Panel<ResourcesPanel>()
  };

PanelManager::PanelManager(WorkspaceWindow &workspace_window) :
  workspaceWindow(workspace_window),
  dock(NULL),
  dockBar(NULL),
  dockLayout(NULL)
{
  memset(&dockPlaceholders, 0, sizeof(dockPlaceholders)); 
}

PanelManager::~PanelManager()
{
  if(dock)
    g_object_unref(dock);
    
  if(dockBar)
    g_object_unref(dockBar);
    
  if(dockLayout)
    g_object_unref(dockLayout);
  
  for(int i = 0; i < 4; i++)
    if(dockPlaceholders[i])
      g_object_unref(dockPlaceholders[i]);
}

void
PanelManager::setup_dock()
{
  REQUIRE(dock == NULL);
  dock = GDL_DOCK(gdl_dock_new());
  ENSURE(dock);
  
  REQUIRE(dockBar == NULL);
  dockBar = GDL_DOCK_BAR(gdl_dock_bar_new(dock));
  ENSURE(dockBar);
  
  REQUIRE(dockLayout == NULL);
  dockLayout = GDL_DOCK_LAYOUT(gdl_dock_layout_new(dock));
  ENSURE(dockLayout);
  
  REQUIRE(dockPlaceholders[0] == NULL && dockPlaceholders[1] == NULL &&
    dockPlaceholders[2] == NULL && dockPlaceholders[3] == NULL);
  dockPlaceholders[0] = GDL_DOCK_PLACEHOLDER(gdl_dock_placeholder_new(
    "ph1", GDL_DOCK_OBJECT(dock), GDL_DOCK_TOP, FALSE));
  dockPlaceholders[1] = GDL_DOCK_PLACEHOLDER(gdl_dock_placeholder_new(
    "ph2", GDL_DOCK_OBJECT(dock), GDL_DOCK_BOTTOM, FALSE));
  dockPlaceholders[2] = GDL_DOCK_PLACEHOLDER(gdl_dock_placeholder_new(
    "ph3", GDL_DOCK_OBJECT(dock), GDL_DOCK_LEFT, FALSE));
  dockPlaceholders[3] = GDL_DOCK_PLACEHOLDER(gdl_dock_placeholder_new(
    "ph4", GDL_DOCK_OBJECT(dock), GDL_DOCK_RIGHT, FALSE));
  ENSURE(dockPlaceholders[0] && dockPlaceholders[1] &&
    dockPlaceholders[2] && dockPlaceholders[3]);
  
  create_panels();
}

GdlDock*
PanelManager::get_dock() const
{
  ENSURE(dock);
  return dock;
}

GdlDockBar*
PanelManager::get_dock_bar() const
{
  ENSURE(dockBar);
  return dockBar;
}

int
PanelManager::get_panel_description_count()
{
  return sizeof(panelDescriptionList) / sizeof(PanelDescription);
}

const gchar*
PanelManager::get_panel_stock_id(int index)
{
  REQUIRE(index >= 0 && index < get_panel_description_count());
  return panelDescriptionList[index].get_stock_id();
}

void
PanelManager::create_panels()
{
  shared_ptr<panels::Panel> resourcesPanel(
    (panels::Panel*)new ResourcesPanel(workspaceWindow));
  shared_ptr<panels::Panel> viewerPanel(
    (panels::Panel*)new ViewerPanel(workspaceWindow));
  shared_ptr<panels::Panel> timelinePanel(
    (panels::Panel*)new TimelinePanel(workspaceWindow));
    
  gdl_dock_add_item(dock,
    resourcesPanel->get_dock_item(), GDL_DOCK_LEFT);
  gdl_dock_add_item(dock,
    timelinePanel->get_dock_item(), GDL_DOCK_BOTTOM);
  gdl_dock_add_item(dock,
    viewerPanel->get_dock_item(), GDL_DOCK_RIGHT);

  panels.push_back(timelinePanel);
  panels.push_back(viewerPanel);
  panels.push_back(resourcesPanel);
}

shared_ptr<panels::Panel> 
PanelManager::create_panel_by_name(const char* class_name)
{
  const int count = get_panel_description_count();
  for(int i = 0; i < count; i++)
    {
      if(strstr(panelDescriptionList[i].get_class_name(), class_name))
        return shared_ptr<panels::Panel>(panelDescriptionList[i].create(
          workspaceWindow));
    }
    
  ERROR(gui, "Unable to create a panel with class name %s", class_name);
  return shared_ptr<panels::Panel>();
}

}   // namespace workspace
}   // namespace gui
