//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>
#include <core/cache.hpp>

#include <vfs/vfs_type_traits.hpp>
#include <vfs/vfs_io_manager.hpp>

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

vfs_io_manager::vfs_io_manager(fd_type& fd, std::size_t start, std::size_t page_size, std::size_t cache_size)
              : my_fd(fd), my_start(start), my_page_size(page_size), my_cache(cache_size)
{
}

vfs_io_manager::~vfs_io_manager()
{
}

bool vfs_io_manager::allocate_page(typename vfs_io_manager::id_type& page_id,
                                   byte*& buffer,
                                   size_type& buffer_size)
{
    page_id = type_traits::invalid_id;
    buffer = nullptr;
    buffer_size = 0;

    bool result = false;

    return result;
}

bool vfs_io_manager::save_page(const id_type& page_id)
{
    page_id;//??

    bool result = false;

    return result;
}

END_NAMESPACE
