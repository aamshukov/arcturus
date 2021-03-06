﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_IO_MANAGER_H__
#define __VIRTUAL_FILE_SYSTEM_IO_MANAGER_H__

#pragma once

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

class vfs_io_manager : private noncopyable
{
    public:
        using type_traits = vfs_type_traits;

        using id_type = typename type_traits::id_type;
        using size_type = typename type_traits::size_type;
        using magic_type = typename type_traits::magic_type;
        using checksum_type = typename type_traits::checksum_type;
        using timestamp_type = typename type_traits::timestamp_type;
        using uuid_type = typename type_traits::uuid_type;

        using endianness_type = typename type_traits::endianness_type;

        using fd_type = typename type_traits::fd_type;

        using paddr_type = typename type_traits::paddr_type;
        using paddrs_type = typename type_traits::paddrs_type;

        using flags_type = typename type_traits::flags_type;

        struct page_descriptor
        {
            magic_type     magic;
            id_type        id;          // page number
            id_type        next_id;     // overflow page(s)
            id_type        prev_id;     // overflow page(s)
            size_type      data_size;   // payload size
            checksum_type  checksum;    // crc32c
            timestamp_type timestamp;   // serialized timestamp
            flags_type     flags;       // 
        };

        struct page
        {
            page_descriptor descriptor;

            byte* data;

            page(size_type page_size) : descriptor({})
            {
                auto data_size = page_size - sizeof(page::descriptor) - sizeof(page::data);

                data = new byte[data_size];

                memset(data, 0, data_size);
            }

           ~page()
            {
                delete [] data;
            }
        };

        using page_type = std::shared_ptr<page>;
        using cache_type = cache<std::size_t, page_type>;

        struct position
        {
            size_type offset;  // absolute offset
            size_type page_id; // current page
        };

        using position_type = position;

    private:
        fd_type             my_fd;          // open file descriptor, ready to read/write
        size_type           my_start;       // offset from start
        size_type           my_page_size;   // I/O operations' block size
        size_type           my_start_page;  // start page to initiate I/O operations, if none - allocate
        position_type       my_position;    // current I/O operations pointer
        cache_type          my_cache;       // pages cache

    private:
        page_type           create_page(const id_type& page_id);
        void                goto_page(const id_type& page_id);

    public:
                            vfs_io_manager(fd_type& fd, std::size_t start, std::size_t page_size, std::size_t cache_size);
                           ~vfs_io_manager();

        size_type           page_size() const;
        size_type           position() const; // current position

        bool                initialize(fd_type& fd); //?? read pagination header and the first page
        bool                finalize(fd_type& fd); //?? save header and all dirty pages

        bool                allocate_page(id_type& page_id, byte*& buffer, size_type& buffer_size);

        bool                read_page(const id_type& page_id, byte*& buffer, size_type& buffer_size);
        bool                save_page(const id_type& page_id);

        bool                mark_page_as_dirty(const id_type& page_id);

        bool                read(fd_type& fd,
                                 std::size_t offset,        // absolute offset
                                 byte* data,                // payload
                                 std::size_t data_size,     // payload size, might spread several pages
                                 std::size_t& read_bytes,   // how many bytes read
                                 status& status);           // status of the operation

        bool                write(fd_type& fd,
                                  std::size_t offset,         // absolute offset
                                  const byte* data,           // payload
                                  std::size_t data_size,      // payload size, might spread several pages
                                  std::size_t& written_bytes, // how many bytes written
                                  status& status);            // status of the operation
};

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_IO_MANAGER_H__
