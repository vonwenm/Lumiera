/*
  render.cpp  -  Definition of the main workspace window object
 
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

namespace lumiera {
namespace gui {
namespace dialogs {


  Render::Render(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& xml) :
  Gtk::Dialog(cobject)
  {
    
  }

  void
  Render::init(const Glib::RefPtr<Gnome::Glade::Xml>& glade_xml, Render*& dialog)
  {
    glade_xml->get_widget_derived("Render", dialog);
  }

}   // namespace dialogs
}   // namespace gui
}   // namespace lumiera