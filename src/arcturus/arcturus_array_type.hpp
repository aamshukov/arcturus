//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __ARCTURUS_ARRAY_TYPE_H__
#define __ARCTURUS_ARRAY_TYPE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class arcturus_array_type : public arcturus_type
{
    public:
        using base_type = std::shared_ptr<arcturus_type>;

    private:
        base_type       my_base;

        index_type      my_array_lower_bound;   //  [-10..10]
        index_type      my_array_upper_bound;   //

        bool            my_checked_array;       //?? checked or unchecked array, default is checked
        bool            my_rowbased_array;      //?? row or column based array, default is row based

    public:
                        arcturus_array_type(const base_type& base_type);
                       ~arcturus_array_type();

        friend bool     operator == (const arcturus_array_type& lhs, const arcturus_array_type& rhs);
        friend bool     operator != (const arcturus_array_type& lhs, const arcturus_array_type& rhs);

        base_type       base() const;

        index_type      array_lower_bound() const;
        index_type&     array_lower_bound();

        index_type      array_upper_bound() const;
        index_type&     array_upper_bound();

        bool            checked_array() const;
        bool&           checked_array();

        bool            rowbased_array() const;
        bool&           rowbased_array();
};

inline typename arcturus_array_type::base_type arcturus_array_type::base() const
{
    return my_base;
}

inline typename arcturus_array_type::index_type arcturus_array_type::array_lower_bound() const
{
    return my_array_lower_bound;
}

inline typename arcturus_array_type::index_type& arcturus_array_type::array_lower_bound()
{
    return my_array_lower_bound;
}

inline typename arcturus_array_type::index_type arcturus_array_type::array_upper_bound() const
{
    return my_array_upper_bound;
}

inline typename arcturus_array_type::index_type& arcturus_array_type::array_upper_bound()
{
    return my_array_upper_bound;
}

inline bool arcturus_array_type::checked_array() const
{
    return my_checked_array;
}

inline bool& arcturus_array_type::checked_array()
{
    return my_checked_array;
}

inline bool arcturus_array_type::rowbased_array() const
{
    return my_rowbased_array;
}

inline bool& arcturus_array_type::rowbased_array()
{
    return my_rowbased_array;
}

END_NAMESPACE

#endif // __ARCTURUS_ARRAY_TYPE_H__
