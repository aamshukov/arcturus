//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COUNTER_H__
#define __COUNTER_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename T = std::size_t>
class counter : private noncopyable
{
    public:
        using integer_type = T;

    private:
        std::atomic<integer_type>
                    my_count;
    public:
        explicit    counter(T count = 0);

        T           number();
        T           value();

        void        rewind();
        void        reset(T count = 0);
};

template <typename T>
inline counter<T>::counter(T count)
                 : my_count(count)
{
}

template <typename T>
inline T counter<T>::number()
{
    return my_count++;
}

template <typename T>
inline T counter<T>::value()
{
    return my_count;
}

template <typename T>
inline void counter<T>::rewind()
{
    my_count--;
}

template <typename T>
inline void counter<T>::reset(T count)
{
    my_count = count;
}

END_NAMESPACE

#endif // __COUNTER_H__
