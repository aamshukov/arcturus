//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DOMAIN_HELPER_H__
#define __DOMAIN_HELPER_H__

#pragma once

#define snwprintf   _snwprintf_s
#define vsnwprintf  _vsnwprintf_s

#define TRUNCATE    _TRUNCATE

BEGIN_NAMESPACE(core)

#define DUMMY_EXCEPTION_TEXT (L"Error occurred.")


template <typename T, std::size_t N>
inline constexpr std::size_t array_size(T const (&)[N])
{
    return N;
}

template <typename T = std::size_t>
inline T modify_flags(T flags, T add, T remove)
{
    return ((flags & ~remove) | add);
}

template <typename T = std::size_t>
inline T modify_enum_flags(T flags, T add, T remove)
{
    static_assert(std::is_enum<T>::value, "Non enum has been provided.");
    return (T)((static_cast<std::underlying_type_t<T>>(flags) & ~static_cast<std::underlying_type_t<T>>(remove)) | static_cast<std::underlying_type_t<T>>(add));
}

template <typename T = std::size_t>
inline bool is_enum_flag_set(T flag, T flags)
{
    static_assert(std::is_enum<T>::value, "Non enum has been provided.");
    return ((static_cast<std::underlying_type_t<T>>(flags) & static_cast<std::underlying_type_t<T>>(flag)) == static_cast<std::underlying_type_t<T>>(flag));
}

inline const string_type& empty_string()
{
    static string_type result;
    return result;
}

template <typename T>
T factorial(T n)
{
    T result = 1;

    for(T k = 1; k <= n; k++)
    {
        result *= k;
    }

    return result;
}

template<std::size_t N>
void calculate_transitive_closure(bool a[N][N])
{
    static_assert(N > 1, "N must be greater than 1");

    // calculate transitive closure, Warshall's algorithm,
    // Compiler Construction, Barrett, Bates, Gustafson, Couch, p. 199
    std::size_t n = N;

    for(std::size_t i = 0; i < n; i++)
    {
        for(std::size_t j = 0; j < n; j++)
        {
            if(a[j][i])
            {
                for(std::size_t k = 0; k < n; k++)
                {
                    a[j][k] = a[j][k] || a[i][k];
                }
            }
        }
    }
}

template<std::size_t N>
void build_reflexive_transitive_completion(bool a[N][N])
{
    static_assert(N > 1, "N must be greater than 1");

    std::size_t n = N;

    for(std::size_t i = 0; i < n; i++)
    {
        a[i][i] = true;
    }
}

template<std::size_t N>
void transpose(bool a[N][N], bool b[N][N])
{
    static_assert(N > 1, "N must be greater than 1");

    std::size_t n = N;

    for(std::size_t i = 0; i < n; i++)
    {
        for(std::size_t j = 0; j < n; j++)
        {
            b[i][j] = a[j][i];
        }
    }
}

template<std::size_t N>
void multiply(bool a[N][N], bool b[N][N], bool c[N][N])
{
    static_assert(N > 1, "N must be greater than 1");

    for(std::size_t i = 0; i < N; i++)
    {
        for(std::size_t j = 0; j < N; j++)
        {
            bool value = false;

            for(std::size_t k = 0; k < N; k++)
            {
                value |= a[i][k] && b[k][j];

                if(value)
                {
                    break;
                }
            }

            c[i][j] = value;
        }
    }
}

template <typename T = std::size_t>
constexpr auto calculate_alignment_up(T value, std::size_t alignment) noexcept
{
    // align value 'value' to boundary 'alignment' which should be power of 2
    return T((value + (T(alignment) - 1)) & ~T(alignment - 1)); // up
}

template <typename T = std::size_t>
constexpr auto calculate_alignment_down(T value, std::size_t alignment) noexcept
{
    // align value 'value' to boundary 'alignment' which should be power of 2
    return T(value & ~T(alignment - 1)); // down
}

template <typename T>
constexpr T integer_log2(T n)
{
    return ((n < 2) ? 1 : 1 + integer_log2<T>(n / 2));
}

inline std::size_t combine_hash(std::size_t number)
{
    return std::hash<std::size_t>{}(number + 0x9E3779B9 + (number << 6) + (number >> 2)); // aka boost hash_combine
}

template <typename T>
inline bool real_number_equal(T n1, T n2)
{
    return std::abs(n1 - n2) < std::numeric_limits<T>::epsilon();
}

string_type format(const char_type* format_template, ...);
std::wstring string_to_wstring(const std::string& str, const std::locale& locale = std::locale(""));

const char_type* get_indent(uint8_t count);

string_type uniqueue_file_name(const string_type& file_name, const string_type& extension);

END_NAMESPACE

#endif // __DOMAIN_HELPER_H__
