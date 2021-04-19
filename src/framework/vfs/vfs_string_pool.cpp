//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>
#include <core/cache.hpp>

#include <../../libs/crc32c/crc32c/crc32c.h>

#include <vfs/vfs_types.hpp>
#include <vfs/vfs_string_pool.hpp>

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

typename vfs_string_pool::key_type vfs_string_pool::dummy_key = 0;
typename vfs_string_pool::value_type vfs_string_pool::dummy_value = { 0, nullptr, 0 };

vfs_string_pool::vfs_string_pool()
{
}

vfs_string_pool::~vfs_string_pool()
{
    my_strings.clear();
}

const typename vfs_string_pool::key_type& vfs_string_pool::key(typename vfs_string_pool::value_type& value) const
{
    const key_type *result(&dummy_key);

    const auto it(my_rl_strings.find(&value));

    if(it != my_rl_strings.end())
    {
        result = (key_type*)(&(*it).second);
    }

    return *result;
}

const typename vfs_string_pool::value_type& vfs_string_pool::value(key_type& key) const
{
    value_type *result(&dummy_value);

    const auto it(my_strings.find(key));

    if(it != my_strings.end())
    {
        result = (value_type*)(&(*it).second);
    }

    return *result;
}

bool vfs_string_pool::add(const typename vfs_string_pool::key_type& key,
                                 typename vfs_string_pool::value_type& value)
{
    auto result = my_strings.emplace(std::make_pair(key, std::move(value)));

    value.ref_count++;

    return result.second;
}

bool vfs_string_pool::remove(const typename vfs_string_pool::key_type& key)
{
    bool result = false;

    auto it(my_strings.find(key));

    if(it != my_strings.end())
    {
        auto& value((*it).second);

        if(value.ref_count == 0)
        {
            my_strings.erase(it);

            value.ref_count--;

            result = true;
        }
    }

    return result;
}

void vfs_string_pool::load()
{
}

void vfs_string_pool::save()
{
}

END_NAMESPACE
