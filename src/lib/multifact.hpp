/*
  MULTIFACT.hpp  -  flexible family-of-object factory template

  Copyright (C)         Lumiera.org
    2009,               Hermann Vosseler <Ichthyostega@web.de>

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

/** @file multifact.hpp
 ** Framework for building a configurable factory, to generate families of related objects.
 ** These building blocks are targeted towards the "classical" factory situation: obtaining
 ** objects of various kinds, which are related somehow (usually through an common interface).
 ** The creation of these objects might be non-trivial, while the number of flavours to be
 ** produced and the exact parametrisation isn't known beforehand and needs to be figured out
 ** at runtime. As a solution, thus a number of "fabrication lines" is set up, to be selected
 ** on invocation through an ID (which may be symbolic, hashed or structural).
 ** 
 ** Usually, the issue of object and storage management is closely related, while it is
 ** desirable to keep the object production logic clean of these rather technical concerns.
 ** The implementation built here separates the latter into a policy template invoked as a
 ** \em wrapper, accepting the raw product and either registering it, taking ownership, clone
 ** it or use it for more involved wiring. Obviously, the product generated by the installed
 ** "fabrication lines" needs to be delivered in a form acceptable by the concrete wrapper;
 ** mismatch will be spotted by the compiler on registration of the respective fabrication
 ** function.
 ** 
 ** \par Singleton generation
 ** For the very common situation of building a family of singleton objects, accessible by ID,
 ** there is a convenience shortcut: The nested MultiFact::Singleton template can be instantiated
 ** within the context providing the objects (usually a static context). In itself a lib::Depend
 ** singleton factory, it automatically registers the singleton access function as "fabrication"
 ** function into a suitable MultiFact instance passed in as ctor parameter.
 ** 
 ** @remarks this is the second attempt at building a skeleton of the core factory mechanics.
 **       The first attempt was pre-C++11, relied on partial specialisations and was hard to
 **       understand and maintain. In theory, with C++11 the task should be quite simple now,
 **       relying on rvalue references and variadic templates. Unfortunately, as of 9/2014,
 **       the compiler support is not yet robust enough on Debian/stable really to deal with
 **       \em all the conceivable cases when forwarding arbitrary factory products. Thus
 **       for now we choose to avoid the "perfect forwarding" problem and rather let the
 **       wrapper invoke the fabrication function and handle the result properly.
 ** 
 ** @see multifact-test.cpp
 ** @see multifact-singleton-test.cpp
 ** @see SingletonFactory
 */


#ifndef LIB_MULTIFACT_H
#define LIB_MULTIFACT_H


#include "lib/error.hpp"
#include "lib/depend.hpp"
#include "util.hpp"

#include <functional>
#include <utility>
#include <memory>
#include <map>



namespace lib {
  namespace factory {
    
    // Helpers to wrap the factory's product
    
    /**
     * Dummy "wrapper",
     * to perform the fabrication and return the unaltered product.
     * @remarks this is a "perfect forwarding" implementation,
     *          similar to std::forward, used as policy template
     */
    template<typename TAR>
    struct PassAsIs
      {
        typedef TAR RawType;
        typedef TAR BareType;
        typedef TAR ResultType;
        
        template<class FUN, typename... ARGS>
        ResultType
        wrap (FUN create, ARGS&&... args)  noexcept
          {
            return create(std::forward<ARGS>(args)...);
          }
      };
    
    
    /**
     * Wrapper taking ownership,
     * by wrapping into smart-ptr
     */
    template<typename RAW>
    struct BuildRefcountPtr
      {
        using RawType    = typename std::remove_pointer<RAW>::type;
        using BareType   = RawType *;
        using ResultType = std::shared_ptr<RawType>;
        
        template<class FUN, typename... ARGS>
        ResultType
        wrap (FUN create, ARGS&&... args)
          {
            return ResultType (create(std::forward<ARGS>(args)...));
          }
      };
    
    
    /**
     * Policy: use a custom functor
     * to finish the generated product
     * @remarks the nested structure allows to define
     *          both the raw type and the wrapped type.
     *          On instantiation of the MultiFact, pass
     *          the nested Wrapper struct template param.
     * @warning the RAW type must match the result type
     *          of the MultiFac SIG. Beware of passing
     *          references or pointers to local data.
     */
    template<typename TAR>
    struct Build
      {
        template<typename RAW>
        struct Wrapper
          {
            using RawType    = RAW;
            using BareType   = RAW;
            using ResultType = TAR;
            
            using WrapFunc = std::function<ResultType(BareType)>;
            
            void
            defineFinalWrapper (WrapFunc&& fun)
              {
                this->wrapper_ = fun;
              }
            
            template<class FUN, typename... ARGS>
            ResultType
            wrap (FUN create, ARGS&&... args)
              {
                return wrapper_(std::forward<BareType> (create(std::forward<ARGS>(args)...)));
              }
            
          private:
            WrapFunc wrapper_;
          };
      };
    
    
    
    
    
    /**
     * Table of registered production functions for MultiFact.
     * Each stored function can be accessed by ID and is able
     * to fabricate a specific object, which is assignable to
     * the nominal target type in the MultiFact definition.
     */
    template<typename SIG, typename ID>
    struct Fab
      {
        typedef std::function<SIG> FactoryFunc;
        
        
        FactoryFunc&
        select (ID const& id)
          {
            if (!contains (id))
              throw lumiera::error::Invalid("unknown factory product requested.");
            
            return producerTable_[id];
          }
        
        void
        defineProduction (ID const& id, FactoryFunc fun)
          {
            producerTable_[id] = fun;
          }
        
        
        /* === diagnostics === */
        
        bool empty ()         const { return producerTable_.empty(); }
        bool contains (ID id) const { return util::contains (producerTable_,id); }
        
      private:
        std::map<ID, FactoryFunc> producerTable_;
      };
    
    
    
    /** 
     * @internal configuration of the elements
     * to be combined into a MultiFact instance
     */
    template< typename TY
            , template<class> class Wrapper
            >
    struct FabConfig
      {
        using WrapFunctor    = Wrapper<TY>;
        using BareProduct    = typename WrapFunctor::BareType;
        using WrappedProduct = typename WrapFunctor::ResultType;
        
        typedef BareProduct SIG_Fab(void);
        
        enum{ ARGUMENT_CNT = 0 };
      };
    /**
     * @internal specialisation to deal with the generic case:
     * using an arbitrary fabrication function with multiple arguments
     */
    template< typename RET
            , typename... ARGS
            , template<class> class Wrapper
            >
    struct FabConfig<RET(ARGS...), Wrapper>
      {
        using WrapFunctor    = Wrapper<RET>;
        using BareProduct    = typename WrapFunctor::BareType;
        using WrappedProduct = typename WrapFunctor::ResultType;
        
        typedef BareProduct SIG_Fab(ARGS...);
        
        enum{ ARGUMENT_CNT = sizeof...(ARGS)};
      };
    
    
    
    
    /* === Main type === */
    
    /**
     * Factory for creating a family of objects by ID.
     * The actual factory functions are to be installed
     * from the usage site through calls to #defineProduction .
     * Each generated object will be treated by the Wrapper template,
     * allowing for the generation of smart-ptrs. The embedded class
     * Singleton allows to build a family of singleton objects; it is
     * to be instantiated at the call site and acts as singleton factory,
     * accessible through a MultiFact instance as frontend.
     */
    template< typename SIG
            , typename ID
            , template<class> class Wrapper = PassAsIs
            >
    class MultiFact
      : public FabConfig<SIG,Wrapper>::WrapFunctor
      {
        using   _Conf = FabConfig<SIG,Wrapper>;
        using SIG_Fab = typename _Conf::SIG_Fab;
        using    _Fab = Fab<SIG_Fab,ID>;
        
        _Fab funcTable_;
        
        
      protected:
        using Creator = typename _Fab::FactoryFunc;
        
        Creator&
        selectProducer (ID const& id)
          {
            return funcTable_.select(id);
          }
        
        
      public:
        using Product = typename _Conf::WrappedProduct;
        
        /**
         * Core operation of the factory:
         * Select a production line and invoke the fabrication function.
         * @param id select the actual pre installed fabrication function to use
         * @param args additional arguments to pass to the fabrication.
         * @note the template parameter #SIG defines the raw or nominal signature
         *       of the fabrication, and especially the number of arguments
         * @return the created product, after passing through the #Wrapper functor
         */
        template<typename... ARGS>
        Product
        operator() (ID const& id, ARGS&& ...args)
          {
            static_assert (sizeof...(ARGS) == _Conf::ARGUMENT_CNT,
                           "MultiFac instance invoked with the wrong number "
                           "of fabrication arguments. See template parameter SIG");
            
            Creator& creator = selectProducer (id);
            return this->wrap (creator, std::forward<ARGS>(args)...);
          }
        
        /** more legible alias for the function operator */
        template<typename... ARGS>
        Product
        invokeFactory (ID const& id, ARGS&& ...args)
          {
            return this->operator() (id, std::forward<ARGS>(args)...);
          }
        
        
        /** to set up a production line,
         *  associated with a specific ID
         */
        template<typename FUNC>
        void
        defineProduction (ID id, FUNC&& fun)
          {
            funcTable_.defineProduction (id, fun);
          }
        
        
        /**
         * Convenience shortcut for automatically setting up
         * a production line, to fabricate a singleton instance
         * of the given implementation target type (IMP)
         */
        template<class IMP>
        class Singleton
          : lib::Depend<IMP>
          {
            typedef lib::Depend<IMP> SingleFact;
            
            Creator
            createSingleton_accessFunction()
              {
                return std::bind (&SingleFact::operator()
                                 , static_cast<SingleFact*>(this));
              }
            
          public:
            Singleton (MultiFact& factory, ID id)
              {
                factory.defineProduction(id, createSingleton_accessFunction());
              }
          };
          
        
        /* === diagnostics === */
        
        bool empty ()         const { return funcTable_.empty();       }
        bool contains (ID id) const { return funcTable_.contains (id); }
      };
    
    
    
}} // namespace lib::factory
#endif
