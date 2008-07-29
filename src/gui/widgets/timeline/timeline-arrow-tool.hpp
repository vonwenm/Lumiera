/*
  timeline-arrow-tool.hpp  -  Declaration of the ArrowTool class
 
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
/** @file timeline-arrow-tool.hpp
 ** This file contains the definition of the arrow tool class
 */

#ifndef TIMELINE_ARROW_TOOL_HPP
#define TIMELINE_ARROW_TOOL_HPP

#include <gtkmm.h>
#include "timeline-tool.hpp"

namespace lumiera {
namespace gui {
namespace widgets {
namespace timeline {

class ArrowTool : public Tool
{
public:
  ArrowTool();

  ToolType get_type() const;
};

}   // namespace timeline
}   // namespace widgets
}   // namespace gui
}   // namespace lumiera

#endif // TIMELINE_ARROW_TOOL_HPP
