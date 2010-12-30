/*
  TIME-GRID.hpp  -  reference scale for quantised time

  Copyright (C)         Lumiera.org
    2010,               Hermann Vosseler <Ichthyostega@web.de>

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

*/


/** @file time-grid.hpp
 ** Establishing a reference scale for quantised time values.
 ** Contrary to the continuous Time values (Lumiera internal time scale),
 ** quantised time can only take on some discrete values corresponding
 ** to blocks or frames, according to the respective \em timecode format.
 ** The act of quantising continuous time values requires the definition
 ** of a time grid of such allowed time values. At least, the specification
 ** of a time origin and a frame spacing is required, but there might be
 ** way more elaborate specifications, e.g. a grid varying over time.
 ** 
 ** Time grid specifications are integrated into Lumiera's framework
 ** for meta assets, automation, configuration and similar metadata.
 ** 
 ** @see MetaFactory creating concrete asset::Meta instances
 ** @see time::Quantiser
 **
 */

#ifndef ASSET_META_TIME_GRID_H
#define ASSET_META_TIME_GRID_H

#include "proc/asset/meta.hpp"
#include "lib/time/quantiser.hpp"



namespace asset {
namespace meta {
  
 
  /**
   * Interface: a grid and scale for time quantisation.
   * This meta-Asset describes a coordinate system or
   * reference scale for quantised time values.
   */ 
  class TimeGrid
    : public Meta
    {
      
    public:
      struct Builder
        {
        
        };
      
    protected:
      friend class Builder;
      
      TimeGrid (Builder const& setup); //////////////TODO
    };
  
  
}} // namespace asset::meta
#endif
