//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COUNTER_H__
#define __COUNTER_H__

#pragma once

BEGIN_NAMESPACE(core)

class counter : private noncopyable
{
    public:
        using size_type = uint32_t;

    private:
        size_type   my_count;

    public:
        explicit    counter(size_type count = 0);

        size_type   number();
        size_type   value();

        void        rewind();
        void        reset(size_type count = 0);
};

inline counter::counter(typename counter::size_type count)
              : my_count(count)
{
}

inline typename counter::size_type counter::number()
{
    return my_count++;
}

inline typename counter::size_type counter::value()
{
    return my_count;
}

inline void counter::rewind()
{
    my_count--;
}

inline void counter::reset(typename counter::size_type count)
{
    my_count = count;
}

END_NAMESPACE

#endif // __COUNTER_H__
