/*
  Timings  -  timing specifications for a frame quantised data stream

  Copyright (C)         Lumiera.org
    2012,               Hermann Vosseler <Ichthyostega@web.de>

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


#include "proc/play/timings.hpp"
#include "lib/time/formats.hpp"
#include "lib/time/timequant.hpp"



namespace proc {
namespace play {
  
  
  using lib::time::PQuant;
  using lib::time::Time;
  
  namespace { // hidden local details of the service implementation....
    
    inline PQuant
    buildStandardGridForFramerate (FrameRate fps)
    {
      return PQuant (new lib::time::FixedFrameQuantiser (fps));
    }                         //////TODO maybe caching these quantisers? they are immutable and threadsafe
    
    
  } // (End) hidden service impl details
  
  
  /** Create a default initialised Timing constraint record.
   * Using the standard optimistic settings for most values,
   * no latency, no special requirements. The frame grid is
   * rooted at the "natural" time origin; it is not related
   * in any way to the current session.
   * @remarks this ctor is intended rather for testing purposes!
   *          Usually, when creating a play/render process,
   *          the actual timings \em are related to the timeline
   *          and the latency/speed requirements of the output.
   */
  Timings::Timings (FrameRate fps)
    : grid_(buildStandardGridForFramerate(fps))
    , playbackUrgency (ASAP)
    , outputLatency (Duration::NIL)
    { 
      ENSURE (grid_);
    }
  
  
  
  TimeValue
  Timings::getOrigin()  const
  {
    return grid_->timeOf(0);
  }
  
  
  Offset
  Timings::getFrameOffsetAt (TimeValue refPoint)  const
  {
    return 1 * getFrameDurationAt (refPoint);      /////////////////TODO implement a speed factor here
  }
  
  
  Duration
  Timings::getFrameDurationAt (TimeValue refPoint)  const
  {
    int64_t frameNr = grid_->gridPoint (refPoint);
    return getFrameDurationAt(frameNr);
  }
  
  
  Duration
  Timings::getFrameDurationAt (int64_t refFrameNr)  const
  {
    return Offset (grid_->timeOf(refFrameNr), grid_->timeOf(refFrameNr + 1));
  }

  
  
  /** @remarks the purpose of this function is to support scheduling
   *           and frame handling even in case the frame rate isn't constant.
   *           To indicate the case the frame rate is changing right now,
   *           this function might returns Duration::NIL
   *  @todo implement real support for variable frame rates      
   */                                  ////////////////////////////////////////////////////////TICKET #236
  Duration
  Timings::constantFrameTimingsInterval (TimeValue startPoint)  const
  {
    return Duration (Time::ANYTIME);
  }
  
  
  Time
  Timings::getTimeDue()  const
  {
    if (TIMEBOUND == playbackUrgency)
      {
        UNIMPLEMENTED ("scheduled delivery spec");
      }
    else
      return Time::NEVER;
  }
  
  
  uint
  Timings::getPlanningChunkSize()  const
  {
    UNIMPLEMENTED ("how to control the engine evaluation chunk size");
  }
  
  
  Timings
  Timings::constrainedBy (Timings additionalConditions)
  {
    UNIMPLEMENTED ("how to combine timing constraints");
  }
  
  
  
}} // namespace proc::play