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
        std::atomic<std::size_t>
                    my_count;
    public:
        explicit    counter(std::size_t count = 0);

        std::size_t number();
        std::size_t value();

        void        rewind();
        void        reset(std::size_t count = 0);
};

inline counter::counter(std::size_t count)
              : my_count(count)
{
}

inline std::size_t counter::number()
{
    return my_count++;
}

inline std::size_t counter::value()
{
    return my_count;
}

inline void counter::rewind()
{
    my_count--;
}

inline void counter::reset(std::size_t count)
{
    my_count = count;
}

END_NAMESPACE

#endif // __COUNTER_H__
