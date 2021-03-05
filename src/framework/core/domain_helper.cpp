//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#include <core/pch.hpp>
#include <core/domain_helper.hpp>

BEGIN_NAMESPACE(core)

bool is_little_endian()
{
    static bool result;

    bool set = false;

    if(!set)
    {
        int32_t x = 1;
        result = (*(char*)&x == 1);
        set = true;
    }

    return result;
}

bool is_big_endian()
{
    return !is_little_endian();
}

string_type format(const char_type *format_template, ...)
{
    string_type result;
    
    auto buffer(std::make_shared<std::array<char_type, 4096>>());

    va_list arguments;

    va_start(arguments, format_template);

    vsnwprintf((*buffer).data(), (*buffer).size(), TRUNCATE, format_template, arguments);

    va_end(arguments);

    result.assign((*buffer).data());

    return result;
}

std::wstring string_to_wstring(const std::string& str, const std::locale& locale)
{
    std::wstring result;

    if(!str.empty())
    {
        std::vector<wchar_t> buffer(str.size());

        std::use_facet<std::ctype<wchar_t>>(locale).widen(str.data(), str.data()+str.size(), buffer.data());

        result.assign(buffer.data(), buffer.size());
    }

    return result;
}

const char_type* get_indent(uint8_t count)
{
    const uint8_t size = 128;

    assert(count < size);

    static const char_type spaces[size + 1] = L"                                                                                                                               ";

    return &spaces[(size - 1) - (count & (size - 1))];
}

string_type uniqueue_file_name(const string_type& file_name, const string_type& extension)
{
    string_type result(empty_string());

    time_t current_time;

    time(&current_time);
    time(&current_time);
    time(&current_time);
    time(&current_time);
    time(&current_time);

    std::size_t hash = std::hash<time_t> {} (current_time);

    result = format(L"$-$-arcturus--%s-%I64u-$-$%s", file_name.c_str(), hash, extension.c_str());

    return result;
}

END_NAMESPACE
