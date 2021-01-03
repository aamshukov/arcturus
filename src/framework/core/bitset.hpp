//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __BITSET_H__
#define __BITSET_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename T = uint64_t>
class bitset : private noncopyable
{
    public:
        class bit
        {
            friend class bitset;

            private:
                bitset*     my_bitset;   // referenced bitset
                size_type   my_position; // position in bitset

            private:
                            bit();
                            bit(bitset& bs, size_type position);

            public:
                           ~bit();

                bit&        operator = (bool value);
                bit&        operator = (const bit& other);

                            operator bool () const;

                bit&        flip();
        };

    private:
        using data_type = T;
        using bits_type = std::unique_ptr<data_type[]>;

        static constexpr size_type chunk_size = bits_in_byte * sizeof(data_type); // in bits

        bits_type       my_bits;        // bit array - sequence of chunks of T

        size_type       my_size;        // how many bits
        size_type       my_capacity;    // how many chunks

    private:
        void            adjust();

    public:
                        bitset();
        explicit        bitset(size_type size);
                        bitset(const bitset& other);
                       ~bitset();

        size_type       size() const;

        const bitset&   operator = (const bitset& other);

        bool            operator [] (size_type position) const;
        bit             operator [] (size_type position);

        bitset&         operator &= (const bitset& other);
        bitset&         operator |= (const bitset& other);
        bitset&         operator ^= (const bitset& other);

        bitset&         set();
        bitset&         set(size_type position, bool value = true);

        bitset&         reset();
        bitset&         reset(size_type position);

        bitset&         flip();
        bitset&         flip(size_type position);

        string_type     to_string() const;
};

template <typename T>
inline bitset<T>::bitset()
                : my_size(0), my_capacity(0)
{
}

template <typename T>
inline bitset<T>::bitset(size_type size)
{
    my_size = size;
    my_capacity = (size == 0 ? 0 : size / chunk_size) + 1;
    my_capacity = calculate_alignment(my_capacity, sizeof(data_type));
    my_bits = std::make_unique<data_type[]>(my_capacity);

    reset();
}

template <typename T>
inline bitset<T>::bitset(const bitset<T>& other)
                : my_size(0), my_capacity(0)
{
    if(this != &other)
    {
        my_size = other.my_size;
        my_capacity = other.my_capacity;
        my_bits = std::make_unique<data_type[]>(my_capacity);

        std::memcpy(my_bits.get(), other.my_bits.get(), my_capacity * sizeof(data_type));

        reset();
    }
}

template <typename T>
inline bitset<T>::~bitset()
{
}

template <typename T>
inline size_type bitset<T>::size() const
{
    return my_size;
}

template <typename T>
inline const bitset<T>& bitset<T>::operator = (const bitset<T>& other)
{
    if(this != &other)
    {
        my_size = other.my_size;
        my_capacity = other.my_capacity;
        my_bits = std::make_unique<data_type[]>(my_capacity);

        std::memcpy(my_bits.get(), other.my_bits.get(), my_capacity * sizeof(data_type));

        reset();
    }

    return *this;
}

template <typename T>
inline bool bitset<T>::operator [] (size_type position) const
{
    return (my_bits[position / chunk_size] & (data_type(1) << position % chunk_size)) != 0;
}

template <typename T>
inline typename bitset<T>::bit bitset<T>::operator [] (size_type position)
{
    return bit(*this, position);
}

template <typename T>
bitset<T>& bitset<T>::operator &= (const bitset<T>& other)
{
    if(this != &other)
    {
        for(size_type k = 0; k < my_capacity; k++)
        {
            my_bits[k] &= other.my_bits[k];
        }
    }

    return *this;
}

template <typename T>
bitset<T>& bitset<T>::operator |= (const bitset<T>& other)
{
    if(this != &other)
    {
        for(size_type k = 0; k < my_capacity; k++)
        {
            my_bits[k] |= other.my_bits[k];
        }
    }

    return *this;
}

template <typename T>
bitset<T>& bitset<T>::operator ^= (const bitset<T>& other)
{
    if(this != &other)
    {
        for(size_type k = 0; k < my_capacity; k++)
        {
            my_bits[k] ^= other.my_bits[k];
        }
    }

    return *this;
}

template <typename T>
bitset<T>& bitset<T>::set()
{
    std::memset(my_bits.get(), 0xFF, my_capacity * sizeof(data_type));

    adjust();

    return *this;
}

template <typename T>
inline bitset<T>& bitset<T>::set(size_type position, bool value)
{
    data_type& chunk(my_bits[position / chunk_size]);

    data_type mask = data_type(1) << position % chunk_size;

    if(value)
        chunk |= mask;
    else
        chunk &= ~mask;

    return *this;
}

template <typename T>
inline bitset<T>& bitset<T>::reset()
{
    std::memset(my_bits.get(), 0x00, my_capacity * sizeof(data_type));
    return *this;
}

template <typename T>
inline bitset<T>& bitset<T>::reset(size_type position)
{
    return set(position, false);
}

template <typename T>
bitset<T>& bitset<T>::flip()
{
    for(size_type k = 0; k < my_capacity; k++)
    {
        my_bits[k] = ~my_bits[k];
    }

    adjust();

    return *this;
}

template <typename T>
inline bitset<T>& bitset<T>::flip(size_type position)
{
    my_bits[position / chunk_size] ^= data_type(1) << position % chunk_size;
    return *this;
}

template <typename T>
inline void bitset<T>::adjust()
{
    if(my_size == 0 || (my_size % chunk_size) != 0)
    {
        // with aligned memory my_capacity might be large than capacity ...
        auto capacity = (my_size == 0 ? 0 : my_size / chunk_size) + 1;
        auto delta = my_capacity - capacity;

        // ... zero out trailing chunks
        for(auto k = 0; k < delta; k++)
        {
            my_bits[my_capacity - 1 - k] = data_type(0);
        }

        // ... zero out real capacity's chunk
        my_bits[capacity - 1] &= (data_type(1) << (my_size % chunk_size)) - 1;
    }
}

template <typename T>
string_type bitset<T>::to_string() const
{
    string_type result;

    result.reserve(my_size);

    for(int k = static_cast<int>(my_size) - 1; k >= 0; k--) // reversed loop to mimic LSB on the left ...
    {
        result.push_back((*this)[k] ? L'1' : L'0');
    }

    return result;
}

template <typename T>
inline bitset<T>::bit::bit()
                : my_bitset(nullptr), my_position(0)
{
}

template <typename T>
inline bitset<T>::bit::bit(bitset<T>& bs, size_type position)
                : my_bitset(&bs), my_position(position)
{
}

template <typename T>
inline bitset<T>::bit::~bit()
{
}

template <typename T>
inline typename bitset<T>::bit& bitset<T>::bit::operator = (bool value)
{
    (*my_bitset).set(my_position, value);
    return *this;
}

template <typename T>
inline typename bitset<T>::bit& bitset<T>::bit::operator = (const bitset<T>::bit& other)
{
    (*my_bitset).set(my_position, static_cast<bool>(other));
    return *this;
}

template <typename T>
inline bitset<T>::bit::operator bool () const
{
    return ((*my_bitset).my_bits[my_position / chunk_size] & (data_type(1) << my_position % chunk_size)) != 0;
}

template <typename T>
inline typename bitset<T>::bit& bitset<T>::bit::flip()
{
    (*my_bitset).flip(my_position);
    return *this;
}

template <typename T>
bitset<T> operator & (const bitset<T>& lhs, const bitset<T>& rhs)
{
    bitset<T> result(lhs);
    return result &= rhs;
}

template <typename T>
bitset<T> operator | (const bitset<T>& lhs, const bitset<T>& rhs)
{
    bitset<T> result(lhs);
    return result |= rhs;
}

template <typename T>
bitset<T> operator ^ (const bitset<T>& lhs, const bitset<T>& rhs)
{
    bitset<T> result(lhs);
    return result ^= rhs;
}

END_NAMESPACE

#endif // __BITSET_H__
