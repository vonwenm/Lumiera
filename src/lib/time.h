/*
  time.h  -  Utilities for handling time

  Copyright (C)         Lumiera.org
    2008,               Christian Thaeter <ct@pipapo.org>

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

/** @file time.h
 ** Common functions for handling of time values.
 ** Working with time values in sexagesimal format, quantising time and converting
 ** to/from common timecode formats can be tricky to get right. Thus the goal is
 ** to concentrate the actual bits of math for these operations into a small number
 ** of library functions, which are easy to test thoroughly in isolation.
 ** 
 ** Built on top of that, the actual time handling in the GUI and within the Lumiera
 ** session is mostly confined to use the opaque lib::time::Time wrapper objects.
 ** When time values actually need to be \em quantised (aligned to a frame grid),
 ** this is expressed at the API through using the lib::time::QuTime type, which
 ** then in turn can be materialised into a number of \em timecode formats.
 ** These definitions ensure that whenever an actual quantisation (rounding)
 ** operation is performed, the link to the appropriate time grid is available,
 ** so that multiple output or rendering operations can use differing time origins
 ** and frame rates simultaneously on the same model.
 ** 
 ** The Lumiera backend functions mostly operate on raw frame counts, which in this
 ** model are defined to be a special kind of timecode, and thus dependent on a
 ** preceding time quantisation.
 ** 
 ** 
 ** @see lib::time::Time
 ** @see timequant.hpp
 ** @see TimeValue_test
 ** 
 */


#ifndef LUMIERA_TIME_H
#define LUMIERA_TIME_H

#include <inttypes.h>
#include <gavl/gavl.h>

#ifdef __cplusplus /*=================== C++ facilities ===================== */
#include "lib/time/timevalue.hpp"


/**
 * Converts a fraction of seconds to Lumiera's internal opaque time scale.
 * @param fractionalSeconds given as rational number
 * @note inconsistent with Lumiera's general quantisation behaviour,
 *       here negative fractional micro-ticks are truncated towards zero.
 *       This was deemed irrelevant in practice.
 */
gavl_time_t
lumiera_rational_to_time (lib::time::FSecs const& fractionalSeconds);


/**
 * Calculates the duration of one frame in Lumiera time units.
 * @param framerate underlying framerate as rational number
 * @throw error::Logic on zero framerate
 */
gavl_time_t
lumiera_frame_duration (lib::time::FrameRate const& fps);





extern "C" {    /* ===================== C interface ======================== */
#endif


/**
 * Formats a time value in H:MM:SS.mmm format into a temporary buffer.
 * @return safeclib temporary buffer containing formatted time string
 */
char*
lumiera_tmpbuf_print_time (gavl_time_t time);

/**
 * Quantise the given time into a fixed grid, relative to the origin.
 * The time grid used for quantisation is comprised of equally spaced intervals,
 * rooted at the given origin. The interval starting with the origin is numbered
 * as zero. Each interval includes its lower bound, but excludes its upper bound.
 * @param grid spacing of the grid intervals, measured in GAVL_TIME_SCALE
 * @return number of the grid interval containing the given time.
 * @warning the resulting value is limited to (Time::Min, Time::MAX) 
 */
long
lumiera_quantise_frames (gavl_time_t time, gavl_time_t origin, gavl_time_t grid);

/**
 * Similar to #lumiera_quantise_frames, but returns a grid aligned \em time value
 * @return time of start of the grid interval containing the given time, 
 *         but measured relative to the origin
 * @warning because the resulting value needs to be limited to fit into a 64bit long,
 *         the addressable time range can be considerably reduced. For example, if
 *         origin = Time::MIN, then all original time values above zero will be
 *         clipped, because the result, relative to origin, needs to be <= Time::MAX 
 */
gavl_time_t
lumiera_quantise_time (gavl_time_t time, gavl_time_t origin, gavl_time_t grid);

/**
 * Builds a time value by summing up the given components.
 */
gavl_time_t
lumiera_build_time (long millis, uint secs, uint mins, uint hours);

/**
 * Extract the hour part of given time.
 */
int lumiera_time_hours(gavl_time_t time);

/**
 * Extract the minute part of given time.
 */
int lumiera_time_minutes(gavl_time_t time);

/**
 * Extract the seconds part of given time.
 */
int lumiera_time_seconds(gavl_time_t time);

/**
 * Extract the milliseconds part of given time.
 */
int lumiera_time_millis(gavl_time_t time);



#ifdef __cplusplus
}//extern "C"
#endif
#endif
