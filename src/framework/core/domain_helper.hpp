//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __DOMAIN_HELPER_H__
#define __DOMAIN_HELPER_H__

#pragma once

#define snwprintf   _snwprintf_s
#define vsnwprintf  _vsnwprintf_s

#define TRUNCATE    _TRUNCATE

BEGIN_NAMESPACE(core)

#ifdef _WINDOWS_
#   define byte_swap_uint16(x) (static_cast<unsigned short>(_byteswap_ushort(x)))
#else
#   define byte_swap_uint16(x) ((((uint16_t)(x) & 0xFF00) >> 8) | (((uint16_t)(x) & 0x00FF) << 8))
#endif

#ifdef _WINDOWS_
#   define byte_swap_uint32(x) (static_cast<unsigned long>(_byteswap_ulong(x)))
#else
#   define byte_swap_uint32(x) ((((uint32_t)(x) & 0xFF000000) >> 24) | (((uint32_t)(x) & 0x00FF0000) >> 8) | (((uint32_t)(x) & 0x0000FF00) << 8) | (((uint32_t)(x) & 0x000000FF) << 24))
#endif

#ifdef _WINDOWS_
#   define byte_swap_uint64(x) (static_cast<__int64>(_byteswap_uint64(x)))
#else
#   define byte_swap_uint64(x) ((((uint64_t)(x) & 0xFF00000000000000ULL) >> 56) | (((uint64_t)(x) & 0x00FF000000000000ULL) >> 40) | (((uint64_t)(x) & 0x0000FF0000000000ULL) >> 24) | (((uint64_t)(x) & 0x000000FF00000000ULL) >> 8) | (((uint64_t)(x) & 0x00000000FF000000ULL) << 8) | (((uint64_t)(x) & 0x0000000000FF0000ULL) << 24) | (((uint64_t)(x) & 0x000000000000FF00ULL) << 40) | (((uint64_t)(x) & 0x00000000000000FFULL) << 56))
#endif

#define byte_swap_float(x)  (arrange_type<float>(x))
#define byte_swap_double(x) (arrange_type<double>(x))

#define DUMMY_EXCEPTION_TEXT (L"Error occurred.")


template <typename T, std::size_t N>
inline constexpr std::size_t array_size(T const (&)[N])
{
    return N;
}

template <typename T>
static T arrange_type(T value)
{
    T result;

    byte* pval((byte*)&value);
    byte* pres((byte*)&result);

    for(std::size_t i = 0, j = sizeof(T) - 1; j >= 0;)
    {
        pres[i++] = pval[j--];
    }

    return result;
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

template <typename T = size_type>
constexpr auto calculate_alignment(T value, size_type alignment) noexcept
{
    // align value 'value' to boundary 'alignment' which should be power of 2
    return ((value + (alignment - 1)) & ~(alignment - 1));
}


bool is_little_endian();
bool is_big_endian();

string_type format(const char_type* format_template, ...);
std::wstring string_to_wstring(const std::string& str, const std::locale& locale = std::locale(""));

const char_type* get_indent(uint8_t count);

string_type uniqueue_file_name(const string_type& file_name, const string_type& extension);

END_NAMESPACE

#endif // __DOMAIN_HELPER_H__
