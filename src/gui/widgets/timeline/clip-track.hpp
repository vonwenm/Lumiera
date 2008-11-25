/*
  clip-track.hpp  -  Declaration of the timeline track object
 
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
/**
 * @file widgets/timeline/clip-track.hpp
 * This file contains the definition of timeline track objects which
 * contain clips.
 */

#ifndef CLIP_TRACK_HPP
#define CLIP_TRACK_HPP

#include "track.hpp"

namespace gui {
namespace widgets {
namespace timeline {

class Clip;
class TimelineViewWindow;
  
class ClipTrack : public Track
{
public:
  ClipTrack();
  
  void draw_track(Cairo::RefPtr<Cairo::Context> cairo,
    TimelineViewWindow* const window) const;
  
private:
  
  std::vector<Clip*> clips;
  
};

}   // namespace timeline
}   // namespace widgets
}   // namespace gui

#endif // CLIP_TRACK_HPP