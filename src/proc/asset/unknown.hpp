/*
  UNKNOWN.hpp  -  placeholder for unknown or unavailable media source
 
  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>
 
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


#ifndef ASSET_UNKNOWN_H
#define ASSET_UNKNOWN_H

#include "proc/asset/media.hpp"



namespace asset
  {
  

  const lumiera::Time DUMMY_TIME (25); ///< @todo solve konfig management

  /**
   * Placeholder Asset for unknown or unavailable media source.
   * @todo maybe do special handling of the media length, allowing
   *       it to support existing clips even if the media length 
   *       is not known? 
   */
  class Unknown : public Media
    {
    protected:
      Unknown (const Asset::Ident& idi, string name="", Time length=DUMMY_TIME);
      friend class MediaFactory;
      
    public:
      virtual Media::PMedia getOrg()  throw(lumiera::error::Invalid);
      
    };
    
  LUMIERA_ERROR_DECLARE (ORIG_NOT_FOUND);
    
    
    
    
} // namespace asset
#endif
