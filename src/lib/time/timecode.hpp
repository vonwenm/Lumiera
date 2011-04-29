/*
  TIMECODE.hpp  -  grid aligned and fixed format time specifications

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


#ifndef LIB_TIME_TIMECODE_H
#define LIB_TIME_TIMECODE_H

#include "lib/time/timevalue.hpp"
#include "lib/time/formats.hpp"
#include "lib/time/digxel.hpp"
#include "lib/symbol.hpp"

//#include <iostream>
#include <boost/operators.hpp>
#include <boost/lexical_cast.hpp> ///////////////TODO
#include <string>


namespace lib {
namespace time {
  
  using std::string;
  using lib::Literal;
  using boost::lexical_cast; /////////TODO
  
  
  /**
   * Interface: fixed format timecode specification.
   * @see time::Format
   * @todo WIP-WIP-WIP
   */
  class TCode
    {
      
    public:
      virtual ~TCode();
      
      operator string()  const { return show(); }
      string describe()  const { return string(tcID()); }
      Time   getTime()   const { return Time(value()); }
      
    protected:
      TCode (PQuant const& quant)
        : quantiser_(quant)
        { }
      
      virtual string show()     const   =0;
      virtual Literal tcID()    const   =0;
      virtual TimeValue value() const   =0;
      
    protected:
      PQuant quantiser_;
    };
  
  
  class QuTime;
  
  /**
   * A frame counting timecode value.
   * This is an hard-coded representation of
   * TCode<format::Frames>, with additional convenience
   * constructors and conversions, which basically make
   * FrameNr values interchangeable with integral numbers. 
   */
  class FrameNr
    : public TCode
    , public CountVal
    {
      
      string show()     const { return string(CountVal::show())+"fr"; }
      Literal tcID()    const { return "Frame-count"; } 
      TimeValue value() const { return Format::evaluate (*this, *quantiser_); }
      
    public:
      typedef format::Frames Format;
      
      FrameNr (QuTime const& quantisedTime);
      
      // CountVal implicitly convertible to long
    };
  
  
  
  /**
   * Classical Timecode value reminiscent to SMPTE format.
   * After quantisation, the resulting (materialised) time value is
   * decimated into a hours, a minutes, a seconds part and the remainder
   * is cast into a frame number relative to the seconds. Consequently,
   * a SmpteTC representation is always linked implicitly to a specific framerate.
   * 
   * \par range extensions
   * Historically, SMPTE timecode format was focused mainly on how to encode a
   * unique timestamp in a way allowing to 'piggyback' these timestamps into an
   * existing (analogue) media data format. As a side effect, quite tight limits
   * had to be imposed on the possible component values in such a fixed-length format.
   * This whole concern is completely beyond the scope of a typical computer based video
   * implementation; thus we can consider ways to extend the value range to be represented
   * in this SMPTE-like timecode format:
   * - we can allow time values below the zero point
   * - we can allow time values beyond 24 hours.
   * Several different schemes how to do this extensions could be devised (and in addition,
   * we could also wrap around the hours field, jumping from 23:59:59:## to 0:0:0:0).
   * Currently, we implement an extension, where the timecode representation is symmetrical
   * to the zero point and the hours field is just extended beyond 23 hours. To give an
   * example: \c 0:0:0:0 minus 1 frame yields \c -0:0:0:1
   * 
   * @todo the range extension scheme could be a configurable strategy
   */
  class SmpteTC
    : public TCode
    , boost::unit_steppable<SmpteTC>
    {
      uint effectiveFramerate_;
      
      virtual string show()     const ;
      virtual Literal tcID()    const { return "SMPTE"; }
      virtual TimeValue value() const { return Format::evaluate (*this, *quantiser_); }
      
      
    public:
      typedef format::Smpte Format;
      
      SmpteTC (QuTime const& quantisedTime);
      SmpteTC (SmpteTC const&);
      SmpteTC& operator= (SmpteTC const&);
      
      uint getFps()   const;
      
      void clear();
      void rebuild();
      void invertOrientation();

      
      HourDigit hours;
      SexaDigit mins;
      SexaDigit secs;
      SexaDigit frames;
      Signum    sgn;
      
      SmpteTC& operator++();
      SmpteTC& operator--();
    };
  
  
  
  /**
   * @warning missing implementation
   */
  class HmsTC
    : public TCode
    {
      TimeVar tpoint_;
      
      virtual string show()     const { return string(tpoint_); }
      virtual Literal tcID()    const { return "Timecode"; }        
      virtual TimeValue value() const { return tpoint_; }        
      
    public:
      typedef format::Hms Format;
      
      HmsTC (QuTime const& quantisedTime);
      
      double getMillis () const;
      int getSecs      () const; 
      int getMins      () const; 
      int getHours     () const; 
    };
  
  
  
  /**
   * @warning partially missing implementation
   */
  class Secs
    : public TCode
    {
      FSecs sec_;
      
      virtual string show()     const { return string(Time(sec_)); }
      virtual Literal tcID()    const { return "Seconds"; }
      virtual TimeValue value() const { return Time(sec_); }
      
    public:
      typedef format::Seconds Format;
      
      Secs (QuTime const& quantisedTime);
      
      operator FSecs()  const;
    };
  
    
  /** writes time value, formatted as HH:MM:SS:mmm */
  

  
}} // lib::time
#endif
