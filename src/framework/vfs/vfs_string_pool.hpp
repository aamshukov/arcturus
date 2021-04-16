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
    public:
        using size_type = typename vfs_types::size_type;
        using length_type = uint16_t;
        using data_type = std::unique_ptr<byte>;

        // count : data, aka pascal string
        struct entry
        {
            length_type size;      // size of the data in bytes
            data_type   data;      // UTF8
            size_type   ref_count; // if 0 - remove from the pool
        };

        using key_type = uint64_t;
        using value_type = entry;

        struct hash_key
        {
            std::size_t operator () (const key_type& key) const
            {
                std::size_t result = key;
                result ^= combine_hash(result);
                return result;
            }
        };

        struct eq_key_comparator
        {
            bool operator() (const key_type& lhs, const key_type& rhs) const
            {
                return lhs == rhs;
            }
        };

        using strings_type = std::unordered_map<key_type, value_type, hash_key, eq_key_comparator>;

    private:
        strings_type    my_strings;

    public:
                        vfs_string_pool();
                       ~vfs_string_pool();

        bool            add(const key_type& key, value_type& value);
        bool            remove(const key_type& key);

        void            load();
        void            save();
};

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_STRING_POOL_H__
