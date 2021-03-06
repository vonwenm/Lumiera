/*
  MObjectFactory  -  creating concrete MObject subclass instances

  Copyright (C)         Lumiera.org
    2008,2010,          Hermann Vosseler <Ichthyostega@web.de>

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


#include "proc/mobject/session/mobjectfactory.hpp"
#include "proc/mobject/session/root.hpp"
#include "proc/mobject/session/clip.hpp"
#include "proc/mobject/session/track.hpp"
#include "proc/mobject/session/effect.hpp"
#include "proc/mobject/session/label.hpp"
#include "proc/mobject/session/binding.hpp"
#include "proc/asset/clip.hpp"
#include "proc/asset/effect.hpp"
#include "common/query/defs-manager.hpp"

using lumiera::query::DefsManager;

namespace proc {
namespace mobject {
namespace session {
    ////////////////////////////////////////////////////////////////////////////////TICKET #414
  
  
  
  /** build a new session/model root element. */
  Placement<Root>
  MObjectFactory::operator() (DefsManager& sessionDefaultsHandler)
  {
    return Placement<Root> (*new Root (sessionDefaultsHandler), &deleterFunc);
  }
  
  
  /** build a new session/model root element. */
  Placement<Label>
  MObjectFactory::operator() (Symbol labelType)
  {
    return Placement<Label> (*new Label (labelType), &deleterFunc);
  }
  
  
  /** creating a Clip-MObject to be placed within the Session,
   *  based on a clip asset, which typically is obtained by calling
   *  the createClip()-function on some asset::Media.
   *  The newly created Clip-MO is (back)linked with this media Asset
   *  internally and wrapped into a Placement, which takes ownership.
   *  So, when the render engine gets across this Clip-MO, it is able
   *  to obtain the media information contained in the corresponding
   *  media asset. Note this will create a single track clip. 
   *  @param mediaDef the actual asset::Media to be used 
   *  created Clip could be a compound (multichannel) clip 
   *  comprised of several SimpleClip sub-objects.
   */
  Placement<Clip>   
  MObjectFactory::operator() (const asset::Clip& clipRef, const asset::Media& mediaDef)
  {
    return Placement<Clip> (*new Clip (clipRef,mediaDef), &deleterFunc);
  }
  
  /** variant of the Clip-MO factory function, creating
   *  a multichannel (compound) clip.
   *  @todo work out the details of multichannel handling
   */
  Placement<Clip>
  MObjectFactory::operator() (const asset::Clip& clipRef, vector<const Media*> mediaTracks)
  {
    UNIMPLEMENTED ("build multichannel compound clip-MO");
  }
  
  /** fabricate a new track-MObject, using the given unique ID */ 
  Placement<Track>
  MObjectFactory::operator() (TrackID const& id)
  {
    TODO ("what needs to be registered when creating an Track-MO?");
    TODO ("assure the ID is indeed unique and not already used for another track");  //////////TICKET #638
    return Placement<Track> (*new Track (id), &deleterFunc);
  }
  
  /** */ 
  Placement<Effect>
  MObjectFactory::operator() (const asset::Effect& processorDef)
  {
    UNIMPLEMENTED ("fabricate effect-MO");
  }
  
  /** */ 
  Placement<Binding>
  MObjectFactory::operator() (PSequence const& sequence_to_bind)
  {
    TODO ("any additional liabilities when creating a binding?");
    return Placement<Binding> (*new Binding(sequence_to_bind), &deleterFunc);
  }
  
  
  
}}} // namespace mobject::session
