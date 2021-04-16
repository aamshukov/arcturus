﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_TYPES_H__
#define __VIRTUAL_FILE_SYSTEM_TYPES_H__

#pragma once

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

class vfs_types : public noncopyable
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
};

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_TYPES_H__
