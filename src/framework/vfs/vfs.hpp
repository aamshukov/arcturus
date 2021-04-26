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
        using type_traits = vfs_type_traits;

        using tree_type = std::shared_ptr<btree<string_type, data>>; //??

        using id_type = typename type_traits::id_type;
        using size_type = typename type_traits::size_type;
        using magic_type = typename type_traits::magic_type;
        using checksum_type = typename type_traits::checksum_type;
        using timestamp_type = typename type_traits::timestamp_type;
        using uuid_type = typename type_traits::uuid_type;

        using endianness_type = typename type_traits::endianness_type;

        using paddr_type = typename type_traits::paddr_type;
        using paddrs_type = typename type_traits::paddrs_type;

        using free_paddrs_type = std::queue<paddr_type>;

        using object_type = typename type_traits::object_type;
        using platform_type = typename type_traits::platform_type;
        using compression_type = typename type_traits::compression_type;

        using flags_type = typename type_traits::flags_type;

    public:
        enum class consts : uint32_t
        {
            name_length     = 255, // persistent - UTF8, in-memory UTF32
            resident_blocks = 16,
            root_page_id    = 0,
            invalid_page_id = std::numeric_limits<std::size_t>::min()
        };

        //enum class magics : magic_type
        //{
        //    master_header = 0x5646534D53484452ULL,  // VFSMSHDR
        //    index_page    = 0x564653494E445047ULL,  // VFSINDPG
        //    leaf_page     = 0x5646534C45465047ULL,  // VFSLEFPG
        //    file_magic    = 0x564653464C445343ULL,  // VFSFLDSC
        //    dir_magic     = 0x5646534452445343ULL,  // VFSDRDSC
        //};

    public:
        enum class disposition
        {
        };






        using string_pool_type = vfs_string_pool;
        //struct string_pool_entry
        //{
        //    id_type   id;
        //    size_type ref_count;
        //    codepoints_type name;
        //};

        //struct string_pool_entry_lt_key_comparator
        //{
        //    bool operator() (const string_pool_entry& lhs, const string_pool_entry& rhs) const
        //    {
        //        return (*lhs).id < (*rhs).id;
        //    }
        //};

        //using strings_type = std::set<string_pool_entry, string_pool_entry_lt_key_comparator>;
        //using id_string_map_type = std::unordered_map<id_type, string_pool_entry>;

        //struct string_pool
        //{
        //    strings_type strings;
        //    id_string_map_type mapping;
        //};

    public:
        using descriptor_table_type = std::unordered_map<cps_type, std::size_t>;

        struct descriptor
        {
            id_type id; // object id, unique for VFS

            object_type type;

            paddr_type paddr;

            time_type create_time;
            time_type access_time;
            time_type modify_time;

            flags_type flags;

            magic_type magic;
        };

        struct file_descriptor : public descriptor // file control block (fcb), inode
        {
            size_type size;
            size_type capacity; // could be fixed, quote

            bool open; // true if stream is open
        };

        struct directory_entry // in-memory or persistent
        {
            id_type object_id;  // object id
            id_type name_id;    // name id from names pool
        };

        struct directory_descriptor : public descriptor
        {
            // list of directory entries
        };

        struct btree_record
        {
            id_type   object_id; // object id
            size_type offset;    // offset to object descriptor
            size_type size;      // size of object
        };

        struct security_descriptor
        {
        };

        struct snapshot
        {
            time_type time; // when snapshot taken
                            // metadata only
        };

        class file
        {
            // list of data blocks
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

        using master_stream_table_type = std::unordered_map<cps_type, descriptor>;

        static const magic_type master_header_magic = 0x5646534D53445343; // VFSMSDSC

        struct master_header
        {
            char             signature[6];      // 'aavfs'                     3   3   4   53
            char             version[64];       // major.minor.patch [note] -> xxx.xxx.xxxxccc...cc
            platform_type    platform;          // on which platform was created
            endianness_type  endianness;        // original endianness
            checksum_type    checksum;          // checksum of the header
            timestamp_type   create_time;       // create time
            timestamp_type   access_time;       // access time
            timestamp_type   modify_time;       // modify time
            flags_type       system_flags;      // system flags
            flags_type       custom_flags;      // custom flags
            size_type        page_size;         // I/O page size
            paddr_type       root_paddr;        // root node location
            paddr_type       free_paddrs;       // start of free paddrs
            id_type          next_id;           // next incremental sequence id
            compression_type compression;       // compression algorithm
            size_type        volume;            // volume number, usually 1
            size_type        volumes;           // how many volumes
            uuid_type        uuid;              // global unique identifier
            byte             custom_data[64];   // custom specific data
            byte             dsignature[256];   // digital signature
            magic_type       magic = master_header_magic;
        };

        using master_header_type = master_header;

    private:
        tree_type           my_tree;            // b-tree hierarchy
        cache_type          my_cache;           // pages cache

        string_pool_type    my_names;           // string pool of names

        std::size_t         my_free_page;       // free page to start writing
        free_paddrs_type    my_free_paddrs;     // the last entry on disk holds the next block with free paddrs,
                                                // when reading in - populate the queue

        paged_position      my_position;        // current I/O operations pointer

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
                            vfs(const cps_type&);
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
        void                create_stream(const cps_type&);

        status              open_stream(const char*);
        status              open_stream(const wchar_t*);
        status              open_stream(const cp_type*);
        status              open_stream(const std::string&);
        status              open_stream(const std::wstring&);
        status              open_stream(const cps_type&);

        //void                lock_stream(stream&, std::size_t, std::size_t);
        //void                unlock_stream(stream&, std::size_t, std::size_t);

        //void                rename_stream(stream&);
        //void                delete_stream(stream&);

        //void                read_stream(stream&);
        //void                write_stream(stream&, byte*);

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
