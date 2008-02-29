/*
  DefsManager  -  access to preconfigured default objects and definitions
 
  Copyright (C)         CinelerraCV
    2007,               Hermann Vosseler <Ichthyostega@web.de>
 
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


#include "proc/mobject/session/defsmanager.hpp"
#include "proc/mobject/session/defsregistry.hpp"
#include "common/configrules.hpp"
#include "common/error.hpp"

#include <boost/format.hpp>

using boost::format;

using asset::Query;
using cinelerra::ConfigRules;
using cinelerra::query::QueryHandler;
using cinelerra::query::CINELERRA_ERROR_CAPABILITY_QUERY;


namespace mobject
  {
  namespace session
    {
    using std::tr1::shared_ptr;
    
    
    namespace // Implementation details
      {
      DefsRegistry& defaultsRegistry; //////////////////TODO
    }
    
    /** initialize the most basic internal defaults. */
    DefsManager::DefsManager ()  throw()
    {
      TODO ("setup basic defaults of the session");
    }
    
    
    template<class TAR>
    shared_ptr<TAR>
    DefsManager::search  (const Query<TAR>& capabilities)
    {
      defaultsRegistry.get (capabilities);
    }
    
    
    template<class TAR>
    shared_ptr<TAR> 
    DefsManager::create  (const Query<TAR>& capabilities)
    {
      shared_ptr<TAR> res;
      QueryHandler<TAR>& typeHandler = ConfigRules::instance();  
      typeHandler.resolve (res, capabilities);
      if (res)
        defaultsRegistry.put (res, capabilities);
      return res;
    }
    
    
    template<class TAR>
    bool 
    DefsManager::define  (const shared_ptr<TAR>& defaultObj, const Query<TAR>& capabilities)
    {
      shared_ptr<TAR> candidate (defaultObj);
      QueryHandler<TAR>& typeHandler = ConfigRules::instance();  
      typeHandler.resolve (candidate, capabilities);
      if (!candidate)
        return false;
      else
        return defaultsRegistry.put (candidate, capabilities);
    }
    
    
    template<class TAR>
    bool 
    DefsManager::forget  (const shared_ptr<TAR>& defaultObj)
    {
      defaultsRegistry.forget (defaultObj);
    }

    
    template<class TAR>
    shared_ptr<TAR> 
    DefsManager::operator() (const Query<TAR>& capabilities)
    {
      shared_ptr<TAR> res (search (capabilities));
      if (res) 
        return res;
      else
        res = create (capabilities); // not yet known as default, create new
      
      if (!res)
        throw cinelerra::error::Config ( str(format("The following Query could not be resolved: %s.") 
                                                   % capabilities.asKey())
                                       , CINELERRA_ERROR_CAPABILITY_QUERY );
      else
        return res;
    }

  } // namespace mobject::session

} // namespace mobject
    
    
   /***************************************************************/
   /* explicit template instantiations for querying various Types */
   /***************************************************************/

#include "proc/asset/procpatt.hpp"
#include "proc/asset/pipe.hpp"

namespace mobject
  {
  namespace session
    {

    using asset::Pipe;
    using asset::PPipe;
    using asset::ProcPatt;
    using asset::PProcPatt;
    
    
    template PPipe     DefsManager::operator() (const Query<Pipe>&); 
    template PProcPatt DefsManager::operator() (const Query<const ProcPatt>&); 
    
    template bool DefsManager::define (const PPipe&, const Query<Pipe>&);
    template bool DefsManager::forget (const PPipe&);

  } // namespace mobject::session

} // namespace mobject

