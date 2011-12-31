/*
  POLYMORPHIC-VALUE.hpp  -  building opaque polymorphic value objects 

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

/** @file polymorphic-value.hpp
 ** A mechanism to allow for opaque polymorphic value objects.
 ** This template helps to overcome a problem frequently encountered in
 ** C++ programming, based on the fundamental design of C++, which favours
 ** explicit low-level control, copying of values and strict ctor-dtor pairs.
 ** Many object oriented design patterns build on polymorphism, where the
 ** actual type of an object isn't disclosed and collaborations rely on
 ** common interfaces. This doesn't mix well with the emphasis the C/C++
 ** language puts on efficient handling of small data elements as values
 ** and explicit control of the used storage; indeed several of the modern
 ** object oriented and functional programming techniques more or less
 ** assume the presence of a garbage collector or similar mechanism,
 ** so 'objects' need just to be mentioned by reference.
 ** 
 ** In C++, in order to employ many of the well known techniques, we're bound
 ** more or less to explicitly put the objects somewhere in heap allocated memory
 ** and then pass an interface pointer or reference into the actual algorithm.
 ** Often, this hinders a design based on constant values and small descriptor
 ** objects used inline, thus forcing into unnecessarily complex and heavyweight
 ** alternatives. While it's certainly pointless to fight the fundamental nature
 ** of the programming language, we may try to pull some (template based) trickery
 ** to make polymorphic objects fit better with the handling of small copyable
 ** value objects. Especially, C++ gives a special meaning to passing parameters
 ** as \c const& -- typically constructing an anonymous temporary object conveniently
 ** just for passing an abstraction barrier (while the optimiser can be expected to
 ** remove this barrier and the accompanying nominal copy operations altogether in
 ** the generated code). Consequently the ability to return a polymorphic object
 ** from a factory or configuration function <i>by value</i> would open a lot of
 ** straight forward design possibilities and concise formulations.
 ** 
 ** \par how to build a copyable value without knowing it's layout in detail
 ** 
 ** So the goal is to build a copyable and assignable type with value semantics,
 ** without disclosing the actual implementation and object layout at the usage site.
 ** This seemingly contradictory goal can be achieved, provided that
 ** - the space occupied by the actual implementation object is limited,
 **   so it can be placed as binary data into an otherwise opaque holder buffer
 ** - and the actual implementation object assists with copying and cloning
 **   itself, observing the actual implementation data layout
 ** 
 ** The PolymorphicValue template implements this idea, by exposing an copyable
 ** container with value semantics to the client code. On instantiation, a common
 ** base interface for the actual value objects needs to be provided; the resulting
 ** instance will be automatically convertible to this interface. Obviously this
 ** common interface must be an ABC or at least contain some virtual functions.
 ** Moreover, the PolymorphicValue container provides static builder functions,
 ** allowing to place a concrete instance of a subclass into the content buffer.
 ** After construction, the actual type of this instance will be forgotten
 ** (``type erasure''), but because of the embedded vtable, on access the
 ** proper implementation functions will be invoked.
 ** 
 ** Expanding on that pattern, the copying and cloning operations of the whole
 ** container can be implemented by forwarding to appropriate virtual functions
 ** on the embedded payload (implementation) object -- the concrete implementation
 ** of these virtual functions can be assumed to know the real type and thus be
 ** able to invoke the correct copy ctor or assignment operator. For this to
 ** work, the interface needs to expose those copy and clone operations somehow
 ** as virtual functions. There are two alternatives to get at this point:
 ** - in the general case, the common base interface doesn't expose such operations.
 **   Thus we need to <i>mix in</i> an additional \em management interface; this
 **   can be done by \em subclassing the desired implementation type, because
 **   this concrete type is irrelevant after finishing the placement constructor.
 **   In order to re-access this management interface, so to be able to invoke
 **   the copy or clone operations, we need to do an elaborate re-cast operation,
 **   first going down to the leaf type and then back up into the mixed in
 **   management interface. Basically this operation is performed by using
 **   a \c dynamic_cast<CopyAPI&>(bufferContents)
 ** - but when the used client types provide some collaboration and implement
 **   this management interface either directly on the API or as an immediate
 **   sub-interface, then this copy/management interface is located within the
 **   direct inheritance chain and can be reached by a simple \c static_cast.
 **   Indeed, as we're just using a different meaning of the VTable, only a
 **   single indirection (virtual function call) is required at runtime in
 **   this case to invoke the copy ctor or assignment operator.
 ** Thus, in this latter (optimal) case, the fact that PolymorphicValue allows
 ** to conceal the actual implementation type comes with zero runtime overhead,
 ** compared with direct usage of a family of polymorphic types (with VTable).
 ** 
 ** \par using polymorphic value objects
 ** 
 ** To start with, we need a situation where polymorphic treatment and type erasure
 ** might be applicable. That is, we use a public API, and only that, in any client
 ** code, while the concrete implementation is completely self contained. Thus, in
 ** the intended use, the concrete implementation objects can be assembled once,
 ** typically in a factory, and after that, no further knowledge of the actual
 ** implementation type is required. All further use can be coded against
 ** the exposed public API.
 ** 
 ** Given such a situation, it might be desirable to conceal the whereabouts of
 ** the implementation completely from the clients employing the generated objects.
 ** For example, the actual implementation might rely on a complicated subsystem
 ** with many compilation dependencies, and we don't want to expose all those
 ** details on the public API.
 ** 
 ** Now, to employ PolymorphicValue in such a situation, on the usage side (header):
 ** - expose the public API, but not the implementation type of the objects
 ** - define an instantiation of PolymorphicValue with this API
 ** - be sure to define a hard wired size limit not to be exceeded by the
 **   actual implementation objects (PolymorphicValue's ctor has an assertion
 **   to verify this constraint)
 ** - provide some kind of factory for the clients to get the actual polymorphic
 **   value instances. Clients may then freely move and copy those objects, but
 **   do not need to know anything about the actual implementation object layout
 **   (it could be figured out using RTTI though)
 ** 
 ** On the implementation side (separate compilation unit)
 ** - include the definition of the PolymorphicValue instantiation (of course)
 ** - define the implementation types to inherit from the public API
 ** - implement the mentioned factory function, based on the static build
 **   PolymorphicValue#build functions, using the actual implementation type
 **   as parameter. 
 ** 
 ** @see polymorphic-value-test.cpp
 ** @see opaque-holder.hpp other similar opaque inline buffer templates
 ** @see lib::time::Mutation usage example
 */


#ifndef LIB_POLYMORPHIC_VALUE_H
#define LIB_POLYMORPHIC_VALUE_H


#include "lib/error.hpp"
#include "lib/meta/duck-detector.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/static_assert.hpp>


namespace lib {
  
  
  namespace polyvalue { // implementation details...
    
    namespace error = lumiera::error;
    using boost::enable_if;
    using lib::meta::Yes_t;
    using lib::meta::No_t;
    
    struct EmptyBase{ };
    
    /**
     * Interface for active support of copy operations
     * by the embedded client objects. When inserted into the
     * inheritance chain \em above the concrete implementation objects,
     * PolymorphicValue is able to perform copy operations trivially and
     * without any \c dynamic_cast and other run time overhead besides a
     * simple indirection through the VTable. To enable this support, the
     * implementation objects should inherit from \c CopySupport<Interface>
     * (where \c Interface would be the public API for all these embedded
     * implementation objects).
     * Alternatively, it's also possible to place this CopySupport API as parent
     * to the public API (it might even be completely absent, but then you'd need
     * to provide an explicit specialisation of the Traits template to tell
     * PolymorphicValue how to access the copy support functions.)
     */
    template<class IFA         ///< the common public interface of all embedded objects
            ,class BA = IFA    ///< direct baseclass to use for this copy support API
            >
    class CopySupport
      : public BA
      {
      public:
        virtual ~CopySupport() { };
        virtual void cloneInto (void* targetBuffer)  const   =0;
        virtual void copyInto  (IFA& targetBase)     const   =0;
      };
    
    /**
     * A variation for limited copy support.
     * Sometimes, only cloning (copy construction) of value objects is allowed,
     * but not assignment of new values to existing objects. In this case, the
     * concrete values can inherit from this variant of the support API.
     */
    template<class BA>         ///< direct baseclass to use for this clone support API
    class CloneValueSupport
      : public BA
      {
      public:
        virtual ~CloneValueSupport() { };
        virtual void cloneInto (void* targetBuffer)  const   =0;
      };
    
    
    
    /** 
     * helper to detect presence of a function
     * to support clone operations
     */
    template<typename T>
    class exposes_CloneFunction
      {
        
        META_DETECT_FUNCTION(void, cloneInto, (void*) const);
        
      public:
        enum{ value = HasFunSig_cloneInto<T>::value
            };
      };
    
    /**
     * helper to detect if the API supports only
     * copy construction, but no assignment
     */
    template<typename T>
    class allow_Clone_but_no_Copy
      {
        META_DETECT_MEMBER(copyInto);
        
      public:
        enum{ value = exposes_CloneFunction<T>::value
                 && ! HasMember_copyInto<T>::value
            };
      };
    
    /**
     * Policy class for invoking the assignment operator.
     * By default the embedded payload objects are assumed
     * to be freely assignable
     */
    template<class API, class YES =void>
    struct AssignmentPolicy
      {
        template<class IMP>
        static void 
        assignEmbedded(IMP& dest,IMP const& src)
          {
            dest = src;
          }
      };
    
    /**
     * special case when the embedded payload objects permit
     * copy construction, but no assignment to existing instances.
     * Instead of invoking the assignment operator (which typically
     * isn't defined at all in such cases), a misguided assignment
     * to the container will raise an exception
     */
    template<class API>
    struct AssignmentPolicy<API, typename enable_if< allow_Clone_but_no_Copy<API> >::type>
      {
        template<class IMP>
        static void 
        assignEmbedded(IMP&,IMP const&)
          {
            throw error::Logic("attempt to overwrite unmodifiable value");
          }
      };
    
    
    
    /**
     * traits template to deal with
     * different ways to support copy operations.
     * Default is no support by the API and implementation types.
     * In this case, the CopySupport interface is mixed in at the
     * level of the concrete implementation class and later on
     * accessed through an \c dynamic_cast
     */
    template <class TY, class YES = void> 
    struct Trait
      {
        typedef CopySupport<TY,EmptyBase> CopyAPI;
        enum{   ADMIN_OVERHEAD = 2 * sizeof(void*) };
        
        static CopyAPI&
        accessCopyHandlingInterface (TY& bufferContents)
          {
            REQUIRE (INSTANCEOF (CopyAPI, &bufferContents));
            return dynamic_cast<CopyAPI&> (bufferContents);
          }
        
        typedef CopyAPI AdapterAttachment;
        typedef AssignmentPolicy<CopyAPI> Assignment;
      };
    
    
    /**
     * Special case when the embedded types support copying
     * on the API level, e.g. there is a sub-API exposing a \c cloneInto
     * function. In this case, the actual implementation classes can be
     * instantiated as-is and the copy operations can be accessed by a
     * simple \c static_cast without runtime overhead.
     */
    template <class TY>
    struct Trait<TY, typename enable_if< exposes_CloneFunction<TY> >::type>
      {
        typedef TY CopyAPI;
        enum{   ADMIN_OVERHEAD = 1 * sizeof(void*) };
        
        template<class IFA>
        static CopyAPI&
        accessCopyHandlingInterface (IFA& bufferContents)
          {
            REQUIRE (INSTANCEOF (CopyAPI, &bufferContents));
            return static_cast<CopyAPI&> (bufferContents);
          }
        
        typedef EmptyBase AdapterAttachment;
        typedef AssignmentPolicy<CopyAPI> Assignment;
      };
    
  }//(End)implementation details
  
  
  
  
  
  
  
  
  
  /**
   * Template to build polymorphic value objects.
   * Inline buffer with value semantics, yet holding and owning an object
   * while concealing the concrete type, exposing only the public interface.
   * Access to the contained object is by implicit conversion to this public
   * interface. The actual implementation object might be placed into the
   * buffer through a builder function; later, this buffer may be copied
   * and passed on without knowing the actual contained type.
   * 
   * For using PolymorphicValue, several \b assumptions need to be fulfilled
   * - any instance placed into OpaqueHolder is below the specified maximum size
   * - the caller cares for thread safety. No concurrent get calls while in mutation!
   * 
   * @warning when a create or copy-into operation fails with exception, the whole
   *          PolymorphicValue object is in undefined state and must not be used henceforth.
   */
  template
    < class IFA                  ///< the nominal Base/Interface class for a family of types
    , size_t storage             ///< maximum storage required for the target data to be held inline
    , class CPY = IFA            ///< special sub-interface to support copy operations
    >
  class PolymorphicValue
    {
      
      typedef polyvalue::Trait<CPY>     _Traits;
      typedef typename _Traits::CopyAPI _CopyHandlingAdapter;
      typedef typename _Traits::Assignment _AssignmentPolicy;
      enum{ 
        siz = storage + _Traits::ADMIN_OVERHEAD 
      };
      
      
      /* === embedded object in buffer === */
      
      /** Storage for embedded objects */
      mutable char buf_[siz];
      
      IFA&
      accessEmbedded()  const
        {
          return reinterpret_cast<IFA&> (buf_);
        }
      
      void
      destroyEmbedded()
        {
          accessEmbedded().~IFA();
        }
      
      
      template<class IMP>
      PolymorphicValue (IMP*)
        {
          BOOST_STATIC_ASSERT (siz >= sizeof(IMP));
          
          new(&buf_) IMP();
        }
      
      template<class IMP, typename A1>
      PolymorphicValue (IMP*, A1 a1)
        {
          REQUIRE (siz >= sizeof(IMP));
          new(&buf_) IMP (a1);
        }
      
      template<class IMP, typename A1, typename A2>
      PolymorphicValue (IMP*, A1 a1, A2 a2)
        {
          REQUIRE (siz >= sizeof(IMP));
          new(&buf_) IMP (a1,a2);
        }
      
      template<class IMP, typename A1, typename A2, typename A3>
      PolymorphicValue (IMP*, A1 a1, A2 a2, A3 a3)
        {
          REQUIRE (siz >= sizeof(IMP));
          new(&buf_) IMP (a1,a2,a3);
        }
      
      
      /**
       * Implementation Helper: supporting copy operations.
       * Actually instances of this Adapter template are placed
       * into the internal buffer, such that they both inherit
       * from the desired implementation type and the copy 
       * support interface. The implementation of the
       * concrete copy operations is provided here
       * forwarding to the copy operations
       * of the implementation object. 
       */
      template<class IMP>
      class Adapter
        : public IMP
        , public _Traits::AdapterAttachment  // mix-in, might be empty
        {
          virtual void
          cloneInto (void* targetBuffer)  const
            {
              new(targetBuffer) Adapter(*this); // forward to copy ctor
            }
          
          virtual void
          copyInto (IFA& targetBase)  const
            {
              REQUIRE (INSTANCEOF (Adapter, &targetBase));
              Adapter& target = static_cast<Adapter&> (targetBase);
              _AssignmentPolicy::assignEmbedded(target,*this);
            }                                   // forward to assignment operator 
          
        public: /* == forwarding ctor to implementation type == */
          
          Adapter() : IMP() { }
          
          template<typename A1>
          Adapter (A1 a1) : IMP(a1) { }
          
          template<typename A1, typename A2>
          Adapter (A1 a1, A2 a2) : IMP(a1,a2) { }
          
          template<typename A1, typename A2, typename A3>
          Adapter (A1 a1, A2 a2, A3 a3) : IMP(a1,a2,a3) { }
          
          /* using default copy and assignment */
        };
      
      
      _CopyHandlingAdapter&
      accessHandlingInterface ()  const
        {
          IFA& bufferContents = accessEmbedded();
          return _Traits::accessCopyHandlingInterface (bufferContents);
        }
      
      
    public: /* === PolymorphicValue public API === */
      
      typedef IFA Interface;
      
      operator Interface& ()
        {
          return accessEmbedded();
        }
      operator Interface const& ()  const
        {
          return accessEmbedded();
        }
      Interface*
      operator-> ()  const
        {
          return &( accessEmbedded() );
        }
      
     ~PolymorphicValue()
        {
          destroyEmbedded();
        }
      
      PolymorphicValue (PolymorphicValue const& o)
        {
          o.accessHandlingInterface().cloneInto (&buf_);
        }
      
      PolymorphicValue&
      operator= (PolymorphicValue const& o)
        {
          o.accessHandlingInterface().copyInto (this->accessEmbedded());
          return *this;
        }
      
      /* === static factory functions === */
      
      template<class IMP>
      static PolymorphicValue
      build ()
        {
          Adapter<IMP>* type_to_build_in_buffer(0);
          return PolymorphicValue (type_to_build_in_buffer);
        }
      
      template<class IMP, typename A1>
      static PolymorphicValue
      build (A1 a1)
        {
          Adapter<IMP>* type_to_build_in_buffer(0);
          return PolymorphicValue (type_to_build_in_buffer, a1);
        }
      
      template<class IMP, typename A1, typename A2>
      static PolymorphicValue
      build (A1 a1, A2 a2)
        {
          Adapter<IMP>* type_to_build_in_buffer(0);
          return PolymorphicValue (type_to_build_in_buffer, a1,a2);
        }
      
      template<class IMP, typename A1, typename A2, typename A3>
      static PolymorphicValue
      build (A1 a1, A2 a2, A3 a3)
        {
          Adapter<IMP>* type_to_build_in_buffer(0);
          return PolymorphicValue (type_to_build_in_buffer, a1,a2,a3);
        }
      
      
      /* === support Equality by forwarding to embedded === */
      
      friend bool
      operator== (PolymorphicValue const& v1, PolymorphicValue const& v2)
      {
        return v1.accessEmbedded() == v2.accessEmbedded();
      }
      friend bool
      operator!= (PolymorphicValue const& v1, PolymorphicValue const& v2)
      {
        return ! (v1 == v2);
      }
    };
  
  
  
  
} // namespace lib
#endif
