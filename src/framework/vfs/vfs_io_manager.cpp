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

    page_type page(create_page(page_id));

    my_cache.put(page_id, page);

    save_page(page_id);

    buffer = (*page).data;
    buffer_size = (*page).descriptor.data_size;

    result = true;

    return result;
}

bool vfs_io_manager::read_page(const typename vfs_io_manager::id_type& page_id,
                               byte*& buffer,
                               size_type& buffer_size)
{
    buffer = nullptr;
    buffer_size = 0;

    bool result = false;

    auto page = my_cache.get(page_id);

    if(!page.has_value())
    {
        page = create_page(page_id);
    }

    goto_page(page_id);

    fread(&(*page.value()).descriptor, sizeof(byte), sizeof((*page.value()).descriptor), my_fd);
    fread(&(*page.value()).data, sizeof(byte), sizeof((*page.value()).descriptor.data_size), my_fd);

    buffer = (*page.value()).data;
    buffer_size = (*page.value()).descriptor.data_size;

    result = true;

    return result;
}

bool vfs_io_manager::save_page(const id_type& page_id)
{
    bool result = false;

    auto page = my_cache.get(page_id);

    if(page.has_value())
    {
        (*page.value()).descriptor.checksum = 0; //??
        (*page.value()).descriptor.timestamp = 0; //??

        goto_page(page_id);

        fwrite(&(*page.value()).descriptor, sizeof(byte), sizeof((*page.value()).descriptor), my_fd);
        fwrite(&(*page.value()).data, sizeof(byte), sizeof((*page.value()).descriptor.data_size), my_fd);

        fflush(my_fd);
    }

    return result;
}

typename vfs_io_manager::page_type vfs_io_manager::create_page(const id_type& page_id)
{
    page_type result(factory::create<page>(my_page_size));

    (*result).descriptor.magic = static_cast<magic_type>(type_traits::magics::io_page_header);
    (*result).descriptor.id = page_id;
    (*result).descriptor.next_id = type_traits::invalid_id;
    (*result).descriptor.prev_id = type_traits::invalid_id;
    (*result).descriptor.data_size = my_page_size - sizeof(page::descriptor) - sizeof(page::data);
    (*result).descriptor.checksum = 0;
    (*result).descriptor.timestamp = 0;
    (*result).descriptor.flags = type_traits::flags::clear; 

    (*result).data;

    return result;
}

void vfs_io_manager::goto_page(const typename vfs_io_manager::id_type& page_id)
{
    size_type offset = my_start + page_id * my_page_size;

    if(!_fseeki64(my_fd, offset, SEEK_SET))
    {
        my_position.offset = offset;
        my_position.page_id = page_id;
    }
}

END_NAMESPACE
