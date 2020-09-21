//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename T>
struct singleton : private noncopyable
{
    static T& instance()
    {
        static T result;
        return result;
    }
};

END_NAMESPACE

#endif // __SINGLETON_H__
