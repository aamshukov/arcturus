//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __LIST_H__
#define __LIST_H__

#pragma once

BEGIN_NAMESPACE(core)

class list : private noncopyable
{
    public:
        using list_type = std::shared_ptr<list>;

        enum class flag : uint64_t
        {
            clear = 0x0000
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

    protected:
        list_type           my_next; // the next node in the double list
        list_type           my_prev; // the prev node in the double list

        flags_type          my_flags;

    public:
                            list();
        virtual            ~list();

        const list_type&    next() const;
        list_type&          next();

        const list_type&    prev() const;
        list_type&          prev();

        const flags_type&   flags() const;
        flags_type&         flags();

        static void         link(list_type& node, list_type& new_node); // append
        static void         unlink(list_type& node);
};

inline const typename list::list_type& list::next() const
{
    return my_next;
}

inline typename list::list_type& list::next()
{
    return my_next;
}

inline const typename list::list_type& list::prev() const
{
    return my_prev;
}

inline typename list::list_type& list::prev()
{
    return my_prev;
}

inline const typename list::flags_type& list::flags() const
{
    return my_flags;
}

inline typename list::flags_type& list::flags()
{
    return my_flags;
}

END_NAMESPACE

#endif // __LIST_H__
