/*
  assets-panel.cpp  -  Implementation of the assets panel
 
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

#include "../gtk-lumiera.hpp"
#include "resources-panel.hpp"

namespace gui {
namespace panels {

ResourcesPanel::ResourcesPanel(workspace::WorkspaceWindow &workspace_window) :
  Panel(workspace_window, "resources", _("Resources"), "panel_resources")
{
  
  notebook.append_page(media, _("Media"));
  notebook.append_page(clips, _("Clips"));
  notebook.append_page(effects, _("Effects"));
  notebook.append_page(transitions, _("Transitions"));

  pack_start(notebook);
}

}   // namespace panels
}   // namespace gui