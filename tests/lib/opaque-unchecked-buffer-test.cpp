/*
  OpaqueUncheckedBuffer(Test)  -  passive inline buffer test
 
  Copyright (C)         Lumiera.org
    2009,               Hermann Vosseler <Ichthyostega@web.de>
 
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



#include "lib/test/run.hpp"
#include "lib/test/test-helper.hpp"
#include "lib/util.hpp"

#include "lib/opaque-holder.hpp"

#include <boost/noncopyable.hpp>
#include <iostream>
#include <cstring>

namespace lib {
namespace test{
  
  using ::Test;
  using util::min;
  using lumiera::error::LUMIERA_ERROR_FATAL;
  
  using boost::noncopyable;
  using std::strlen;
  using std::cout;
  using std::endl;
  
  
  
  namespace { // test dummy hierarchy
             //  Note: vtable (and virtual dtor), but varying storage requirements 
    
    long _checksum = 0;
    uint _create_count = 0;
    
    
    struct Base
      : noncopyable
      {
        uint id_;
          
        Base(uint i) : id_(i)     { ++_create_count; _checksum += id_; }
        
        virtual ~Base()           { }
        
        virtual void confess()  =0;
      };
    
    
    template<uint ii>
    struct DD : Base
      {
        char buff_[ii+1];
      
       ~DD() { _checksum -= ii; }  // verify the correct dtor is called...
       
        DD(Symbol sym = 0)
          : Base(ii)
          {
            memset (&buff_, '*', ii);
            if (sym)
              memcpy (&buff_, sym, min (ii, strlen (sym)));
            buff_[ii] = 0;
          }
        
        void
        confess ()
          {
            cout << "DD<" << ii << ">: " << buff_ << endl;
          }
      };  
    
    
    struct D42Sub
      : DD<42>
      {
        D42Sub (string s1, string s2)
          : DD<42> ((s1+' '+s2).c_str())
          { }
        
        void
        confess ()
          {
            cout << "I'm special, " << buff_ << endl;
          }
      };
    
    
    struct Killer
      : DD<23>
      {
        Killer () { throw lumiera::error::Fatal ("crisscross"); }
      };
    
    
    
    /** maximum additional storage maybe wasted
     *  due to alignment of the contained object
     *  within OpaqueHolder's buffer
     */
    const size_t _ALIGN_ = sizeof(size_t);
    
  }
  
  
  
  /**********************************************************************************
   *  @test use an inline buffer to place objects of a subclass, without any checks.
   *        InPlaceBuffer only provides minimal service, to be covered here, 
   *        including automatic dtor invocation and smart-ptr style access.
   */
  class OpaqueUncheckedBuffer_test : public Test
    {
      
      virtual void
      run (Arg)
        {
          _checksum = 0;
          _create_count = 0;
          {
            typedef InPlaceBuffer<Base, sizeof(DD<42>), DD<0> > Buffer;
            
            Buffer buff;
            CHECK (sizeof(buff) <= sizeof(DD<42>) + _ALIGN_);
            CHECK (1 == _create_count);
            CHECK (0 == _checksum);
            buff->confess();          // one default object of type DD<0> has been created
            
            buff.create<DD<5> > ();
            buff->confess();
            
            buff.create<DD<9> > ("I'm fine");
            buff->confess();
            
            VERIFY_ERROR( FATAL, buff.create<Killer> () );
            
            CHECK(0 == buff->id_);   // default object was created, due to exception...
            
            buff.create<D42Sub> ("what the f**","is going on here?");
            buff->confess();
            
            CHECK (6 == _create_count);
            CHECK (42 == _checksum); // No.42 is alive
          }
          CHECK (0 == _checksum);  // all dead
        }
    };
  
  
  LAUNCHER (OpaqueUncheckedBuffer_test, "unit common");
  
  
}} // namespace lib::test

