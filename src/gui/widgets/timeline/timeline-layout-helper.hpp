/*
  timeline-layout-helper.hpp  -  Declaration of the timeline
  layout helper class
 
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
/** @file timeline-layout-helper.cpp
 ** This file contains the definition of the layout helpeer class
 */

#ifndef TIMELINE_LAYOUT_HELPER_HPP
#define TIMELINE_LAYOUT_HELPER_HPP

#include "../../gtk-lumiera.hpp"
#include "../../../lib/tree.hpp"

namespace gui {

namespace model {
class Track;
}
  
namespace widgets {

class TimelineWidget;
  
namespace timeline {
  
class Track;

/**
 * A helper class for the TimelineWidget. TimelineLayoutHelper
 * is a class which calculates the layout of tracks in the timeline
 * track tree.
 */
class TimelineLayoutHelper : public boost::noncopyable
{
public:
  typedef lumiera::tree< boost::shared_ptr<model::Track> > TrackTree;
  
public:
  TimelineLayoutHelper(TimelineWidget &owner);
    
  void clone_tree_from_sequence();
  
  TrackTree& get_layout_tree();
  
  /**
   * Recalculates the track layout from layoutTree.
   **/
  void update_layout();
  
  boost::optional<Gdk::Rectangle> get_track_header_rect(
    boost::weak_ptr<timeline::Track> track);
    
  boost::weak_ptr<timeline::Track> header_from_point(
    const Gdk::Point &point);
    
  boost::weak_ptr<timeline::Track> track_from_y(const int y);
    
  int get_total_height() const;
  
protected:
  
  void add_branch(TrackTree::iterator_base parent_iterator, 
    boost::shared_ptr<model::Track> parent);
  

  
  /**
   * Recursively lays out all the controls in the header widget.
  
  * /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  * 
  * 
  *  * @param track The parent track object which will be recursed into.
   * @param offset A shared value used to accumulate the y-offset of
   * header widgets.
   * @param header_width The width of this widget in pixels.
   * !!!!!!!!!!! indent_width
   * @param depth The depth within the tree of track.
   **/
  void layout_headers_recursive(
    TrackTree::iterator_base parent_iterator,
    int &offset, const int header_width, const int indent_width,
    const int depth, bool parent_expanded);
  
  /**
   * A helper function which calls lookup_timeline_track within the
   * parent timeline widget, but also applies lots of data consistency
   * checks in the process.
   * @param model_track The model track to look up in the parent widget.
   * @return Returns the track found, or returns NULL if no matching
   * track was found.
   * @remarks If the return value is going to be NULL, an ENSURE will
   * fail.
   **/
  boost::shared_ptr<timeline::Track> lookup_timeline_track(
    boost::shared_ptr<model::Track> model_track);
  
protected:

  TimelineWidget &timelineWidget;
  
  TrackTree layoutTree;
  
  /**
   * A map of tracks to the rectangles of their headers.
   * @remarks This map is used as a cache, so that the rectangles don't
   * need to be perpetually recalculated. This cache is regenerated by
   * the layout_headers method.
   **/
  std::map<boost::weak_ptr<timeline::Track>, Gdk::Rectangle>
    headerBoxes;
    
  int totalHeight;
};

}   // namespace timeline
}   // namespace widgets
}   // namespace gui

#endif // TIMELINE_LAYOUT_HELPER_HPP
