//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FLAGS_H__
#define __FLAGS_H__

#pragma once

BEGIN_NAMESPACE(core)

// Based on:
//  Copyright (c) 2014 Dmitry Arkhipov
//  https://github.com/grisumbras/enum-flags/blob/master/include/flags/flags.hpp
// AA: the simplified version to be able using inside classes ...
template <class T, class Enabler = void> struct is_flags : public std::false_type
{
};

template <typename T> class tmpl_flags
{
    public:
        using enum_type = typename std::decay<T>::type;
        using underlying_type = typename std::underlying_type<enum_type>::type;

    private:
        underlying_type my_value;

    public:
        tmpl_flags() noexcept = default;

        tmpl_flags(const tmpl_flags& other) noexcept = default;
        tmpl_flags(tmpl_flags&& other) noexcept = default;

        constexpr tmpl_flags(enum_type other) noexcept
            : my_value(static_cast<underlying_type>(other))
        {
        }

        constexpr tmpl_flags(underlying_type other) noexcept
            : my_value(other)
        {
        }

        tmpl_flags& operator = (const tmpl_flags& other) noexcept = default;
        tmpl_flags& operator = (tmpl_flags&& other) noexcept= default;

        tmpl_flags& operator = (enum_type other) noexcept
        {
            my_value = static_cast<underlying_type>(other);
            return *this;
        }

        tmpl_flags& operator = (underlying_type other) noexcept
        {
            my_value = other;
            return *this;
        }

        constexpr underlying_type underlying_value() const noexcept
        {
            return static_cast<underlying_type>(my_value);
        }

        constexpr explicit operator bool() const noexcept
        {
            return my_value;
        }

        constexpr bool operator ! () const noexcept
        {
            return !my_value;
        }

        friend constexpr bool operator == (const tmpl_flags& lhs, const tmpl_flags& rhs)
        {
            return lhs.my_value == rhs.my_value;
        }

        friend constexpr bool operator != (const tmpl_flags& lhs, const tmpl_flags& rhs)
        {
            return lhs.my_value != rhs.my_value;
        }

        tmpl_flags& operator |= (enum_type other) noexcept
        {
            my_value |= static_cast<underlying_type>(other);
            return *this;
        }

        tmpl_flags& operator |= (underlying_type other) noexcept
        {
            my_value |= other;
            return *this;
        }

        friend constexpr tmpl_flags operator | (const tmpl_flags& lhs, const tmpl_flags& rhs) noexcept
        {
            return tmpl_flags(static_cast<underlying_type>(lhs.my_value | rhs.my_value));
        }

        friend constexpr tmpl_flags operator | (enum_type lhs, enum_type rhs) noexcept
        {
            return tmpl_flags(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs));
        }

        tmpl_flags& operator &= (enum_type other) noexcept
        {
            my_value &= static_cast<underlying_type>(other);
            return *this;
        }

        tmpl_flags& operator &= (underlying_type other) noexcept
        {
            my_value &= other;
            return *this;
        }

        friend constexpr tmpl_flags operator & (const tmpl_flags& lhs, const tmpl_flags& rhs) noexcept
        {
            return tmpl_flags(static_cast<underlying_type>(lhs.my_value & rhs.my_value));
        }

        friend constexpr tmpl_flags operator & (enum_type lhs, enum_type rhs) noexcept
        {
            return tmpl_flags(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs));
        }

        tmpl_flags& operator ^= (enum_type other) noexcept
        {
            my_value &= static_cast<underlying_type>(other);
            return *this;
        }

        tmpl_flags& operator ^= (underlying_type other) noexcept
        {
            my_value &= other;
            return *this;
        }

        friend constexpr tmpl_flags operator ^ (const tmpl_flags& lhs, const tmpl_flags& rhs) noexcept
        {
            return tmpl_flags(static_cast<underlying_type>(lhs.my_value ^ rhs.my_value));
        }

        friend constexpr tmpl_flags operator ^ (enum_type lhs, enum_type rhs) noexcept
        {
            return tmpl_flags(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs));
        }

        constexpr tmpl_flags operator ~ () const noexcept
        {
            return tmpl_flags(~my_value);
        }

        tmpl_flags& modify_flags(enum_type add, enum_type remove)
        {
            my_value = ((my_value & ~static_cast<underlying_type>(remove)) | static_cast<underlying_type>(add));
            return *this;
        }
};

template <class T>
constexpr auto operator | (const T& lhs, const T& rhs) noexcept -> typename std::enable_if<is_flags<T>::value, tmpl_flags<T>>::type
{
    return tmpl_flags<T>(lhs) | rhs;
}

template <class T>
constexpr auto operator | (typename tmpl_flags<T>::enum_type lhs, typename tmpl_flags<T>::enum_type rhs) noexcept -> typename std::enable_if<is_flags<T>::value, tmpl_flags<T>>::type
{
    return tmpl_flags<T>(lhs) | rhs;
}

template <class T>
constexpr auto operator & (const T& lhs, const T& rhs) noexcept -> typename std::enable_if<is_flags<T>::value, tmpl_flags<T>>::type
{
    return tmpl_flags<T>(lhs) & rhs;
}

template <class T>
constexpr auto operator & (typename tmpl_flags<T>::enum_type lhs, typename tmpl_flags<T>::enum_type rhs) noexcept -> typename std::enable_if<is_flags<T>::value, tmpl_flags<T>>::type
{
    return tmpl_flags<T>(lhs) & rhs;
}

template <class T>
constexpr auto operator ^ (const T& lhs, const T& rhs) noexcept -> typename std::enable_if<is_flags<T>::value, tmpl_flags<T>>::type
{
    return tmpl_flags<T>(lhs) ^ rhs;
}

template <class T>
constexpr auto operator ^ (typename tmpl_flags<T>::enum_type lhs, typename tmpl_flags<T>::enum_type rhs) noexcept -> typename std::enable_if<is_flags<T>::value, tmpl_flags<T>>::type
{
    return tmpl_flags<T>(lhs) ^ rhs;
}

// this macro works only for non-template entities
#define DECLARE_NATIVE_ENUM_OPERATORS(__ENUM_TYPE__)                                \
constexpr auto operator | (__ENUM_TYPE__ lhs, __ENUM_TYPE__ rhs) noexcept           \
{                                                                                   \
    using T = std::underlying_type_t<__ENUM_TYPE__>;                                \
    return static_cast<__ENUM_TYPE__>(static_cast<T>(lhs) | static_cast<T>(rhs));   \
}                                                                                   \
constexpr auto operator & (__ENUM_TYPE__ lhs, __ENUM_TYPE__ rhs) noexcept           \
{                                                                                   \
    using T = std::underlying_type_t<__ENUM_TYPE__>;                                \
    return static_cast<__ENUM_TYPE__>(static_cast<T>(lhs) & static_cast<T>(rhs));   \
}                                                                                   \
constexpr auto operator ^ (__ENUM_TYPE__ lhs, __ENUM_TYPE__ rhs) noexcept           \
{                                                                                   \
    using T = std::underlying_type_t<__ENUM_TYPE__>;                                \
    return static_cast<__ENUM_TYPE__>(static_cast<T>(lhs) ^ static_cast<T>(rhs));   \
}                                                                                   \
constexpr auto operator ~ (const __ENUM_TYPE__& other)                              \
{                                                                                   \
    using T = std::underlying_type_t<__ENUM_TYPE__>;                                \
    return static_cast<__ENUM_TYPE__>(~static_cast<T>(other));                      \
}

END_NAMESPACE

#endif // __FLAGS_H__
