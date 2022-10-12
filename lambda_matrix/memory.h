#pragma once
/*
(c) 2022 Q4 by dbj@dbj.org CC BY SA 4.0

we encapsualte memory in one abstyarction so we can easily change hwo do we get it and so on
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
            slab_proxy(void)
            {
            }
            ~slab_proxy(void)
            {
            }

        public:

            void * allocate ( size_t sz_) noexcept { return 0; }
            void   release  ( void * mem_, size_t ) noexcept {}
            static slab_proxy &make()
            {
            }
        };

        class memory_manager
        {
            constexpr static size_t size = 0xFF;
            memory_manager() = default;
            memory_manager(memory_manager const &) = delete;
            memory_manager &operator=(const memory_manager &) = delete;
            memory_manager(memory_manager &&) = delete;
            memory_manager &operator=(memory_manager &&) = delete;

            void *blocks[size]{};
            int last_index = 0;

        public:
            // return what was made or null if NEM occured
            void *next(size_t count_, size_t size_) noexcept
            {
                if (last_index == size)
                    return 0;

                void *next_block_ = calloc(count_, size_);

                if (next_block_)
                {
                    blocks[last_index] = next_block_;
                    last_index += 1;
                }
                return next_block_;
            }

            void release() noexcept
            {
                // last_index is one beyond the last occupied slot
                while (--last_index > -1)
                {
                    free(blocks[last_index]);
                    blocks[last_index] = 0;
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