/*
  parent-track-.cpp  -  Implementation of the ParentTrack class
 
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

#include "parent-track.hpp"
#include <boost/foreach.hpp>

using namespace boost;

namespace gui {
namespace model {

ParentTrack::ParentTrack()
{
}

const std::list< boost::shared_ptr<Track> >&
ParentTrack::get_child_tracks() const
{
  return tracks.get_list();
}

lumiera::observable_list< boost::shared_ptr<Track> >&
ParentTrack::get_child_track_list()
{
  return tracks;
}

bool
ParentTrack::can_host_children() const
{
  return true;
}

bool
ParentTrack::remove_descendant_track(const boost::shared_ptr<Track> track)
{
  REQUIRE(track);
  
  BOOST_FOREACH(const boost::shared_ptr<Track> child_track, tracks)
    {
      REQUIRE(child_track);
      
      if(track.get() == child_track.get())
        {
          tracks.remove(track);
          return true;
        }
        
      if(child_track->remove_descendant_track(track))
        return true;
    }
    
  return false;
}

boost::shared_ptr<ParentTrack>
ParentTrack::find_descendant_track_parent(
  boost::shared_ptr<Track> child)
{ 
  REQUIRE(child != NULL);
  BOOST_FOREACH(shared_ptr<Track> track, tracks)
    {
      if(track == child)
        return shared_from_this();
    
      shared_ptr<ParentTrack> result =
        track->find_descendant_track_parent(child);
      if(result)
        return result;
    }
  
  return shared_ptr<ParentTrack>();
}

}   // namespace model
}   // namespace gui
