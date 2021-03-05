//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __FACTORY_H__
#define __FACTORY_H__

#pragma once

BEGIN_NAMESPACE(core)

class factory : private noncopyable
{
    public:
        template <typename T, typename ... ARGS>
        static std::shared_ptr<T> create(const ARGS& ... args);
};

template <typename T, typename ... ARGS>
inline static std::shared_ptr<T> factory::create(const ARGS& ... args)
{
    return std::make_shared<T>(args...);
}

END_NAMESPACE

#endif // __FACTORY_H__
