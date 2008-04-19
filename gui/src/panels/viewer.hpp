/*
  viewer.hpp  -  Definition of the viewer panel            
 
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
/** @file viewer.hpp
 ** This file contains the definition of the viewer panel
 */

#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <gtkmm.h>

#include "panel.hpp"
#include "../widgets/video-display.hpp"

using namespace lumiera::gui::widgets;
using namespace Gtk;

namespace lumiera {
namespace gui {
namespace panels {

  class Viewer : public Panel
    {
    public:
      Viewer();

    protected:

      ToolButton previousButton;
      ToolButton rewindButton;
      ToolButton playPauseButton;
      ToolButton forwardButton;
      ToolButton nextButton;

      VideoDisplay display;
      Toolbar toolBar;
    };

}   // namespace panels
}   // namespace gui
}   // namespace lumiera

#endif // VIEWER_HPP
