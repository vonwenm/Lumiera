/*
  main.cpp  -  start the Lumiera Application

  Copyright (C)         Lumiera.org
    2007-2008,          Joel Holdsworth <joel@airwebreathe.org.uk>
                        Christian Thaeter <ct@pipapo.org>
                        Hermann Vosseler <Ichthyostega@web.de>

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


#include <iostream>

#include "proc/common.hpp"

using std::cout;
using std::endl;
using lumiera::Appconfig;
using lumiera::ON_GLOBAL_INIT;
using lumiera::ON_GLOBAL_SHUTDOWN;


int main (int argc, char* argv[])
{
  cout << "*** Lumiera NLE for Linux ***" << endl;
  
  Appconfig::lifecycle (ON_GLOBAL_INIT);
  
  // great things are happening here....
  
  Appconfig::lifecycle (ON_GLOBAL_SHUTDOWN);
  return 0;
}
