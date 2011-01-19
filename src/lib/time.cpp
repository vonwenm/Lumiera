/*
  Time  -  Utilities for handling time

  Copyright (C)         Lumiera.org
    2011,               Christian Thaeter <ct@pipapo.org>

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

#include "lib/time.h"
#include "lib/error.hpp"
#include "lib/util.hpp"

extern "C" {
#include "lib/tmpbuf.h"
}

#include <limits.h>
#include <math.h>

using util::floordiv;
using lib::time::FSecs;
using lib::time::FrameRate;
using boost::rational_cast;

namespace error = lumiera::error;




/* GAVL_TIME_SCALE is the correct factor or dividend when using gavl_time_t for
 * units of whole seconds from gavl_time_t.  Since we want to use milliseconds,
 * we need to multiply or divide by 1000 to get correct results. */
#define GAVL_TIME_SCALE_MS (GAVL_TIME_SCALE / 1000)

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
lumiera_frame_duration (FrameRate const& fps)
{
  if (!fps)
    throw error::Logic ("Impossible to quantise to an zero spaced frame grid"
                       , error::LUMIERA_ERROR_BOTTOM_VALUE);
  
  FSecs duration = rational_cast<FSecs> (1/fps);
  return lumiera_rational_to_time (duration);
}


namespace { // implementation helper
  
  inline long
  calculate_quantisation (gavl_time_t time, gavl_time_t origin, gavl_time_t grid)
  {
    time -= origin;
    return floordiv (time,grid);
  }
}


long
lumiera_quantise_frames (gavl_time_t time, gavl_time_t origin, gavl_time_t grid)
{
  return calculate_quantisation (time, origin, grid);
}

gavl_time_t
lumiera_quantise_time (gavl_time_t time, gavl_time_t origin, gavl_time_t grid)
{
  int64_t count = calculate_quantisation (time, origin, grid);
  gavl_time_t alignedTime = count * grid;
  return alignedTime;
}

gavl_time_t
lumiera_time_of_gridpoint (long nr, gavl_time_t origin, gavl_time_t grid)
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
lumiera_build_time_fps (float fps, uint frames, uint secs, uint mins, uint hours)
{
  gavl_time_t time = frames * (1000.0 / fps)
                   + 1000 * secs
                   + 1000 * 60 * mins
                   + 1000 * 60 * 60 * hours;
  time *= GAVL_TIME_SCALE_MS;
  return time;
}

gavl_time_t
lumiera_build_time_ntsc_drop (uint frames, uint secs, uint mins, uint hours)
{
  int total_mins = 60 * hours + mins;
  int total_frames  = 108000 * hours
                    + 1800 * mins
                    + 30 * secs
                    + frames
                    - 2 * (total_mins - total_mins / 10);
  
  return (total_frames / 29.97f) * 1000 * GAVL_TIME_SCALE_MS;
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
lumiera_time_frames (gavl_time_t time, float fps)
{
  return (fps * (lumiera_time_millis(time))) / 1000;
}

int
lumiera_time_frame_count (gavl_time_t time, float fps)
{
  REQUIRE (fps > 0);
  
  return roundf((time / GAVL_TIME_SCALE_MS / 1000.0f) * fps);
}

/**
 * This function is used in the NTSC drop-frame functions to avoid code
 * repetition.
 * @return the frame number for given time
 */
static int
ntsc_drop_get_frame_number (gavl_time_t time)
{
  int frame = lumiera_time_frame_count (time, NTSC_DROP_FRAME_FPS);
  
  int d = frame / 17982; // 17982 = 600 * 29.97
  int m = frame % 17982; // 17982 = 600 * 29.97
  
  return frame + 18*d + 2*((m - 2) / 1798); // 1798 = 60 * 29.97
}

int
lumiera_time_ntsc_drop_frames (gavl_time_t time)
{
  return ntsc_drop_get_frame_number(time) % 30;
}

int
lumiera_time_ntsc_drop_seconds (gavl_time_t time)
{
  return ntsc_drop_get_frame_number(time) / 30 % 60;
}

int
lumiera_time_ntsc_drop_minutes (gavl_time_t time)
{
  return ntsc_drop_get_frame_number(time) / 30 / 60 % 60;
}

int
lumiera_time_ntsc_drop_hours (gavl_time_t time)
{
  return ntsc_drop_get_frame_number(time) / 30 / 60 / 60 % 24;
}

