//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FACTORY_H__
#define __FACTORY_H__

#pragma once

BEGIN_NAMESPACE(core)

class factory : private noncopyable
{
    public:
        template <typename T, typename ... TArgs>
        static std::shared_ptr<T> create(const TArgs& ... args);
};

template <typename T, typename ... TArgs>
inline static std::shared_ptr<T> factory::create(const TArgs& ... args)
{
    return std::make_shared<T>(args...);
}

END_NAMESPACE

#endif // __FACTORY_H__
