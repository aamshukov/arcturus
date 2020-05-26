#ifndef __TYPE_INL__
#define __TYPE_INL__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

//template <typename TReturn, typename TParam>
inline type::type()
{
}

//template <typename TReturn, typename TParam>
inline type::~type()
{
}

//template <typename TReturn, typename TParam>
inline bool type::operator == (const type&)
{
    return false;
}

//template <typename TReturn, typename TParam>
inline bool type::operator != (const type&)
{
    return false;
}

END_NAMESPACE

#endif // __TYPE_INL__
