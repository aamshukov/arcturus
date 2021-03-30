//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_PAGING_H__
#define __VIRTUAL_FILE_SYSTEM_PAGING_H__

#pragma once

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

class vfs_paging : private noncopyable
{
    public:
        using fd_type = FILE*;

        enum class flag : uint64_t
        {
            clear = 0x0000,
            dirty = 0x0002,
            index = 0x0004,
            leaf  = 0x0008,
            error = 0x0010
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

        struct page_descriptor
        {
            std::size_t id;         // page number
            std::size_t next_id;    // overflow page(s)
            std::size_t prev_id;    // overflow page(s)
            std::size_t data_size;  // payload size
            std::size_t checksum;
            std::size_t timestamp;
            std::size_t reserved;
            flags_type  flags;
        };

        struct page
        {
            //?? factory::create<page>()
            page_descriptor descriptor;

            byte* data;

            page()
            {
                descriptor.id = 0;
                //??

                data = reinterpret_cast<byte*>(reinterpret_cast<void*>(this + sizeof(descriptor)));
            }
        };

        using page_type = std::shared_ptr<page>;
        using cache_type = cache<std::size_t, page_type>;

        struct position
        {
            std::size_t offset;  // absolute offset
            std::size_t page_id; // current page
        };

        using position_type = position;

    private:
        std::size_t         my_start;       // offset from start
        std::size_t         my_page_size;   // I/O operations' block size
        std::size_t         my_start_page;  // start page to initiate I/O operations, if none - allocate
        position_type       my_position;    // current I/O operations pointer
        cache_type          my_cache;       // pages cache

    public:
                            vfs_paging(std::size_t start, std::size_t page_size, std::size_t cache_size);
                           ~vfs_paging();

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

#endif // __VIRTUAL_FILE_SYSTEM_PAGING_H__
