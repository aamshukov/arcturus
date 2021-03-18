//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_H__
#define __VIRTUAL_FILE_SYSTEM_H__

#pragma once

BEGIN_NAMESPACE(core)

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

class time; //??
using time_type = time;
class btree; //??
class guid; //??
using guid_type = guid; //??




template <typename Traits = vfs_traits<byte>>
class vfs : private noncopyable
{
    // syntax:
    //      /root/dir(s)/stream(s)
    public:
        using traits_type = Traits;
        using tree_type = std::shared_ptr<btree>; //??

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
            big_endian    = 2
        };

        enum class access_mode : uint64_t
        {
            read  = 0x0001,
            write = 0x0002,
        };

    public:
        struct header
        {
            char    magic[7];   // AA-vfs
            char    version[8];
        };

        struct descriptor
        {
            guid_type id;

            time_type create_time;
            time_type access_time;
            time_type modify_time;

            flags_type flags;
        };

        struct stream_descriptor : public descriptor
        {
            uint64_t size;
            uint64_t capacity;  // could be fixed
            bool     open;      // true if stream is open
        };

        struct directory_descriptor : public descriptor
        {
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

    private:
        tree_type   my_tree;        

    public:
                    vfs();
                    vfs(const char*);
                    vfs(const wchar_t*);
                    vfs(const cp_type*);
                    vfs(const std::string&);
                    vfs(const std::wstring&);
                    vfs(const codepoints_type&);
                   ~vfs();

        void        create();
        void        open();
        void        close();
        void        flush();

        void        enumerate();                        // enumerates streams and directories in the current directory
        void        enumerate(directory&);              // enumerates streams and directories in the specified directory

        void        enumerate_streams();                // enumerates streams in the current directory
        void        enumerate_streams(directory&);      // enumerates streams in the specified directory

        void        enumerate_directories();            // enumerates directories in the current directory
        void        enumerate_directories(directory&);  // enumerates directories in the specified directory

        void        push_directory(directory&);
        void        pop_directory(directory&);

        status      current_directory(directory& current_directory);
        void        change_directory(directory&);

        status      create_stream(const char*);
        void        create_stream(const wchar_t*);
        void        create_stream(const cp_type*);
        void        create_stream(const std::string&);
        void        create_stream(const std::wstring&);
        void        create_stream(const codepoints_type&);

        status      open_stream(const char*);
        status      open_stream(const wchar_t*);
        status      open_stream(const cp_type*);
        status      open_stream(const std::string&);
        status      open_stream(const std::wstring&);
        status      open_stream(const codepoints_type&);

        void        lock_stream(stream&, std::size_t, std::size_t);
        void        unlock_stream(stream&, std::size_t, std::size_t);

        void        rename_stream(stream&);
        void        delete_stream(stream&);

        void        read_stream(stream&);
        void        write_stream(stream&, byte*);

        void        stream_flush();

        std::size_t stream_size() const;
        void        stream_reserve(std::size_t);

        void        stream_seek(disposition, std::size_t);
};

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_H__
