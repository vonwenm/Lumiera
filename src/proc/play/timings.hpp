/*
  TIMINGS.hpp  -  timing specifications for a frame quantised data stream

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

*/

/** @file timings.hpp
 ** How to define a timing specification or constraint.
 ** A timing spec is used to anchor a data stream with relation to a time axis or frame grid.
 ** There are two kinds of timing specs:
 ** - nominal timing specifications relate to the nominal time values
 **   of the frames in a data stream, i.e. the "should be" time values.
 **   These might be values derived from a timecode or just values in
 **   in relation to the timeline axis, but without any connection to
 **   the real wall clock time
 ** - actual timing specifications are always connected or related to
 **   an external time source, typically just wall clock time. For example,
 **   actual timing specs dictate the constraints for real time frame
 **   delivery to an external output connection.
 **   
 ** @todo WIP-WIP-WIP 8/2011
 ** @see output-slot.hpp  ////TODO
 */


#ifndef PROC_PLAY_TIMINGS_H
#define PROC_PLAY_TIMINGS_H


#include "lib/error.hpp"
//#include "lib/handle.hpp"
#include "lib/time/timevalue.hpp"
//#include "proc/engine/buffer-provider.hpp"
//#include "lib/iter-source.hpp"
//#include "lib/sync.hpp"

//#include <boost/noncopyable.hpp>
//#include <string>
//#include <vector>
#include <boost/rational.hpp>
#include <tr1/memory>
//#include <boost/scoped_ptr.hpp>

namespace lib {
namespace time{
  class Quantiser;
  typedef std::tr1::shared_ptr<const Quantiser> PQuant;
}}

namespace proc {
namespace play {

  using lib::time::FrameRate;
  using lib::time::TimeValue;
  using lib::time::Duration;
  using lib::time::Offset;
  using lib::time::Time;
//using std::string;

//using std::vector;
//using std::tr1::shared_ptr;
//using boost::scoped_ptr;
  
  enum PlaybackUrgency {
    ASAP,
    NICE,
    TIMEBOUND
  };
  
  
  
  /*****************************************************************************
   * Generic frame timing specification. Defines the expected delivery interval,
   * optionally also the expected quality-of-service
   * 
   * @note copyable value class
   * 
   * @todo write type comment
   *          /////////////////////////////////////////////////////////////////TODO: WIP seems to become some kind of abstracted Grid representation!
   */
  class Timings
    {
      lib::time::PQuant grid_;
      
    public:
      PlaybackUrgency playbackUrgency;
      boost::rational<int64_t> playbackSpeed;                     /////////////TICKET #902 we need a more generic representation for variable speed playback
      Duration outputLatency;
      
      Timings (FrameRate fps);
      
      // default copy acceptable
      
      TimeValue getOrigin()  const;
      
      Offset   getFrameOffsetAt   (TimeValue refPoint)  const;
      Duration getFrameDurationAt (TimeValue refPoint)  const;
      Duration getFrameDurationAt (int64_t refFrameNr)  const;
      
      /** the frame spacing and duration remains constant for some time...
       * @param startPoint looking from that time point into future
       * @return duration after this starting point, where it's safe
       *         to assume unaltered frame dimensions */
      Duration constantFrameTimingsInterval (TimeValue startPoint)  const;
      
      /** for scheduled time of delivery, which is signalled
       *  by \code playbackUrgency == TIMEBOUND \endcode
       * @return wall clock time to expect delivery of data corresponding
       *         to \link #getOrigin time axis origin \endlink
       * @note for other playback urgencies \c Time::NEVER
       */
      Time getTimeDue()  const;
      
      /** calculate the given frame's distance from origin,
       *  but do so using the real time scale, including any
       *  playback speed factory and similar corrections.
       * @param frameOffset frame number relative to the implicit grid
       * @return real time value relative to the implicit grid's zero point
       * @note since the Timings don't contain any information relating the
       *       nominal time scale to wall clock time, this result is just
       *       a relative offset, but expressed in real time scale values
       * @see proc::engine::TimeAnchor for an absolutely anchored conversion
       */
      Offset getRealOffset (int64_t frameOffset)  const;
      
      /** number of jobs to be planned and scheduled in one sway.
       *  The continuous planning of additional frame calculation jobs
       *  for playback or rendering proceeds in chunks of jobs
       *  controlled by this chunk size.
       */
      uint getPlanningChunkSize() const;
      
      
      bool isOriginalSpeed()  const;
      
      
      //////////////TODO further accessor functions here
      
      Timings constrainedBy (Timings additionalConditions);
      
    };
  
  
  
  
  
  inline bool
  Timings::isOriginalSpeed()  const
  {
    return 1 == playbackSpeed;
  }
  
  
  
}} // namespace proc::play
#endif
