//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TYPE_H__
#define __TYPE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class type : public visitable
{
    public:
        using index_type = int32_t;

        enum class flag : uint64_t
        {
            clear = 0x00
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

    protected:
        string_type         my_name;

        std::size_t         my_size;            // size in bits, abstract width, like C type hierarchy
        std::size_t         my_platform_size;   // size in bits, platform specific width

        std::size_t         my_alignment;       // alignment in memory

        flags_type          my_flags;

        std::size_t         my_cardinality;     // scalar (0), vector/1D-array(1), matrix/2D-array(2), etc.

    public:
                            type();

                            type(const type& other) = default;
                            type(type&& other) = default;

        virtual            ~type() = 0;

        type&               operator = (const type& other) = default;
        type&               operator = (type&& other) = default;

        const string_type&  name() const;
        string_type&        name();

        std::size_t         size() const;
        std::size_t&        size();

        std::size_t         platform_size() const;
        std::size_t&        platform_size();

        std::size_t         alignment() const;
        std::size_t&        alignment();

        flags_type          flags() const;
        flags_type&         flags();

        std::size_t         cardinality() const;
        std::size_t&        cardinality();

        virtual bool        equivalent(const type& other) const = 0;

        ACCEPT_METHOD;
};

inline type::type()
           : my_size(0),
             my_platform_size(0),
             my_alignment(0),
             my_flags(type::flags_type::clear),
             my_cardinality(0)
{
}

inline type::~type()
{
}

inline const string_type& type::name() const
{
    return my_name;
}

inline string_type& type::name()
{
    return my_name;
}

inline std::size_t type::size() const
{
    return my_size;
}

inline std::size_t& type::size()
{
    return my_size;
}

inline std::size_t type::platform_size() const
{
    return my_platform_size;
}

inline std::size_t& type::platform_size()
{
    return my_platform_size;
}

inline std::size_t type::alignment() const
{
    return my_alignment;
}

inline std::size_t& type::alignment()
{
    return my_alignment;
}

inline typename type::flags_type type::flags() const
{
    return my_flags;
}

inline typename type::flags_type& type::flags()
{
    return my_flags;
}


inline std::size_t type::cardinality() const
{
    return my_cardinality;
}

inline std::size_t& type::cardinality()
{
    return my_cardinality;
}

END_NAMESPACE

#endif // __TYPE_H__
