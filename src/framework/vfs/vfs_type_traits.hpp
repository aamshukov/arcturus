//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_TYPE_TRAITS_H__
#define __VIRTUAL_FILE_SYSTEM_TYPE_TRAITS_H__

#pragma once

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

class vfs_type_traits : public noncopyable
{
    public:
        using id_type = uint64_t;
        using size_type = uint64_t;
        using magic_type = uint64_t;
        using checksum_type = uint64_t;
        using timestamp_type = uint64_t;
        using uuid_type = byte[16]; // GUID/UUID

        using endianness_type = endianness;

        using paddr_type = std::size_t; // persistent address
        using paddrs_type = std::vector<paddr_type>;

        using fd_type = FILE*;

    public:
        enum class magics : magic_type
        {
            master_header = 0x5646534D53484452ULL,  // VFSMSHDR
            index_page    = 0x564653494E445047ULL,  // VFSINDPG
            leaf_page     = 0x5646534C45465047ULL,  // VFSLEFPG
            file_magic    = 0x564653464C445343ULL,  // VFSFLDSC
            dir_magic     = 0x5646534452445343ULL,  // VFSDRDSC

            string_pool_header = 0x5646535350484452ULL,  // VFSSPHDR
        };

        enum class flags : uint64_t
        {
            clear       = 0x0000000000000000,
            dirty       = 0x0000000000000001,
            error       = 0x0000000000000002,

            open        = 0x0000000000000004,
            eos         = 0x0000000000000008,

            index       = 0x0000000000000010,
            leaf        = 0x0000000000000020,
            overflow    = 0x0000000000000040,
            underflow   = 0x0000000000000080,
            resident    = 0x0000000000000100
        };

        DECLARE_ENUM_OPERATORS(flags)

        using flags_type = flags;

        enum class platform : uint8_t
        {
            unknown = 0,
            windows = 1,
            unix    = 2 // unixes, linux and others
        };

        using platform_type = platform;

        enum class compression : uint8_t
        {
            uncompressed = 0,
            pf_algorithm = 1
        };

        using compression_type = compression;

        enum class file_type : uint8_t
        {
            directory = 1,
            file = 2
        };

        enum class access_mode : uint8_t
        {
            read  = 0x0001,
            write = 0x0002
        };

        using access_mode_type = access_mode;

        static const id_type invalid_id = static_cast<id_type>(-1);
};

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_TYPE_TRAITS_H__
