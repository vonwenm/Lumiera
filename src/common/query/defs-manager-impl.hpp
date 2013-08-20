/*
  DEFS-MANAGER-IMPL.h  -  access to preconfigured default objects and definitions

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

*/


/** @file defs-manager-impl.hpp
 ** Implementation of the core defaults-management operations.
 ** These generic implementations are factored out into a separate header (include)
 ** to improve readability. To actually generate code, it is necessary to pull in this
 ** include to create explicit template instantiations with the concrete types to be used
 ** for definition and retrieval of default-configured objects.
 ** 
 ** For the standard use-case within the session / Proc-Layer, this is performed for the
 ** core MObject types, alongside with the definition of the generic config-query-resolver.
 ** 
 ** @see config-resolver.cpp definition of the explicit specialisations for the session 
 ** @see proc::ConfigResolver
 **
 */




#ifndef LUMIERA_QUERY_DEFS_MANAGER_IMPL_H
#define LUMIERA_QUERY_DEFS_MANAGER_IMPL_H


#include "common/query/defs-manager.hpp"
#include "common/query/defs-registry.hpp"
#include "common/config-rules.hpp"
#include "lib/format-string.hpp"
#include "lib/error.hpp"

using util::_Fmt;

using proc::ConfigResolver;
using lumiera::query::QueryHandler;                    ///////TODO preliminary interface defined in config-rules.hpp
using lumiera::query::LUMIERA_ERROR_CAPABILITY_QUERY;


namespace lumiera{
namespace query  {
  
  
  
  
  /** initialise the most basic internal defaults. */
  DefsManager::DefsManager ()  throw()
    : defsRegistry_(new DefsRegistry)
  {
    TODO ("setup basic technical defaults of the session?");
  }
  
  
  
  /** @internal causes boost::checked_delete from \c scoped_ptr<DefsRegistry>
   *  to be placed here, where the declaration of DefsRegistry is available.*/
  DefsManager::~DefsManager() {}
  
  
  void
  DefsManager::clear()
  {
    defsRegistry_.reset(new DefsRegistry);
  }
  
  
  
  template<class TAR>
  P<TAR>
  DefsManager::search  (const Query<TAR>& capabilities)
  {
    P<TAR> res;
    QueryHandler<TAR>& typeHandler = ConfigResolver::instance();
    for (DefsRegistry::Iter<TAR> i = defsRegistry_->candidates(capabilities);
         bool(*i) ; ++i )
      {
        res = *i;
        typeHandler.resolve (res, capabilities);
        if (res)
          return res;
      }
    return res; // "no solution found"
  }
  
  
  template<class TAR>
  P<TAR> 
  DefsManager::create  (const Query<TAR>& capabilities)
  {
    P<TAR> res;
    QueryHandler<TAR>& typeHandler = ConfigResolver::instance();
    typeHandler.resolve (res, capabilities);
    if (res)
      defsRegistry_->put (res, capabilities);
    return res;
  }
  
  
  template<class TAR>
  bool 
  DefsManager::define  (const P<TAR>& defaultObj, const Query<TAR>& capabilities)
  {
    P<TAR> candidate (defaultObj);
    QueryHandler<TAR>& typeHandler = ConfigResolver::instance();  
    typeHandler.resolve (candidate, capabilities);
    if (!candidate)
      return false;
    else
      return defsRegistry_->put (candidate, capabilities);
  }
  
  
  template<class TAR>
  bool 
  DefsManager::forget  (const P<TAR>& defaultObj)
  {
    return defsRegistry_->forget (defaultObj);
  }
  
  
  template<class TAR>
  P<TAR>
  DefsManager::operator() (const Query<TAR>& capabilities)
  {
    P<TAR> res (search (capabilities));
    if (res) 
      return res;
    else
      res = create (capabilities); // not yet known as default, create new
    
    if (!res)
      throw lumiera::error::Config (_Fmt("The following Query could not be resolved: %s.")
                                        % capabilities.rebuild().asKey()
                                   , LUMIERA_ERROR_CAPABILITY_QUERY );
    else
      return res;
  }
  
}} // namespace lumiera::query
#endif /* LUMIERA_QUERY_DEFS_MANAGER_IMPL_H */
