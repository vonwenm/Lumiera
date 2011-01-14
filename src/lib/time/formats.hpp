/*
  FORMATS.hpp  -  formats for displaying and specifying time

  Copyright (C)         Lumiera.org
    2010,               Hermann Vosseler <Ichthyostega@web.de>

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


#ifndef LIB_TIME_FORMATS_H
#define LIB_TIME_FORMATS_H

#include "lib/time/timevalue.hpp"

//#include <boost/operators.hpp>
#include <tr1/memory>
#include <string>


namespace lib {
namespace time {
  
  
  // ====== forward declarations of concrete Timecode types
  
  class FrameNr;
  class SmpteTC;
  class HmsTC;
  class Secs;
  
  
  class Quantiser; // API for grid aligning
  typedef Quantiser const& QuantR;
  typedef std::tr1::shared_ptr<const Quantiser> PQuant;
  
  
  
  /**
   * descriptor for a time representation format (ABC).
   * A time format describes how to specify time; it allows
   * to format a time value and to parse a textual representation.
   * @note while Format is an generic descriptor, the actual
   *       TCode (timecode) values are time values, which \em
   *       use a specific format, given as template parameter
   * 
   * @todo WIP-WIP-WIP
   */
  class Format
    {
      
    public:
      virtual ~Format();
    };
  
  
  namespace format {
    
    /** 
     * Frame count as timecode format.
     * An integral number used to count frames
     * can be used as a simple from of time code.
     * Indeed the Lumiera backend mostly relies on
     * these frame counts. As with any timecode, the
     * underlying framerate/quantisation remains implicit.
     */
    struct Frames
      : Format
      {
        static void       rebuild (FrameNr&, QuantR, TimeValue const&);
        static TimeValue evaluate (FrameNr const&, QuantR);
      };
    
    
    /**
     * Widely used standard media timecode format.
     * A SMPTE timestamp addresses individual frames,
     * by specifying time as hour-minute-second plus the
     * frame number within the actual second.
     */
    struct Smpte
      : Format
      {
        static void       rebuild (SmpteTC&, QuantR);
        static TimeValue evaluate (SmpteTC const&, QuantR);
      };
    
    
    /**
     * The informal hours-minutes-seconds-millisecond timecode.
     * As such, this timecode is quantisation agnostic, but usually
     * it is used to address some frame or block or otherwise quantised
     * entity in time. HMS-Timecode is similar to SMPTE, but uses a
     * floating point milliseconds value instead of the frame count
     */
    struct Hms
      : Format
      {
        static void       rebuild (HmsTC&, QuantR);
        static TimeValue evaluate (HmsTC const&, QuantR);
      };
    
    
    /**
     * Simple timecode specification as fractional seconds.
     * Similar to HMS, a specification of seconds is quantisation agnostic,
     * but usually some implicit quantisation is used anyway, be it on actual
     * data frames, audio frames, or just on some smaller time interval, e.g.
     * full milliseconds.
     * @note Seconds is implemented as rational number and thus uses
     *       decimal format, not the usual sexagesimal time format
     */
    struct Seconds
      : Format
      {
        static void       rebuild (Secs&, QuantR);
        static TimeValue evaluate (Secs const&, QuantR);
      };
    
    
    
    
    
    template<class FMT>
    struct Traits;
    
    template<>
    struct Traits<Frames>
      {
        typedef FrameNr TimeCode;
      };
    
    template<>
    struct Traits<Smpte>
      {
        typedef SmpteTC TimeCode;
      };
    
    template<>
    struct Traits<Hms>
      {
        typedef HmsTC TimeCode;
      };
    
    template<>
    struct Traits<Seconds>
      {
        typedef Secs TimeCode;
      };
    
    
    
}}} // lib::time::format
#endif
