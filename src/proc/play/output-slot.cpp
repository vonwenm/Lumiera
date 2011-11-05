/*
  OutputSlot  -  capability to transfer data to a physical output

  Copyright (C)         Lumiera.org
    2011,               Hermann Vosseler <Ichthyostega@web.de>

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


#include "lib/error.hpp"
#include "proc/play/output-slot.hpp"

#include <boost/noncopyable.hpp>
#include <vector>


namespace proc {
namespace play {
  
  using std::vector;
  
  namespace error = lumiera::error;
  
  
  
  namespace { // hidden local details of the service implementation....
    
  } // (End) hidden service impl details
  
  
  class Connection
    {
    public:
    };
  
  
  class ConnectionState
    : public OutputSlot::Allocation
    , public vector<Connection>
    , boost::noncopyable
    {

      typedef OutputSlot::OpenedSinks OpenedSinks;
      
      
    private: /* == Allocation Interface == */
      OpenedSinks
      getOpenedSinks()
        {
          UNIMPLEMENTED ("yield all opened channels");
        }
      
      bool
      isActive()
        {
          return 0 < size();
        }
      
      
    public:
      ConnectionState()
        {
          UNIMPLEMENTED ("immediately build up the necessary number of connections");
        }

     virtual
    ~ConnectionState()
        {
          UNIMPLEMENTED ("shut down all connections");
        }
    };
  
  
  
  OutputSlot::~OutputSlot() { }  // emit VTables here....
  
  OutputSlot::Allocation::~Allocation() { }
  
  
  
  
  
  /** whether this output slot is occupied
   * @return true if currently unconnected and
   *         able to connect and handle output data 
   */
  bool
  OutputSlot::isFree()  const
  {
    return ! this->state_;
  }
  
  
  /** */
  OutputSlot::Allocation&
  OutputSlot::allocate()
  {
    if (!isFree())
      throw error::Logic ("Attempt to open/allocate an OutputSlot already in use.");
    
    UNIMPLEMENTED ("internal interface to determine the number of channel-connections");
    
    this->state_.reset(new ConnectionState());
    return *state_;
  }
  
  
  
}} // namespace proc::play
