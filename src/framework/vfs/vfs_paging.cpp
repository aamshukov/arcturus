//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>
#include <core/cache.hpp>

#include <vfs/vfs_paging.hpp>

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

vfs_paging::vfs_paging(std::size_t start, std::size_t page_size, std::size_t cache_size)
          : my_start(start), my_page_size(page_size), my_cache(cache_size)
{
}

vfs_paging::~vfs_paging()
{
}

END_NAMESPACE
