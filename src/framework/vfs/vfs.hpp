//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_H__
#define __VIRTUAL_FILE_SYSTEM_H__

#pragma once

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

template <typename T>
struct vfs_traits
{
    std::size_t page_size;
};

template <>
struct vfs_traits<bitset<uint64_t>::bit>
{
};

template <>
struct vfs_traits<byte>
{
};

template <>
struct vfs_traits<wchar_t>
{
};

class time
{
    std::size_t utc;
    std::size_t time_zone;
};

using time_type = time;
class guid; //??
using guid_type = guid; //??


template <typename TValue>
class btree<string_type, typename TValue, std::less<string_type>, std::hash<string_type>> : public noncopyable
{
};

struct data //??
{
};


template <typename Traits = vfs_traits<byte>>
class vfs : private noncopyable
{
    // header | string pool | descriptors | data
    // 512    | btree       | btree       | btree



    // syntax:
    //      /root/dir(s)/stream(s)
    public:
        using traits_type = Traits;
        using tree_type = std::shared_ptr<btree<string_type, data>>; //??

        using id_type = uint64_t;   // absolute number
        using loc_type = uint64_t;  // absolute location
        using size_type = uint64_t; // absolute size

    public:
        enum class disposition
        {
        };

        enum class flag : uint64_t
        {
            clear = 0x0000,
            open  = 0x0001,
            eos   = 0x0002,
            error = 0x0004
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

        enum class platform
        {
            unknown = 0,
            windows = 1,
            unix    = 2 // unixes, linux and others
        };

        enum class endianness // target endianness
        {
            unknown       = 0,
            little_endian = 1,
            big_endian    = 2,
            cpu_endian    = 3 // platform/cpu specific
        };

        enum class type : uint64_t
        {
            directory = 1,
            stream    = 2
        };

        enum class access_mode : uint64_t
        {
            read  = 0x0001,
            write = 0x0002,
        };



        struct string_pool_entry
        {
            id_type   id;
            size_type ref_count;
            codepoints_type name;
        };

        struct string_pool_entry_lt_key_comparator
        {
            bool operator() (const string_pool_entry& lhs, const string_pool_entry& rhs) const
            {
                return (*lhs).id < (*rhs).id;
            }
        };

        using strings_type = std::set<string_pool_entry, string_pool_entry_lt_key_comparator>;
        using id_string_map_type = std::unordered_map<id_type, string_pool_entry>;

        struct string_pool
        {
            strings_type strings;
            id_string_map_type mapping;
        };

    public:
        struct vfs_header
        {
            char    signature[7]; // aa-vfs                        3   3    4      53
            char    version[64];  // major.minor.patch [note] -> xxx.xxx.xxxxccc...cc

            const id_type magic = 0x5505505512349851;
        };

        using descriptor_table_type = std::unordered_map<codepoints_type, std::size_t>;

        struct descriptor
        {
            std::size_t id;

            time_type create_time;
            time_type access_time;
            time_type modify_time;

            flags_type flags;

            const id_type magic = 0x5505505512349852;
        };

        struct stream_descriptor : public descriptor // file control block (fcb), inode
        {
            uint64_t size;
            uint64_t capacity;  // could be fixed
            bool     open;      // true if stream is open

            const id_type magic = 0x5505505512349853;
        };

        struct directory_descriptor : public descriptor
        {
            //stream_table_type stream_table;
        };

        struct security_descriptor
        {
            
        };

        struct snapshot
        {
            time_type time; // when snapshot taken
                            // metadata only
        };

        class stream
        {
        };

        class directory
        {
            
        };

        struct page_descriptor
        {
            std::size_t id;           // unique number
            std::size_t next_page_id; // next page id if data spreaded
            std::size_t data_offset;  // payload offset
            std::size_t data_size;    // payload size
            std::size_t data_capacity;// payload capacity
            flags_type  flags;

            const id_type magic = 0x5505505512349854;
        };

        struct page
        {
            //?? factory::create<page>()
            page_descriptor descriptor;

            byte* data;

            page()
            {
                data = reinterpret_cast<byte*>(reinterpret_cast<void*>(this) + sizeof(descriptor));
            }
        };

        using page_type = std::shared_ptr<page>;
        using cache_type = cache<std::size_t, page_type>;

        struct paged_position
        {
            std::size_t offset;  // absolute offset
            std::size_t page_id; // current page
        };

        struct fat // file allocation table
        {
            std::vector<std::size_t> pages;
        };

        using master_stream_table_type = std::unordered_map<codepoints_type, descriptor>;

    private:
        tree_type           my_tree;            // b-tree hierarchy
        cache_type          my_cache;           // pages cache
        std::size_t         my_free_page;       // free page to start writing
        paged_position      my_paged_position;  // current paged I/O operations pointer

    private:
        static std::size_t  get_page_size();

        std::size_t         paged_read_data(std::size_t offset,     // absolute offset
                                            byte* data,             // data
                                            std::size_t data_size); // data size, might read from several pages

        std::size_t         paged_write_data(std::size_t offset,     // absolute offset
                                             const byte* data,       // data
                                             std::size_t data_size); // data size, might spread several pages

    public:
                            vfs();
                            vfs(const char*);
                            vfs(const wchar_t*);
                            vfs(const cp_type*);
                            vfs(const std::string&);
                            vfs(const std::wstring&);
                            vfs(const codepoints_type&);
                           ~vfs();

        void                create();
        void                open();
        void                close();
        void                flush();

        void                enumerate();                        // enumerates streams and directories in the current directory
        void                enumerate(directory&);              // enumerates streams and directories in the specified directory

        void                enumerate_streams();                // enumerates streams in the current directory
        void                enumerate_streams(directory&);      // enumerates streams in the specified directory

        void                enumerate_directories();            // enumerates directories in the current directory
        void                enumerate_directories(directory&);  // enumerates directories in the specified directory

        void                push_directory(directory&);
        void                pop_directory(directory&);

        status              current_directory(directory& current_directory);
        void                change_directory(directory&);

        status              create_stream(const char*);
        void                create_stream(const wchar_t*);
        void                create_stream(const cp_type*);
        void                create_stream(const std::string&);
        void                create_stream(const std::wstring&);
        void                create_stream(const codepoints_type&);

        status              open_stream(const char*);
        status              open_stream(const wchar_t*);
        status              open_stream(const cp_type*);
        status              open_stream(const std::string&);
        status              open_stream(const std::wstring&);
        status              open_stream(const codepoints_type&);

        void                lock_stream(stream&, std::size_t, std::size_t);
        void                unlock_stream(stream&, std::size_t, std::size_t);

        void                rename_stream(stream&);
        void                delete_stream(stream&);

        void                read_stream(stream&);
        void                write_stream(stream&, byte*);

        void                stream_flush();

        std::size_t         stream_size() const;
        void                stream_reserve(std::size_t);

        void                stream_seek(disposition, std::size_t);
};

template <typename Traits>
vfs<Traits>::vfs() : my_cache(32)
{
    get_page_size();
}

template <typename Traits>
vfs<Traits>::~vfs()
{
    //??stream_flush();
}

template <typename Traits>
std::size_t vfs<Traits>::get_page_size()
{
    std::size_t result = 4 * 1024;

#ifdef _WIN32
    DWORD sectorsPerCluster;
    DWORD bytesPerSector;
    DWORD numberOfFreeClusters;
    DWORD totalNumberOfClusters;

    GetDiskFreeSpace(L".", &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters);

    result = sectorsPerCluster * bytesPerSector;
#endif

    return result;
}

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_H__
