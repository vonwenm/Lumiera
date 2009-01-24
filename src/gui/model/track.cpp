/*
  track.cpp  -  Implementation of the Track class
 
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

#include "track.hpp"
#include "parent-track.hpp"
#include <boost/foreach.hpp>

using namespace boost;
using namespace std;

namespace gui {
namespace model {
  
const list< shared_ptr<Track> > Track::NoChildren;

Track::Track()
{

}

const std::list< boost::shared_ptr<Track> >&
Track::get_child_tracks() const
{
  return Track::NoChildren;
}

const string
Track::get_name() const
{
  return name;
}

void
Track::set_name(const string &name)
{
  this->name = name;
}

bool
Track::can_host_children() const
{
  return false;
}

string
Track::print_branch()
{
  return print_branch_recursive(0);
}

boost::shared_ptr<ParentTrack>
Track::find_descendant_track_parent(
  boost::shared_ptr<Track> /*child*/)
{ 
  return shared_ptr<ParentTrack>();
}

string
Track::print_branch_recursive(const unsigned int indentation)
{
  string str;
  
  for(unsigned int i = 0; i < indentation; i++)
    str += "  ";
  str += print_track();
  str += "\n";
  
  BOOST_FOREACH(shared_ptr<Track> track, get_child_tracks())
    {
      REQUIRE(track);
      str += track->print_branch_recursive(indentation + 1);
    }
  
  return str;
}

}   // namespace model
}   // namespace gui
