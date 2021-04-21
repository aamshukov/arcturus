//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>
#include <core/cache.hpp>

#include <../../libs/crc32c/crc32c/crc32c.h>

#include <vfs/vfs_type_traits.hpp>
#include <vfs/vfs_paging.hpp>
#include <vfs/vfs_name_pool.hpp>

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

typename vfs_string_pool::id_type vfs_string_pool::dummy_id = 0;
typename vfs_string_pool::name_type vfs_string_pool::dummy_name = { 0, nullptr, 0 };

vfs_string_pool::vfs_string_pool()
{
}

vfs_string_pool::~vfs_string_pool()
{
    my_names_rl.clear();
    my_names.clear();
}

const typename vfs_string_pool::id_type& vfs_string_pool::id(const string_type& name) const
{
    name;
    return dummy_id;
}

const typename vfs_string_pool::id_type& vfs_string_pool::id(const cps_type& name) const
{
    name;
    return dummy_id;
}

const typename vfs_string_pool::id_type& vfs_string_pool::id(const typename vfs_string_pool::name_type& name) const
{
    name;
    return dummy_id;
}

const typename vfs_string_pool::name_type& vfs_string_pool::name(const typename vfs_string_pool::id_type& id) const
{
    id;
    return dummy_name;
}

const typename vfs_string_pool::name_type& vfs_string_pool::name(const string_type& name) const
{
    name;
    return dummy_name;
}

const typename vfs_string_pool::name_type& vfs_string_pool::name(const cps_type& name) const
{
    name;
    return dummy_name;
}

bool vfs_string_pool::add(const string_type& name)
{
    name;
    return true;;
}

bool vfs_string_pool::add(const cps_type& name)
{
    name;
    return true;;
}

bool vfs_string_pool::add(const typename name_type& name)
{
    name;
    return true;;
}

bool vfs_string_pool::remove(const typename id_type& id)
{
    id;
    return true;;
}

bool vfs_string_pool::remove(const string_type& name)
{
    name;
    return true;;
}

bool vfs_string_pool::remove(const cps_type& name)
{
    name;
    return true;;
}

bool vfs_string_pool::remove(const typename name_type& name)
{
    name;
    return true;;
}


//const typename vfs_string_pool::key_type& vfs_string_pool::key(typename vfs_string_pool::value_type& value) const
//{
//    const key_type *result(&dummy_key);
//
//    const auto it(my_strings.find(value));
//
//    if(it != my_strings.end())
//    {
//        result = (key_type*)(&(*it).second);
//    }
//
//    return *result;
//}
//
//const typename vfs_string_pool::value_type& vfs_string_pool::value(key_type& key) const
//{
//    value_type *result(&dummy_value);
//
//    const auto it(my_strings.find(key));
//
//    if(it != my_strings.end())
//    {
//        result = (value_type*)(&(*it).second);
//    }
//
//    return *result;
//}
//
//bool vfs_string_pool::add(const typename vfs_string_pool::key_type& key,
//                          typename vfs_string_pool::value_type& value)
//{
//    auto result = my_strings.emplace(std::make_pair(value, key));
//
//    value.ref_count++;
//
//    if(result.second)
//    {
//        my_strings_rl.emplace(std::make_pair(key, &(*result.first).second));
//    }
//
//    return result.second;
//}
//
//bool vfs_string_pool::remove(const typename vfs_string_pool::key_type& key)
//{
//    bool result = false;
//
//    //auto it(my_strings.find(key));
//
//    //if(it != my_strings.end())
//    //{
//    //    auto& value((*it).second);
//
//    //    if(value.ref_count == 0)
//    //    {
//    //        my_strings_rl.erase(&(*it).second);
//    //        my_strings.erase(it);
//
//    //        value.ref_count--;
//
//    //        result = true;
//    //    }
//    //}
//
//    return result;
//}

void vfs_string_pool::load(typename vfs_string_pool::io_type& io)
{
    io; //??
    
    //auto page_size = (*io).page_size();
    //page_size;//??
}

void vfs_string_pool::save(typename vfs_string_pool::io_type& io)
{
    //id_type page_id = 0;
    //id_type prev_page_id = 0;

    //byte* buffer = nullptr;
    //size_type size = 0;

    io;

    //for(const auto& kvp : my_names)
    //{
    //    kvp;
    //    prev_page_id = page_id;
    //    //io.get_space(page_id, buffer, size);
    //}

    //auto page(io.get_page());

    //auto page_size = (*io).page_size();
    //page_size;//??
}

END_NAMESPACE
