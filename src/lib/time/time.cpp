/*
  Time  -  Lumiera time handling foundation

  Copyright (C)         Lumiera.org
    2008,               Christian Thaeter <ct@pipapo.org>
    2010,               Stefan Kangas <skangas@skangas.se>
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


/** @file time.cpp 
 ** Lumiera time handling core implementation unit.
 ** This translation unit generates code for the Lumiera internal time wrapper,
 ** based on gavl_time_t, associated constants, marker classes for the derived
 ** time entities (TimeVar, Offset, Duration, TimeSpan, FrameRate) and for the
 ** basic time and frame rate conversion functions.
 ** 
 ** Client code includes either time.h (for basics and conversion functions)
 ** or timevalue.hpp (for the time entities), timequant.hpp for grid aligned
 ** time values or timecode.hpp 
 ** 
 ** @see Time
 ** @see TimeValue
 ** @see Grid
 ** @see TimeValue_test
 ** @see QuantiserBasics_test
 **
 */


#include "lib/error.hpp"
#include "lib/time.h"
#include "lib/time/timevalue.hpp"
#include "lib/util-quant.hpp"

extern "C" {
#include "lib/tmpbuf.h"
}

#include <math.h>
#include <limits>
#include <string>
#include <boost/rational.hpp>
#include <boost/lexical_cast.hpp>

using std::string;
using util::floordiv;
using lib::time::FSecs;
using lib::time::FrameRate;
using boost::rational_cast;
using boost::lexical_cast;

namespace error = lumiera::error;




/* GAVL_TIME_SCALE is the correct factor or dividend when using gavl_time_t for
 * units of whole seconds from gavl_time_t.  Since we want to use milliseconds,
 * we need to multiply or divide by 1000 to get correct results. */
#define GAVL_TIME_SCALE_MS (GAVL_TIME_SCALE / 1000)


namespace lib {
namespace time {
  
  
  
  /** @note the allowed time range is explicitly limited to help overflow protection */
  const Time Time::MAX ( TimeValue::buildRaw_(+std::numeric_limits<gavl_time_t>::max() / 30) );
  const Time Time::MIN ( TimeValue::buildRaw_(-_raw(Time::MAX)                             ) );
  const Time Time::ZERO;
  
  const Time Time::ANYTIME(Time::MAX);
  const Time Time::NEVER  (Time::MIN);
  
  const Offset Offset::ZERO (Time::ZERO);
  
  
  
  /** convenience constructor to build an
   *  internal Lumiera Time value from the usual parts
   *  of an sexagesimal time specification. Arbitrary integral
   *  values are acceptable and will be summed up accordingly.
   *  The minute and hour part can be omitted.
   * @warning internal Lumiera time values refer to an
   *         implementation dependent time origin/scale.
   *         The given value will be used as-is, without
   *         any further adjustments.
   */
  Time::Time ( long millis
             , uint secs 
             , uint mins
             , uint hours
             )
    : TimeValue(lumiera_build_time (millis,secs,mins,hours))
    { }
  
  
  /** convenience constructor to build an Time value
   *  from a fraction of seconds, given as rational number.
   *  An example would be to the time unit of a framerate.
   */
  Time::Time (FSecs const& fractionalSeconds)
    : TimeValue(lumiera_rational_to_time (fractionalSeconds))
    { }
  
  
  /** displaying an internal Lumiera Time value
   *  for diagnostic purposes or internal reporting.
   * @warning internal Lumiera time values refer to an
   *         implementation dependent time origin/scale.
   * @return string rendering of the actual, underlying
   *         implementation value, as \c h:m:s:ms
   */
  Time::operator string()  const
  {
    return string (lumiera_tmpbuf_print_time (t_));
  }
  
  TimeVar::operator string()  const
  {
    return string (lumiera_tmpbuf_print_time (t_));
  }
  
  
  /** @internal backdoor to sneak in a raw time value
   *   bypassing any normalisation and limiting */
  TimeValue
  TimeValue::buildRaw_ (gavl_time_t raw)
  {
    return reinterpret_cast<TimeValue const&> (raw);
  }
  
  
  
  /** predefined constant for PAL framerate */
  const FrameRate FrameRate::PAL  (25);
  const FrameRate FrameRate::NTSC (30000,1001);
  
  const FrameRate FrameRate::HALTED (1,std::numeric_limits<int>::max());
  
  
  /** @return time span of one frame of this rate,
   *   cast into internal Lumiera time scale */
  Duration
  FrameRate::duration() const
  {
    if (0 == *this)
      throw error::Logic ("Impossible to quantise to an zero spaced frame grid"
                         , error::LUMIERA_ERROR_BOTTOM_VALUE);
    
    return Duration (1, *this);
  }
  
  
  /** visual framerate representation (for diagnostics) */
  FrameRate::operator string() const
  {
    return 1==denominator() ? lexical_cast<string> (numerator())+"FPS"
                            : lexical_cast<string> (numerator())
                               + "/"
                               + lexical_cast<string> (denominator())
                               + "FPS";
  }

  
  
  Offset
  operator* (boost::rational<int64_t> factor, Offset const& o)
  {
    boost::rational<int64_t> distance (_raw(o));
    distance *= factor;
    gavl_time_t microTicks = floordiv (distance.numerator(), distance.denominator());
    return Offset(TimeValue(microTicks));
  }
  
  
  /** offset by the given number of frames. */
  Offset::Offset (FrameCnt count, FrameRate const& fps)
    : TimeValue (count?  (count<0? -1:+1) * lumiera_framecount_to_time (::abs(count), fps)
                      : _raw(Duration::NIL))
    { }
  
  /** duration of the given number of frames.
   * @note always positive; count used absolute */
  Duration::Duration (FrameCnt count, FrameRate const& fps)
    : TimeValue (count? lumiera_framecount_to_time (abs(count), fps) : _raw(Duration::NIL))
    { }
  
  
  /** constant to indicate "no duration" */
  const Duration Duration::NIL (Time::ZERO);
  
  
}} // namespace lib::Time








/* ===== implementation of the C API functions ===== */


char*
lumiera_tmpbuf_print_time (gavl_time_t time)
{
  int milliseconds, seconds, minutes, hours;
  int negative;
  
  if(time < 0)
    {
      negative = 1;
      time = -time;
    }
  else negative = 0;
  
  time /= GAVL_TIME_SCALE_MS;
  milliseconds = time % 1000;
  time /= 1000;
  seconds = time % 60;
  time /= 60;
  minutes = time % 60;
  time /= 60;
  hours = time;
  
  char *buffer = lumiera_tmpbuf_snprintf(64, "%s%01d:%02d:%02d.%03d",
    negative ? "-" : "", hours, minutes, seconds, milliseconds);
    
  ENSURE(buffer != NULL);
  return buffer;
}

gavl_time_t
lumiera_rational_to_time (FSecs const& fractionalSeconds)
{
  return rational_cast<gavl_time_t> (GAVL_TIME_SCALE * fractionalSeconds);
}

gavl_time_t
lumiera_framecount_to_time (uint64_t frameCount, FrameRate const& fps)
{
  // convert to 64bit
  boost::rational<uint64_t> framerate (fps.numerator(), fps.denominator());
  
  return rational_cast<gavl_time_t> (GAVL_TIME_SCALE * frameCount / framerate);
}

gavl_time_t
lumiera_frame_duration (FrameRate const& fps)
{
  if (!fps)
    throw error::Logic ("Impossible to quantise to an zero spaced frame grid"
                       , error::LUMIERA_ERROR_BOTTOM_VALUE);
  
  FSecs duration = rational_cast<FSecs> (1/fps);
  return lumiera_rational_to_time (duration);
}


namespace { // implementation: basic frame quantisation....
  
  inline int64_t
  calculate_quantisation (gavl_time_t time, gavl_time_t origin, gavl_time_t grid)
  {
    time -= origin;
    return floordiv (time,grid);
  }
  
  inline int64_t
  calculate_quantisation (gavl_time_t time, gavl_time_t origin, uint framerate, uint framerate_divisor=1)
  {
    REQUIRE (framerate);
    REQUIRE (framerate_divisor);
    
    const int64_t limit_num = std::numeric_limits<gavl_time_t>::max() / framerate;
    const int64_t limit_den = std::numeric_limits<gavl_time_t>::max() / framerate_divisor;
    const int64_t microScale(GAVL_TIME_SCALE);
    
    // protect against numeric overflow 
    if (abs(time) < limit_num && microScale < limit_den)
      {
        // safe to calculate "time * framerate"
        time -= origin;
        return floordiv (time*framerate, microScale*framerate_divisor);
      }
    else
      {
        // direct calculation will overflow.
        // use the less precise method instead...
        gavl_time_t frameDuration = microScale / framerate; // truncated to µs
        return calculate_quantisation (time,origin, frameDuration);
      }
  }
}


int64_t
lumiera_quantise_frames (gavl_time_t time, gavl_time_t origin, gavl_time_t grid)
{
  return calculate_quantisation (time, origin, grid);
}

int64_t
lumiera_quantise_frames_fps (gavl_time_t time, gavl_time_t origin, uint framerate)
{
  return calculate_quantisation (time,origin,framerate);
}

gavl_time_t
lumiera_quantise_time (gavl_time_t time, gavl_time_t origin, gavl_time_t grid)
{
  int64_t count = calculate_quantisation (time, origin, grid);
  gavl_time_t alignedTime = count * grid;
  return alignedTime;
}

gavl_time_t
lumiera_time_of_gridpoint (int64_t nr, gavl_time_t origin, gavl_time_t grid)
{
  gavl_time_t offset = nr * grid;    
  return origin + offset;
}


gavl_time_t
lumiera_build_time(long millis, uint secs, uint mins, uint hours)
{
  gavl_time_t time = millis
                   + 1000 * secs
                   + 1000 * 60 * mins
                   + 1000 * 60 * 60 * hours;
  time *= GAVL_TIME_SCALE_MS;
  return time;
}

gavl_time_t
lumiera_build_time_fps (uint fps, uint frames, uint secs, uint mins, uint hours)
{
  gavl_time_t time = 1000LL * frames/fps
                   + 1000 * secs
                   + 1000 * 60 * mins
                   + 1000 * 60 * 60 * hours;
  time *= GAVL_TIME_SCALE_MS;
  return time;
}

int
lumiera_time_hours (gavl_time_t time)
{
  return time / GAVL_TIME_SCALE_MS / 1000 / 60 / 60;
}

int
lumiera_time_minutes (gavl_time_t time)
{
  return (time / GAVL_TIME_SCALE_MS / 1000 / 60) % 60;
}

int
lumiera_time_seconds (gavl_time_t time)
{
  return (time / GAVL_TIME_SCALE_MS / 1000) % 60;
}

int
lumiera_time_millis (gavl_time_t time)
{
  return (time / GAVL_TIME_SCALE_MS) % 1000;
}

int
lumiera_time_frames (gavl_time_t time, uint fps)
{
  REQUIRE (fps < uint(std::numeric_limits<int>::max()));
  return floordiv (lumiera_time_millis(time) * int(fps), GAVL_TIME_SCALE_MS);
}




/* ===== NTSC drop-frame conversions ===== */


namespace { // implementation helper
  
  const uint FRAMES_PER_10min = 10*60 * 30000/1001;
  const uint FRAMES_PER_1min  =  1*60 * 30000/1001;
  const uint DISCREPANCY      = (1*60 * 30) - FRAMES_PER_1min;
   
  
  /** reverse the drop-frame calculation
   * @param  time absolute time value in micro ticks 
   * @return the absolute frame number using NTSC drop-frame encoding
   * @todo I doubt this works correct for negative times!! 
   */
  inline int64_t
  calculate_drop_frame_number (gavl_time_t time)
  {
    int64_t frameNr = calculate_quantisation (time, 0, 30000, 1001);
    
    // partition into 10 minute segments
    lldiv_t tenMinFrames = lldiv (frameNr, FRAMES_PER_10min);
    
    // ensure the drop-frame incidents happen at full minutes;
    // at start of each 10-minute segment *no* drop incident happens,
    // thus we need to correct discrepancy between nominal/real framerate once: 
    int64_t remainingMinutes = (tenMinFrames.rem - DISCREPANCY) / FRAMES_PER_1min;
    
    int64_t dropIncidents = (10-1) * tenMinFrames.quot + remainingMinutes;
    return frameNr + 2*dropIncidents;
  }
}

int
lumiera_time_ntsc_drop_frames (gavl_time_t time)
{
  return calculate_drop_frame_number(time) % 30;
}

int
lumiera_time_ntsc_drop_seconds (gavl_time_t time)
{
  return calculate_drop_frame_number(time) / 30 % 60;
}

int
lumiera_time_ntsc_drop_minutes (gavl_time_t time)
{
  return calculate_drop_frame_number(time) / 30 / 60 % 60;
}

int
lumiera_time_ntsc_drop_hours (gavl_time_t time)
{
  return calculate_drop_frame_number(time) / 30 / 60 / 60 % 24;
}

gavl_time_t
lumiera_build_time_ntsc_drop (uint frames, uint secs, uint mins, uint hours)
{
  uint64_t total_mins = 60 * hours + mins;
  uint64_t total_frames  = 30*60*60 * hours
                         + 30*60 * mins
                         + 30 * secs
                         + frames
                         - 2 * (total_mins - total_mins / 10);
  gavl_time_t result = lumiera_framecount_to_time (total_frames, FrameRate::NTSC);
  
  if (0 != result) // compensate for truncating down on conversion
    result += 1;  //  without this adjustment the frame number  
  return result; //   would turn out off by -1 on back conversion
}
