/*
  STRUCT.hpp  -  key abstraction: structural asset
 
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


/** @file struct.hpp
 ** Structural parts of the Session (e.g. Tracks) can be reflected
 ** into the "bookkeeping view" as a specific Kind of Asset.
 ** For the different <i>Kinds</i> of Assets, we use sub-interfaces inheriting
 ** from the general Asset interface. To be able to get asset::Struct instances
 ** directly from the AssetManager, we define a specialisation of the Asset ID.
 **
 ** @see asset.hpp for explanation
 ** @see StructFactory creating concrete asset::Struct instances
 **
 */


#ifndef ASSET_STRUCT_H
#define ASSET_STRUCT_H

#include "pre_a.hpp"

#include "proc/asset.hpp"
#include "lib/query.hpp"
#include "lib/factory.hpp"
#include "lib/singleton.hpp"

#include <boost/scoped_ptr.hpp>
#include <string>



namespace asset {
  
  using std::string;
  using std::wstring;
  using boost::scoped_ptr;
  using lumiera::Query;
  
  class Struct;
  class StructFactory;
  class StructFactoryImpl;
  class Pipe;
  
  
  template<>
  class ID<Struct> : public ID<Asset>
    {
    public:
      ID (size_t id);
      ID (const Struct&);
    };

    
    
  /**
   * key abstraction: structural asset
   * @todo just a stub, have to figure out what a asset::Struct is
   */
  class Struct : public Asset
    {
    public:
      static StructFactory create;
      
      virtual const ID<Struct>& getID()  const    ///< @return ID of kind asset::Struct 
        { 
          return static_cast<const ID<Struct>& > (Asset::getID()); 
        }
      
      const string queryStreamID()  const;
      const string queryPipeID()    const;
      
      
    protected:
      Struct (const Asset::Ident& idi) : Asset(idi) {}
      friend class StructFactory;
    };
    
    
    // definition of ID<Struct> ctors is possible now,
   //  after providing full definition of class Proc

  inline ID<Struct>::ID(size_t id)          : ID<Asset> (id)           {};
  inline ID<Struct>::ID(const Struct& stru) : ID<Asset> (stru.getID()) {};
  
  
  
  
  
  
  /** 
   * Factory specialised for creating Structural Asset objects.
   */ 
  class StructFactory : public lib::Factory<asset::Struct>
    {
      scoped_ptr<StructFactoryImpl> impl_;

    protected:
      StructFactory ();
      friend class Struct;

      
    public:
      typedef P<asset::Struct> PType;
      
      template<class STRU>
      P<STRU> operator() (const Query<STRU>& query);      ////////////TODO actually do something sensible here 
      
      P<Pipe> operator() (string pipeID, string streamID);
      
    };
    
    
    
} // namespace asset
#endif
