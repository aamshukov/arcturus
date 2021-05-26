//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>
#include <core/cache.hpp>

#include <../../libs/crc32c/crc32c/crc32c.h>

#include <vfs/vfs_type_traits.hpp>
#include <vfs/vfs_io_manager.hpp>
#include <vfs/vfs_name_pool.hpp>

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

typename vfs_string_pool::name_type vfs_string_pool::invalid_name = { 0, nullptr, 0 };

vfs_string_pool::vfs_string_pool()
{
}

vfs_string_pool::~vfs_string_pool()
{
    reset();
}

bool vfs_string_pool::id(const string_type& string, typename vfs_string_pool::id_type& result_id) const
{
    bool result = false;

    result_id = type_traits::invalid_id;

    try
    {
        name_type name;

        chars_to_name(string.c_str(), name);

        id_type aux_id;

        result = id(name, aux_id);

        if(result)
        {
            result_id = aux_id;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: the name '%s' is not in the pool.",
                            string.c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::id(const cps_type& codepoints, typename vfs_string_pool::id_type& result_id) const
{
    bool result = false;

    result_id = type_traits::invalid_id;

    try
    {
        name_type name;

        codepoints_to_name(codepoints, name);

        id_type aux_id;

        result = id(name, aux_id);

        if(result)
        {
            result_id = aux_id;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: the name '%s' is not in the pool.",
                            text::codepoints_to_string(codepoints.c_str(), codepoints.size()).c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::id(const typename vfs_string_pool::name_type& name, typename vfs_string_pool::id_type& result_id) const
{
    bool result = false;

    result_id = type_traits::invalid_id;

    try
    {
        const auto it(my_names.find(name));

        if(it != my_names.end())
        {
            result_id = (*it).second;
            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: the name '%s' is not in the pool.",
                            name_to_string(name).c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::name(const typename vfs_string_pool::id_type& id,
                           typename vfs_string_pool::name_type& result_name) const
{
    bool result = false;

    result_name = invalid_name;

    try
    {
        const auto it(my_names_rl.find(id));

        if(it != my_names_rl.end())
        {
            result_name = (*(*it).second);
            result = true;
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: the id '%ld' is not in the pool.",
                            id)
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::add(const char_type* wchars, id_type& result_id)
{
    bool result = false;

    try
    {
        name_type name;

        chars_to_name(wchars, name);

        result = add(name, result_id);
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot add '%s' to the pool.",
                            wchars)
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::add(const string_type& string, id_type& result_id)
{
    bool result = false;

    try
    {
        name_type name;

        chars_to_name(string.c_str(), name);

        result = add(name, result_id);
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot add '%s' to the pool.",
                            string.c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::add(const cps_type& codepoints, id_type& result_id)
{
    bool result = false;

    try
    {
        name_type name;

        codepoints_to_name(codepoints, name);

        result = add(name, result_id);
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot add '%s' to the pool.",
                            text::codepoints_to_string(codepoints.c_str(), codepoints.size()).c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::add(typename name_type& name, id_type& result_id)
{
    bool result = false;

    try
    {
        id_type id = my_names.size() + 1;

        auto kvp = my_names.emplace(std::make_pair(name, id));

        name_type& nm = const_cast<name_type&>((*kvp.first).first);

        result_id = (*kvp.first).second;

        nm.ref_count++;

        if(kvp.second)
        {
            my_names_rl.emplace(std::make_pair(id, &nm));
        }

        result = kvp.second;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot add '%s' to the pool.",
                            name_to_string(name).c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::remove(const typename id_type& id)
{
    bool result = false;

    try
    {
        auto it(my_names_rl.find(id));

        if(it != my_names_rl.end())
        {
            name_type& name(*(*it).second);

            result = remove(name);
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot remove '%ld' from the pool.",
                            id)
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::remove(const char_type* wchars)
{
    bool result = false;

    try
    {
        name_type name;

        chars_to_name(wchars, name);

        result = remove(name);
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot remove '%s' from the pool.",
                            wchars)
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::remove(const string_type& string)
{
    bool result = false;

    try
    {
        name_type name;

        chars_to_name(string.c_str(), name);

        result = remove(name);
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot remove '%s' from the pool.",
                            string.c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::remove(const cps_type& codepoints)
{
    bool result = false;

    try
    {
        name_type name;

        codepoints_to_name(codepoints, name);

        result = remove(name);
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot remove '%s' from the pool.",
                            text::codepoints_to_string(codepoints.c_str(), codepoints.size()).c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool vfs_string_pool::remove(const typename name_type& name)
{
    bool result = false;

    try
    {
        auto it(my_names.find(name));

        if(it != my_names.end())
        {
            name_type& nm = const_cast<name_type&>((*it).first);

            if(--nm.ref_count == 0)
            {
                my_names_rl.erase((*it).second);
                my_names.erase(it);

                result = true;
            }
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: cannot remove '%s' from the pool.",
                            name_to_string(name).c_str())
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

void vfs_string_pool::reset()
{
    my_names_rl.clear();
    my_names.clear();
}

bool vfs_string_pool::load(typename vfs_string_pool::io_manager_type& io_manager)
{
    bool result = false;

    io_manager; //??
    
    //auto page_size = (*io).page_size();
    //page_size;//??

    return result;
}

bool vfs_string_pool::save(typename vfs_string_pool::io_manager_type& io_manager)
{
    bool result = true;

    try
    {
        id_type page_id = type_traits::invalid_id;

        byte* buffer = nullptr;
        size_type buffer_size = 0;

        size_type offset = 0; // current offset in buffer

        int index = 0; // which part of name has been serialized

        if(io_manager.allocate_page(page_id, buffer, buffer_size) &&
           buffer_size > sizeof(name_type)) // > because at least one byte of data should be stored
        {
            WRITE64_LE(endianness::instance().host_to_le64(type_traits::invalid_id),
                                                           buffer,
                                                           offset); // beginning of buffer - next page
            io_manager.mark_page_as_dirty(page_id);

            for(const auto& kvp : my_names)
            {
                const auto& name(kvp.first);
                const auto& id(kvp.second);

                index = 0;

                if(offset == buffer_size)
                {
__allocate:
                    id_type prev_page_id = page_id;

                    byte* new_buffer = nullptr;
                    size_type new_buffer_size = 0;

                    if(io_manager.allocate_page(page_id, new_buffer, new_buffer_size) && buffer_size > sizeof(name_type))
                    {
                        goto __bad_page_allocation;
                    }

                    offset = 0;

                    WRITE64_LE(endianness::instance().host_to_le64(page_id), buffer, offset); // beginning of buffer - next page

                    io_manager.save_page(prev_page_id);
                    io_manager.mark_page_as_dirty(page_id);

                    buffer = new_buffer;
                    buffer_size = new_buffer_size;
                }

                if(index == 0)
                {
                    if(offset <= (buffer_size - sizeof(length_type))) // name length
                    {
                        WRITE16_LE(endianness::instance().host_to_le16(name.size), buffer, offset);
                        index++;
                    }
                    else
                        goto __allocate;
                }

                if(index == 1)
                {
                    if(offset <= (buffer_size - sizeof(size_type))) // name ref count
                    {
                        WRITE64_LE(endianness::instance().host_to_le64(name.ref_count), buffer, offset);
                        index++;
                    }
                    else
                        goto __allocate;
                }

                if(index == 2)
                {
                    if(offset <= (buffer_size - sizeof(size_type))) // name id
                    {
                        WRITE64_LE(endianness::instance().host_to_le64(id), buffer, offset);
                        index++;
                    }
                    else
                        goto __allocate;
                }

                if(index == 3)
                {
                    if(offset <= (buffer_size - name.size)) // name
                    {
                        WRITE_BYTES(name.data.get(), buffer, name.size, offset);
                    }
                    else
                    {
                        // this case might happen for large names which overflow page
                        for(length_type k = 0, n = name.size; k < n; k++, offset++)
                        {
                            if(!(offset <= buffer_size))
                            {
                                break;
                            }

                            *(buffer + offset) = *(name.data.get() + k);
                        }
                    }
                }
            }

            io_manager.save_page(page_id);
        }
        else
        {
__bad_page_allocation:
            OPERATION_FAILED(status::custom_code::error,
                             0,
                             status::contributor::vfs,
                             L"VFS: names pool save operation has failed, unable allocate a page.")
            log_error(diagnostics::instance().last_status().text().c_str());
        }
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributor::vfs,
                            L"VFS: names pool save operation has failed.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

string_type vfs_string_pool::name_to_string(const name_type& name)
{
    cps_type codepoints = text::chars_to_codepoints((char*)name.data.get(), static_cast<size_type>(name.size));

    string_type result = text::codepoints_to_string(codepoints.c_str(), codepoints.size());

    return result;
}

bool vfs_string_pool::chars_to_name(const wchar_t* wchars, typename vfs_string_pool::name_type& name)
{
    name.size = 0;
    name.data = nullptr;
    name.ref_count = 0;

    std::shared_ptr<char[]> chars;
    std::size_t chars_count;

    bool result = text::wchars_to_chars(wchars, wcslen(wchars), chars, chars_count);

    if(result)
    {
        name.size = static_cast<length_type>(chars_count);

        auto utf8_bytes = std::reinterpret_pointer_cast<byte>(chars);

        name.data.swap(utf8_bytes);
    }

    return result;
}

bool vfs_string_pool::codepoints_to_name(const cps_type& cps, typename vfs_string_pool::name_type& name)
{
    name.size = 0;
    name.data = nullptr;
    name.ref_count = 0;

    std::shared_ptr<char[]> chars;
    std::size_t chars_count;

    bool result = text::codepoints_to_chars(cps.c_str(), cps.size(), chars, chars_count);

    if(result)
    {
        name.size = static_cast<length_type>(chars_count);

        auto utf8_bytes = std::reinterpret_pointer_cast<byte>(chars);

        name.data.swap(utf8_bytes);
    }

    return result;
}

END_NAMESPACE
