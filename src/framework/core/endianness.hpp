//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ENDIANNESS_H__
#define __ENDIANNESS_H__

#pragma once

BEGIN_NAMESPACE(core)

#ifdef _WINDOWS_
#   define swap_bytes_uint16(x) (static_cast<unsigned short>(_byteswap_ushort(x)))
#else
#   define swap_bytes_uint16(x) ((((uint16_t)(x) & 0xFF00) >> 8) | \
                                 (((uint16_t)(x) & 0x00FF) << 8))
#endif

#ifdef _WINDOWS_
#   define swap_bytes_uint32(x) (static_cast<unsigned long>(_byteswap_ulong(x)))
#else
#   define swap_bytes_uint32(x) ((((uint32_t)(x) & 0xFF000000) >> 24) | \
                                 (((uint32_t)(x) & 0x00FF0000) >> 8)  | \
                                 (((uint32_t)(x) & 0x0000FF00) << 8)  | \
                                 (((uint32_t)(x) & 0x000000FF) << 24))
#endif

#ifdef _WINDOWS_
#   define swap_bytes_uint64(x) (static_cast<__int64>(_byteswap_uint64(x)))
#else
#   define swap_bytes_uint64(x) ((((uint64_t)(x) & 0xFF00000000000000ULL) >> 56) | \
                                 (((uint64_t)(x) & 0x00FF000000000000ULL) >> 40) | \
                                 (((uint64_t)(x) & 0x0000FF0000000000ULL) >> 24) | \
                                 (((uint64_t)(x) & 0x000000FF00000000ULL) >> 8)  | \
                                 (((uint64_t)(x) & 0x00000000FF000000ULL) << 8)  | \
                                 (((uint64_t)(x) & 0x0000000000FF0000ULL) << 24) | \
                                 (((uint64_t)(x) & 0x000000000000FF00ULL) << 40) | \
                                 (((uint64_t)(x) & 0x00000000000000FFULL) << 56))
#endif

#define swap_bytes_float(x)  (swap_bytes_type<float>(x))
#define swap_bytes_double(x) (swap_bytes_type<double>(x))

class endianness : public singleton<endianness>
{
    public:
        enum class endian : uint8_t
        {
            unknown_endian = 0,
            little_endian  = 1,
            big_endian     = 2
        };

        DECLARE_ENUM_OPERATORS(endian)

        using endian_type = endian;

    private:
        endian_type my_endian; // host endianness

    public:
                    endianness();

    static bool     little_endian();
    static bool     big_endian();
        
    template <typename T>
    static T        swap_bytes_type(T value);

    uint16_t        le16_to_host(uint16_t value) const;
    uint32_t        le32_to_host(uint32_t value) const;
    uint64_t        le64_to_host(uint64_t value) const;
    float           leflt_to_host(float value) const;
    double          ledbl_to_host(double value) const;

    uint16_t        be16_to_host(uint16_t value) const;
    uint32_t        be32_to_host(uint32_t value) const;
    uint64_t        be64_to_host(uint64_t value) const;
    float           beflt_to_host(float value) const;
    double          bedbl_to_host(double value) const;

    uint16_t        host_to_le16(uint16_t value) const;
    uint32_t        host_to_le32(uint32_t value) const;
    uint64_t        host_to_le64(uint64_t value) const;
    float           host_to_leflt(float value) const;
    double          host_to_ledbl(double value) const;

    uint16_t        host_to_be16(uint16_t value) const;
    uint32_t        host_to_be32(uint32_t value) const;
    uint64_t        host_to_be64(uint64_t value) const;
    float           host_to_beflt(float value) const;
    double          host_to_bedbl(double value) const;
};

inline endianness::endianness()
                 : my_endian(little_endian() ? endian::little_endian : endian::big_endian)
{
}

inline bool endianness::little_endian()
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

inline bool endianness::big_endian()
{
    return !little_endian();
}

template <typename T>
T endianness::swap_bytes_type(T value)
{
    T result = {};

    byte *pval(reinterpret_cast<byte*>(&value));
    byte *pres(reinterpret_cast<byte*>(&result));

    for(int i = 0, j = sizeof(T) - 1; j >= 0;)
    {
        pres[i++] = pval[j--];
    }

    return result;
}

inline uint16_t endianness::le16_to_host(uint16_t value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_uint16(value);
}

inline uint32_t endianness::le32_to_host(uint32_t value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_uint32(value);
}

inline uint64_t endianness::le64_to_host(uint64_t value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_uint64(value);
}

inline float endianness::leflt_to_host(float value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_type<float>(value);
}

inline double endianness::ledbl_to_host(double value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_type<double>(value);
}

inline uint16_t endianness::be16_to_host(uint16_t value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_uint16(value);
}

inline uint32_t endianness::be32_to_host(uint32_t value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_uint32(value);
}

inline uint64_t endianness::be64_to_host(uint64_t value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_uint64(value);
}

inline float endianness::beflt_to_host(float value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_type<float>(value);
}

inline double endianness::bedbl_to_host(double value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_type<double>(value);
}

inline uint16_t endianness::host_to_le16(uint16_t value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_uint16(value);
}

inline uint32_t endianness::host_to_le32(uint32_t value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_uint32(value);
}

inline uint64_t endianness::host_to_le64(uint64_t value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_uint64(value);
}

inline float endianness::host_to_leflt(float value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_type<float>(value);
}

inline double endianness::host_to_ledbl(double value) const
{
    return my_endian == endian::little_endian ? value : swap_bytes_type<double>(value);
}

inline uint16_t endianness::host_to_be16(uint16_t value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_uint16(value);
}

inline uint32_t endianness::host_to_be32(uint32_t value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_uint32(value);
}

inline uint64_t endianness::host_to_be64(uint64_t value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_uint64(value);
}

inline float endianness::host_to_beflt(float value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_type<float>(value);
}

inline double endianness::host_to_bedbl(double value) const
{
    return my_endian == endian::big_endian ? value : swap_bytes_type<double>(value);
}


#define READ_BYTE(__n, __buffer, __offset)          \
{                                                   \
    byte n = (__n);                                 \
    byte* p = (byte*)(&n);                          \
    *(p + 0) = (__buffer)[0 + (__offset)];          \
    (__offset) += sizeof(byte);                     \
}

#define WRITE_BYTE(__n, __buffer, __offset)         \
{                                                   \
    byte n = (__n);                                 \
    byte* p = (byte*)(&n);                          \
    (__buffer)[0 + (__offset)] = *(p + 0);          \
    (__offset) += sizeof(int8);                     \
}

#define READ_BYTES(__b, __buffer, __sz, __offset)   \
{                                                   \
    memcpy((__b), (__buffer) + (__offset), (__sz)); \
    (__offset) += (__sz);                           \
}

#define WRITE_BYTES(__b, __buffer, __sz, __offset)  \
{                                                   \
    memcpy((__buffer) + (__offset), (__b), (__sz)); \
    (__offset) += (__sz);                           \
}

#define READ16_LE(__n, __buffer, __offset)          \
{                                                   \
    uint16_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    *(p + 1) = (__buffer)[0 + (__offset)];          \
    *(p + 0) = (__buffer)[1 + (__offset)];          \
    (__offset) += sizeof(uint16_t);                 \
}

#define READ16_BE(__n, __buffer, __offset)          \
{                                                   \
    uint16_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    *(p + 0) = (__buffer)[0 + (__offset)];          \
    *(p + 1) = (__buffer)[1 + (__offset)];          \
    (__offset) += sizeof(uint16_t);                 \
}

#define WRITE16_LE(__n, __buffer, __offset)         \
{                                                   \
    uint16_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    (__buffer)[0 + (__offset)] = *(p + 1);          \
    (__buffer)[1 + (__offset)] = *(p + 0);          \
    (__offset) += sizeof(uint16_t);                 \
}

#define WRITE16_BE(__n, __buffer, __offset)         \
{                                                   \
    uint16_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    (__buffer)[0 + (__offset)] = *(p + 0);          \
    (__buffer)[1 + (__offset)] = *(p + 1);          \
    (__offset) += sizeof(uint16_t);                 \
}

#define READ32_LE(__n, __buffer, __offset)          \
{                                                   \
    uint32_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    *(p + 3) = (__buffer)[0 + (__offset)];          \
    *(p + 2) = (__buffer)[1 + (__offset)];          \
    *(p + 1) = (__buffer)[2 + (__offset)];          \
    *(p + 0) = (__buffer)[3 + (__offset)];          \
    (__offset) += sizeof(uint32_t);                 \
}

#define READ32_BE(__n, __buffer, __offset)          \
{                                                   \
    uint32_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    *(p + 0) = (__buffer)[0 + (__offset)];          \
    *(p + 1) = (__buffer)[1 + (__offset)];          \
    *(p + 2) = (__buffer)[2 + (__offset)];          \
    *(p + 3) = (__buffer)[3 + (__offset)];          \
    (__offset) += sizeof(uint32_t);                 \
}

#define WRITE32_LE(__n, __buffer, __offset)         \
{                                                   \
    uint32_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    (__buffer)[0 + (__offset)] = *(p + 3);          \
    (__buffer)[1 + (__offset)] = *(p + 2);          \
    (__buffer)[2 + (__offset)] = *(p + 1);          \
    (__buffer)[3 + (__offset)] = *(p + 0);          \
    (__offset) += sizeof(uint32_t);                 \
}

#define WRITE32_BE(__n, __buffer, __offset)         \
{                                                   \
    uint32_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    (__buffer)[0 + (__offset)] = *(p + 0);          \
    (__buffer)[1 + (__offset)] = *(p + 1);          \
    (__buffer)[2 + (__offset)] = *(p + 2);          \
    (__buffer)[3 + (__offset)] = *(p + 3);          \
    (__offset) += sizeof(uint32_t);                 \
}

#define READ64_LE(__n, __buffer, __offset)          \
{                                                   \
    uint64_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    *(p + 7) = (__buffer)[0 + (__offset)];          \
    *(p + 6) = (__buffer)[1 + (__offset)];          \
    *(p + 5) = (__buffer)[2 + (__offset)];          \
    *(p + 4) = (__buffer)[3 + (__offset)];          \
    *(p + 3) = (__buffer)[4 + (__offset)];          \
    *(p + 2) = (__buffer)[5 + (__offset)];          \
    *(p + 1) = (__buffer)[6 + (__offset)];          \
    *(p + 0) = (__buffer)[7 + (__offset)];          \
    (__offset) += sizeof(uint64_t);                 \
}

#define READ64_BE(__n, __buffer, __offset)          \
{                                                   \
    uint64_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    *(p + 0) = (__buffer)[0 + (__offset)];          \
    *(p + 1) = (__buffer)[1 + (__offset)];          \
    *(p + 2) = (__buffer)[2 + (__offset)];          \
    *(p + 3) = (__buffer)[3 + (__offset)];          \
    *(p + 4) = (__buffer)[4 + (__offset)];          \
    *(p + 5) = (__buffer)[5 + (__offset)];          \
    *(p + 6) = (__buffer)[6 + (__offset)];          \
    *(p + 7) = (__buffer)[7 + (__offset)];          \
    (__offset) += sizeof(uint64_t);                 \
}

#define WRITE64_LE(__n, __buffer, __offset)         \
{                                                   \
    uint64_t n = (__n);                             \
    byte* p = (byte*)(&n)  ;                        \
    (__buffer)[0 + (__offset)] = *(p + 7);          \
    (__buffer)[1 + (__offset)] = *(p + 6);          \
    (__buffer)[2 + (__offset)] = *(p + 5);          \
    (__buffer)[3 + (__offset)] = *(p + 4);          \
    (__buffer)[4 + (__offset)] = *(p + 3);          \
    (__buffer)[5 + (__offset)] = *(p + 2);          \
    (__buffer)[6 + (__offset)] = *(p + 1);          \
    (__buffer)[7 + (__offset)] = *(p + 0);          \
    (__offset) += sizeof(uint64_t);                 \
}

#define WRITE64_BE(__n, __buffer, __offset)         \
{                                                   \
    uint64_t n = (__n);                             \
    byte* p = (byte*)(&n);                          \
    (__buffer)[0 + (__offset)] = *(p + 0);          \
    (__buffer)[1 + (__offset)] = *(p + 1);          \
    (__buffer)[2 + (__offset)] = *(p + 2);          \
    (__buffer)[3 + (__offset)] = *(p + 3);          \
    (__buffer)[4 + (__offset)] = *(p + 4);          \
    (__buffer)[5 + (__offset)] = *(p + 5);          \
    (__buffer)[6 + (__offset)] = *(p + 6);          \
    (__buffer)[7 + (__offset)] = *(p + 7);          \
    (__offset) += sizeof(uint64_t);                 \
}

END_NAMESPACE

#endif // __ENDIANNESS_H__
