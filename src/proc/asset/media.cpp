/*
  Media(Asset)  -  key abstraction: media-like assets
 
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


#include "proc/assetmanager.hpp"
#include "proc/asset/media.hpp"
#include "proc/asset/clip.hpp"
#include "proc/asset/unknown.hpp"
#include "proc/mobject/session/clip.hpp"
#include "proc/mobject/session/mobjectfactory.hpp"
#include "common/util.hpp"
#include "nobugcfg.h"

#include <boost/regex.hpp>
#include <boost/format.hpp>


using util::isnil;

using boost::format;
using boost::regex;
using boost::smatch;
using boost::regex_search;
using std::tr1::dynamic_pointer_cast;


namespace asset
  {
  
  namespace // Implementation details
  {
    /** helper: extract a name token out of a given path/filename
     *  @return sanitized token based on the name (minus extension),
     *          empty string if not the common filename pattern.
     */
    string extractName (const string& path)
    {
      regex pathname_pattern("([^/\\.]+)(\\.\\w+)?$");
      smatch match;
      
      if (regex_search (path, match, pathname_pattern))
        return util::sanitize (string (match[1]));
      else
        return "";
    }
  } 

  
  
  
  
  Media::PClipMO
  Media::createClip ()
  {
    PClip clipAsset (getClipAsset()); 
    PClipMO clipMO = clipAsset->createClip();
    
    ENSURE (clipMO->isValid());
    return clipMO;
  }
  
  
  /** @internal used to either create an asset::Clip denoting the whole media,
   *            or to get the right reference to some already existing asset::Clip,
   *            especially when this media is part of a compound (multichannel) media.
   */
  Media::PClip
  Media::getClipAsset ()
  {
    if (PMedia parent = this->checkCompound())
      return parent->getClipAsset();
    else
      return Media::create(*this);
  }
  
  
  Media::PMedia
  Media::checkCompound()  const
  {
    vector<PAsset> parents = this->getParents();
    PMedia parent;
    if ( !isnil (parents))  // primary parent is a media asset?
       parent = dynamic_pointer_cast<Media,Asset> (parents[0]);
    return parent;
  }
  
  
  Media::PProcPatt
  Media::howtoProc ()  const
  {
    UNIMPLEMENTED ("calculate and return processing pattern for media asset");
    PProcPatt ppatt; //TODO:null
    
    ENSURE (ppatt);
    return ppatt;
  }
  
  
  cinelerra::Time
  Media::getLength()  const
  {
     return len_;
  }
  
  
  
  MediaFactory Media::create;  ///< storage for the static MediaFactory instance
  
  
  
  /** Factory method for Media Asset instances. Depending on the filename given,
   *  either a asset::Media object or an "Unknown" placeholder will be provided. If
   *  the given Category already contains an "Unkown", we just get the
   *  corresponding smart-ptr. Otherwise a new asset::Unknown is created.
   *  @return an Media smart ptr linked to the internally registered smart ptr
   *          created as a side effect of calling the concrete Media subclass ctor.
   */
  MediaFactory::PType 
  MediaFactory::operator() (Asset::Ident& key, const string& file)
  { 
    asset::Media* pM (0);
    AssetManager& aMang = AssetManager::instance();
    
    TODO ("check and fix Category if necessary");
    
    if (isnil (file))
      {
        if (isnil (key.name)) key.name="nil";
        ID<Asset> id = aMang.getID (key);
        if (aMang.known (id))
          return aMang.getAsset(ID<Media>(id));
        else
          pM = new Unknown(key);
      }
    else
      {
        if (isnil (key.name)) key.name=extractName(file);
        TODO ("file exists?");
        TODO ("extract media file properties");
        Time length(25);
        TODO ("detecting and wiring multichannel compound media!");
        pM = new Media (key,file,length); 
      }
    ASSERT (pM);
    ENSURE (key.category.hasKind (VIDEO) || key.category.hasKind(AUDIO));
    ENSURE (!isnil (key.name));
    ENSURE (dynamic_cast<Media*>(pM) || (isnil (file) && dynamic_cast<Unknown*>(pM)));
    
    return aMang.getAsset (pM->getID());  // note: because we query with an ID<Media>, 
                                         //        we get a Media smart ptr.    
  }

  
  /** Variant of the Factory method for Media Assets, automatically 
   *  providing most of the Asset key fields based on the filename given
   */
  MediaFactory::PType 
  MediaFactory::operator() (const string& file, const Category& cat)
  { 
    Asset::Ident key(extractName(file), cat, "cin3", 1);
    return operator() (key, file);
  }
  
  MediaFactory::PType 
  MediaFactory::operator() (const string& file, asset::Kind kind)
  { 
    Category cat(kind);
    return operator() (file, cat);
  }

  
  MediaFactory::PType 
  MediaFactory::operator() (const char* file, const Category& cat)
  { 
    if (!file) file = "";
    return operator() (string(file),cat);
  }
  
  MediaFactory::PType 
  MediaFactory::operator() (const char* file, asset::Kind kind)
  { 
    if (!file) file = "";
    return operator() (string(file),kind);
  }
  
  MediaFactory::PType 
  MediaFactory::operator() (Asset::Ident& key, const char* file)
  {
    if (!file) file = "";
    return operator() (key, string(file));
  }
  
  
  /** Factory method for creating a Clip asset based
   *  on the given Media asset. This asset::Clip can be used
   *  to create clip in the EDL covering the whole length of
   *  this media.
   *  @note  creates a dependency between media and new clip 
   *  @throw Invalid if the given media asset is not top-level,
   *         but rather part or a multichannel (compound) media
   */
  shared_ptr<asset::Clip>
  MediaFactory::operator() (asset::Media& mediaref)  throw(cinelerra::error::Invalid)
  {
    if (mediaref.checkCompound())
      throw cinelerra::error::Invalid (str(format("Attempt to create a asset::Clip from the media %s, "
                                                  "which is not toplevel but rather part or a compound "
                                                  "(multichannel) media. Found parent Media %s.") 
                                                  % string(mediaref) 
                                                  % string(*mediaref.checkCompound()))
                                      ,CINELERRA_ERROR_PART_OF_COMPOUND
                                      );
    asset::Clip* pC = new asset::Clip (mediaref);
    return AssetManager::instance().getPtr (*pC);
  }
  
  CINELERRA_ERROR_DEFINE (PART_OF_COMPOUND, "part of compound used as toplevel element");
  



} // namespace asset