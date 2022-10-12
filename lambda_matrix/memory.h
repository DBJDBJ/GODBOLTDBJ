#pragma once
/*
(c) 2022 Q4 by dbj@dbj.org CC BY SA 4.0

we encapsualte memory in one abstraction so we can easily change how do we get it and so on
*/

#include <assert.h>
#include <stdlib.h>

// to be able to use this
// must have in some cpp
// #define USERLAND_SLAB_IMPLEMENTED_HERE
// #include "../src/slab_implementation.h"
#include "userland_slab/src/slab.h"

namespace dbj
{

    // hidden DBJ_MEM_MGR_IMPLEMENTATION
    namespace
    {

        class slab_proxy
        {
            struct Objs_cache cache;
            const unsigned obj_size;

            slab_proxy(const unsigned for_this_size_) : obj_size(for_this_size_)
            {
                // we initialise the slab allocator once and for all
                if (!slab_allocator_init())
                {
                    perror("Error : slab allocator initialisation failed !\n");
                    exit(-1);
                }

                // we initialise a cache to allocate objects of obj_size
                if (!objs_cache_init(&cache, obj_size, NULL))
                {
                    perror("Error : cache initialisation failed !\n");
                    exit(-1);
                }
            }
            ~slab_proxy(void)
            {
                objs_cache_destroy(&cache);
                slab_allocator_destroy();
            }

        public:
            
            unsigned size (void) const noexcept { return obj_size; } ;

            void *allocate(void) noexcept
            {
                return objs_cache_alloc(&cache);
            }

            void release(void *mem_) noexcept
            {
                if (mem_)
                {
                    objs_cache_free(&cache, mem_);
                    mem_ = 0;
                }
            }

            // basically making one slab per one size
            template<unsigned int SZE>
            static slab_proxy &make()
            {
                static slab_proxy slab_proxy_ = slab_proxy(SZE);
                return slab_proxy_;
            }
        };

        /*
           holding allocated blocks until destruction time
           then releasing them
        */
        class memory_manager
        {
            constexpr static size_t blocks_arr_size = 0xFF;
            memory_manager() = default;
            memory_manager(memory_manager const &) = delete;
            memory_manager &operator=(const memory_manager &) = delete;
            memory_manager(memory_manager &&) = delete;
            memory_manager &operator=(memory_manager &&) = delete;

            void *blocks[blocks_arr_size]{};
            int blocks_last_index = 0;

        public:
            // create or reuse slab of a given size
            // return what was made or null if NEM occured
            template<unsigned int size_> 
            void *next(size_t count_) noexcept
            {
                if (blocks_last_index == size)
                    return 0;

                // void *next_block_ = calloc(count_, size_);
                void *next_block_ = slab_proxy::make<size_>().allocate() ;

                if (next_block_)
                {
                    this->blocks[blocks_last_index] = next_block_;
                    blocks_last_index += 1;
                }
                return next_block_;
            }

            template<unsigned int size_> 
            void release() noexcept
            {
                // last_index is one beyond the last occupied slot
                while (--blocks_last_index > -1)
                {
                    // free(blocks[blocks_last_index]);
                    slab_proxy::make<size_>().release( blocks[blocks_last_index] );
                    blocks[blocks_last_index] = 0;
                }
            }

            ~memory_manager() noexcept
            {
                release();
            }

            friend struct memory_manager_proxy;
        };

    } // DBJ_MEM_MGR_IMPLEMENTATION

    struct memory_manager_proxy
    {
        static memory_manager &make(void)
        {
            static memory_manager mmgr_;
            return mmgr_;
        }
    };

    //--------------------------------------------------------------------------------
    inline auto &dbj_memory = memory_manager_proxy::make();
    //--------------------------------------------------------------------------------

} // dbj NS