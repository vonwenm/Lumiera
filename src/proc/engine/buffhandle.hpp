/*
  BUFFHANDLE.hpp  -  Buffer handling support for the render engine

  Copyright (C)         Lumiera.org
    2008,               Hermann Vosseler <Ichthyostega@web.de>

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

/** @file buffhandle.hpp
 ** Various bits needed to support the buffer management within the render nodes.
 ** When pulling data from predecessor nodes and calculating new data, each render node
 ** needs several input and output buffers. These may be allocated and provided by several
 ** different "buffer providers" (for example the frame cache). Typically, the real buffers
 ** will be passed as parameters to the actual job instance when scheduled, drawing on the
 ** results of prerequisite jobs. Yet the actual job implementation remains agnostic with
 ** respect to the way actual buffers are provided; the invocation just pushes BuffHandle
 ** objects around. The actual render function gets an array of C-pointers to the actual
 ** buffers, and for accessing those buffers, the node needs to keep a table of buffer
 ** pointers, and for releasing the buffers later on, we utilise the buffer handles.
 ** The usage pattern of those buffer pointer tables is stack-like, thus the actual
 ** implementation utilises a single large buffer pointer array per pull() call
 ** sequence and dynamically claims small chunks for each node.
 ** 
 ** @see nodewiring-def.hpp
 ** @see nodeoperation.hpp
 ** @see bufftable.hpp       storage for the buffer table
 ** @see engine::RenderInvocation
 */

#ifndef ENGINE_BUFFHANDLE_H
#define ENGINE_BUFFHANDLE_H


#include "lib/error.hpp"
#include "lib/streamtype.hpp"
#include "lib/bool-checkable.hpp"


namespace engine {
  
  class BufferProvider;
  
  
  
  /**
   * An opaque descriptor to identify the type and further properties of a data buffer.
   * For each kind of buffer, there is somewhere a BufferProvider responsible for the
   * actual storage management. This provider may "lock" a buffer for actual use,
   * returning a BuffHandle.
   * 
   * @todo try to move that definition into buffer-provider.hpp   ////////////////////////////////////TICKET #249
   */
  class BufferDescriptor
    {
      BufferProvider* provider_;
      uint64_t subClassification_;
      
      BufferDescriptor(BufferProvider& manager, uint64_t detail)
        : provider_(&manager)
        , subClassification_(detail)
      { }
      
      friend class BufferProvider;
      
    public:
      // using standard copy operations
      
      bool checkValidity();
    };
  
  
  
  class ProcNode;
  typedef ProcNode* PNode;
  
  
  struct ChannelDescriptor  ///////TODO collapse this with BufferDescriptor?
    {
      BufferDescriptor bufferType;
    };
  
  struct InChanDescriptor : ChannelDescriptor
    {
      PNode dataSrc;    ///< the ProcNode to pull this input from
      uint srcChannel; ///<  output channel to use on the predecessor node
    };
  
  
  
  
  
  /**
   * Handle for a buffer for processing data, abstracting away the actual implementation.
   * The real buffer pointer can be retrieved by dereferencing this smart-handle class.
   * 
   * @todo as of 6/2011 it isn't clear how buffer handles are actually created
   *       and how the lifecycle (and memory) management works                  //////////////////////TICKET #249 rework BuffHandle creation and usage
   */
  class BuffHandle
    : public lib::BoolCheckable<BuffHandle>
    {
      typedef lumiera::StreamType::ImplFacade::DataBuffer Buff;
      
      BufferDescriptor descriptor_;
      Buff* pBuffer_; 
      
      
    public:
      /** @internal a buffer handle may be obtained by "locking"
       *  a buffer from the corresponding BufferProvider */
      BuffHandle(BufferDescriptor const& typeInfo, Buff* storage = 0)
        : descriptor_(typeInfo)
        , pBuffer_(storage)
        { }
      
      // using standard copy operations
      
      
      Buff&
      operator* ()  const
        {
          ENSURE (pBuffer_);
          return *pBuffer_;
        }
      
      bool
      isValid()  const
        {
          return bool(pBuffer_)
              && descriptor_.checkValidity();
        }
      
    };
  
  
  
  
} // namespace engine
#endif
