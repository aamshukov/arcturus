//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_STRING_POOL_H__
#define __VIRTUAL_FILE_SYSTEM_STRING_POOL_H__

#pragma once

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

class vfs_string_pool : private noncopyable
{
    // |................|................|................|................|................|................|
    //  hdr e0 e1 ... eN e0 e1 ...        eN             eN+1         eN           +             2
    public:                                          //   might overflow to another page(s)
        using type_traits = vfs_type_traits;

        using id_type = typename type_traits::id_type;
        using size_type = typename type_traits::size_type;
        using magic_type = typename type_traits::magic_type;
        using checksum_type = typename type_traits::checksum_type;

        using length_type = uint16_t;
        using data_type = std::shared_ptr<byte>;

        struct descriptor
        {
            magic_type magic;
            size_type  page_size;
            size_type  pool_size;   // how many entries
            size_type  start;       // offset to the first entry
        };

        using descriptor_type = descriptor;

        struct name
        {
            length_type size;      // size of the data in bytes
            data_type   data;      // UTF8
            size_type   ref_count; // if 0 - remove from the pool
        };

        using name_type = name;
        using ptr_name_type = name_type*;

        struct hash_name
        {
            std::size_t operator () (const name_type& name) const
            {
                std::size_t result = name.size;
                result ^= combine_hash(crc32c_append(static_cast<uint32_t>(result), name.data.get(), name.size));
                result ^= combine_hash(result);
                return result;
            }
        };

        struct eq_name_comparator
        {
            bool operator() (const name_type& lhs, const name_type& rhs) const
            {
                return lhs.size == rhs.size && memcmp(lhs.data.get(), rhs.data.get(), lhs.size) == 0;
            }
        };

        struct hash_id
        {
            std::size_t operator () (const id_type& id) const
            {
                std::size_t result = id;
                result ^= combine_hash(result);
                return result;
            }
        };

        struct eq_id_comparator
        {
            bool operator() (const id_type& lhs, const id_type& rhs) const
            {
                return lhs == rhs;
            }
        };

        using names_type = std::unordered_map<name_type, id_type, hash_name, eq_name_comparator>; // name -> id
        using names_rl_type = std::unordered_map<id_type, ptr_name_type, hash_id, eq_id_comparator>; // id -> name*, reverse lookup

        using io_manager_type = vfs_io_manager;

    private:
        names_type          my_names;    // name -> id
        names_rl_type       my_names_rl; // id -> name*

    private:
        static name_type    invalid_name;

    private:
        static string_type  name_to_string(const name_type& name);
        static bool         chars_to_name(const char_type* wchars, name_type& name);
        static bool         codepoints_to_name(const cps_type& codepoints, name_type& name);

    public:
                            vfs_string_pool();
                           ~vfs_string_pool();

        bool                id(const string_type& string, id_type& result_id) const;
        bool                id(const cps_type& codepoints, id_type& result_id) const;
        bool                id(const name_type& name, id_type& result_id) const;

        bool                name(const id_type& id, name_type& result_name) const;

        bool                add(const char_type* chars, id_type& result_id);
        bool                add(const string_type& string, id_type& result_id);
        bool                add(const cps_type& codepoints, id_type& result_id);
        bool                add(name_type& name, id_type& result_id);

        bool                remove(const id_type& id);
        bool                remove(const char_type* wchars);
        bool                remove(const string_type& string);
        bool                remove(const cps_type& codepoints);
        bool                remove(const name_type& name);

        void                reset();

        bool                load(io_manager_type& io_manager);
        bool                save(io_manager_type& io_manager);
};

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_STRING_POOL_H__
