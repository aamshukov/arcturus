//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ENUMERATE_H__
#define __ENUMERATE_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename T>
struct enumerate_wrapper
{
    using iterator_type = std::conditional_t<std::is_const_v<T>, typename T::const_iterator, typename T::iterator>;
    using pointer_type = std::conditional_t<std::is_const_v<T>, typename T::const_pointer, typename T::pointer>;
    using reference_type = std::conditional_t<std::is_const_v<T>, typename T::const_reference, typename T::reference>;

    T& container;

    constexpr enumerate_wrapper(T& c) : container(c)
    {
    }

    struct enumerate_wrapper_iter
    {
        std::size_t index;

        iterator_type value;

        bool operator!=(const iterator_type& other) const
        {
            return value != other;
        }

        enumerate_wrapper_iter& operator ++ ()
        {
            ++index;
            ++value;

            return *this;
        }

        constexpr std::pair<size_t, reference_type> operator * ()
        {
            return std::pair<size_t, reference_type>{index, *value};
        }
    };

    constexpr enumerate_wrapper_iter begin()
    {
        return { 0, std::begin(container) };
    }

    constexpr iterator_type end()
    {
        return std::end(container);
    }
};

template <typename T>
constexpr auto enumerate(T& c)
{
    return enumerate_wrapper<T>(c);
}

END_NAMESPACE

#endif // __ENUMERATE_H__
