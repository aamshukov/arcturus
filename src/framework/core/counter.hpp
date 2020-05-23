//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __COUNTER_H__
#define __COUNTER_H__

#pragma once

BEGIN_NAMESPACE(core)

class counter : private noncopyable
{
    private:
        uint32_t    my_count;

    public:
        explicit    counter(uint32_t count = 0);

        uint32_t    number();
        uint32_t    value();

        void        rewind();
        void        reset(uint32_t count = 0);
};

inline counter::counter(uint32_t count)
              : my_count(count)
{
}

inline uint32_t counter::number()
{
    return my_count++;
}

inline uint32_t counter::value()
{
    return my_count;
}

inline void counter::rewind()
{
    my_count--;
}

inline void counter::reset(uint32_t count)
{
    my_count = count;
}

END_NAMESPACE

#endif // __COUNTER_H__
