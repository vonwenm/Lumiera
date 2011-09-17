/*
  BufferProvider  -  Abstraction for Buffer management during playback/render

  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>

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


#include "proc/engine/buffer-provider.hpp"

namespace engine {
  
  
  namespace { // impl. details and definitions
    
    const uint DEFAULT_DESCRIPTOR = 0;
  }


  BufferProvider::~BufferProvider() { }
  
  
  /** @internal verify the given descriptor.
   *  @return true if it corresponds to a buffer
   *          currently locked and usable by client code
   */
  bool
  BufferProvider::verifyValidity (BufferDescriptor const&)
  {
    UNIMPLEMENTED ("BufferProvider basic and default implementation");
  }
  
  
  BufferDescriptor
  BufferProvider::getDefaultDescriptor()
  {
    return BufferDescriptor (*this, DEFAULT_DESCRIPTOR);
  }
      
      
  
  
  /* === BufferDescriptor and BuffHandle === */
  
  bool
  BufferDescriptor::verifyValidity()  const
  {
    return provider_->verifyValidity(*this);
  }
  


} // namespace engine
