/*
  convert.hpp  - Declares utility functions for conversions
 
  Copyright (C)         Lumiera.org
    2006,               Paul Davis
    2010,               Stefan Kangas <skangas@skangas.se>
 
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

#include <string>

using std::string;

namespace gui {
namespace util {

double atof(const string& s);

int atoi(const string& s);

}   // namespace util
}   // namespace gui


