//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ENUM_H__
#define __ENUM_H__

#pragma once

BEGIN_NAMESPACE(core)

#define DECLARE_ENUM(E, T, ...)                             \
    enum class E : T                                        \
    {                                                       \
        __VA_ARGS__                                         \
    };                                                      \
    using enum_map_type = std::map<E, string_type>;         \
                                                            \
    static enum_map_type mapping;                           \
                                                            \
    static void initialize()                                \
    {                                                       \
        mapping = parse_enum<E>(L#__VA_ARGS__);             \
    }                                                       \
                                                            \
    static string_type name(const E& token_type)            \
    {                                                       \
        auto it(mapping.find(token_type));                  \
                                                            \
        if(it != mapping.end())                             \
        {                                                   \
            return (*it).second;                            \
        }                                                   \
        else                                                \
        {                                                   \
            return empty_string();                          \
        }                                                   \
    }                                                       \
                                                            \
    static E value(const string_type& token_name)           \
    {                                                       \
        E result((E)0);                                     \
                                                            \
        for(const auto& kvp : mapping)                      \
        {                                                   \
            if(kvp.second == token_name)                    \
            {                                               \
                result = kvp.first;                         \
                break;                                      \
            }                                               \
        }                                                   \
                                                            \
        return result;                                      \
    }

template <typename T>
static std::map<T, string_type> parse_enum(const string_type& values)
{
    std::map<T, string_type> result;

    std::vector<string_type> parts;

    text::split(values, L",", parts);

    size_type k = 0;

    for(const auto& part : parts)
    {
        std::vector<string_type> kvp;

        text::split(part, L"=", kvp);

        string_type value;
        text::trim(kvp[0], L" \t", value);

        if(kvp.size() > 1)
        {
            string_type key;
            text::trim(kvp[1], L" \t", key);
            char_type* end;
            k = std::wcstol(key.c_str(), &end, 10);
        }

        result[static_cast<T>(k)] = value;

        k++;
    }

    return result;
}

END_NAMESPACE

#endif // __ENUM_H__
