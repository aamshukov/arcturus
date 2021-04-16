//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>
#include <core/cache.hpp>

#include <vfs/vfs_types.hpp>
#include <vfs/vfs_string_pool.hpp>

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

vfs_string_pool::vfs_string_pool()
{
}

vfs_string_pool::~vfs_string_pool()
{
    my_strings.clear();
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
