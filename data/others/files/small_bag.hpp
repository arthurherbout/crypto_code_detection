// Copyright (C) 2018 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef FOONATHAN_ARRAY_SMALL_BAG_HPP_INCLUDED
#define FOONATHAN_ARRAY_SMALL_BAG_HPP_INCLUDED

#include <foonathan/array/bag.hpp>
#include <foonathan/array/block_storage_default.hpp>
#include <foonathan/array/block_storage_heap_sbo.hpp>

namespace foonathan
{
    namespace array
    {
        /// Convenience alias for [array::bag<T>]() with a small size optimization.
        template <typename T, std::size_t SmallN, class Heap = default_heap,
                  class Growth = default_growth>
        using small_bag = bag<T, block_storage_heap_sbo<SmallN * sizeof(T), Heap, Growth>>;
    } // namespace array
} // namespace foonathan

#endif // FOONATHAN_ARRAY_SMALL_BAG_HPP_INCLUDED
