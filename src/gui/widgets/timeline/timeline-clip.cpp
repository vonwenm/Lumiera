/*
  timeline-clip.cpp  -  Implementation of the timeline clip object

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

#include "timeline-clip.hpp"

namespace gui {
namespace widgets {
namespace timeline {

  Clip::Clip(boost::shared_ptr<model::Clip> clip)
    : modelClip(clip),
      selected(false)
  {
    REQUIRE(modelClip);

    // TODO: Connect signals
    //modelClip->signalNameChanged().connect(mem_fun(this,
    //  &Clip::onNameChanged);
  }

  void
  Clip::draw_clip(Cairo::RefPtr<Cairo::Context> cr,
                  TimelineViewWindow* const window) const
  {
    REQUIRE(cr);
    REQUIRE(window);
    REQUIRE(modelClip);

    int x = window->time_to_x(modelClip->getBegin());
    int width = window->time_to_x(
      modelClip->getEnd()) - window->time_to_x(modelClip->getBegin());

    // Draw a rectangle for the clip
    cr->rectangle(x, 1, width, 100-2);
    // TODO: get height from the Timeline::Track

    if (selected)
      cr->set_source(Cairo::SolidPattern::create_rgb (0.4, 0.4, 0.8));
    else
      cr->set_source(Cairo::SolidPattern::create_rgb (0.4, 0.4, 0.4));
    cr->fill_preserve();

    cr->set_source_rgb(0.25, 0.25, 0.25);
    cr->stroke();

    // Show the clip name
    cr->rectangle(x, 1, width, 100-2);
    cr->clip();

    cr->move_to (x + 3, 12);
    cr->set_source_rgb (1.0, 1.0, 1.0);

    cr->set_font_size (9);
    cr->show_text (modelClip->getName());

    // TODO: Show thumbnails for clip
  }

  void
  Clip::setSelected(bool selected)
  {
    this->selected = selected;
  }

}   // namespace timeline
}   // namespace widgets
}   // namespace gui

