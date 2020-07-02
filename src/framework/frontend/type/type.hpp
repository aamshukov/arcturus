//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TYPE_H__
#define __TYPE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class type
{
    public:
        using index_type = int32_t;
        using size_type = std::size_t;

        enum class flagss : uint64_t
        {
            clear = 0x00
        };

        DECLARE_ENUM_OPERATORS(flagss)

        using flags_type = flagss;

    protected:
        string_type     my_name;

        size_type       my_size;            // size in bits, abstract width, like C type hierarchy
        size_type       my_platform_size;   // size in bits, platform specific width

        flags_type      my_flags;

        size_type       my_cardinality;     // scalar (0), vector/1D-array(1), matrix/2D-array(2), etc.

    public:
                        type();

                        type(const type& other) = default;
                        type(type&& other) = default;

        virtual        ~type() = 0;

        type&           operator = (const type& other) = default;
        type&           operator = (type&& other) = default;

        const string_type&  name() const;
        string_type&        name();

        size_type       size() const;
        size_type&      size();

        size_type       platform_size() const;
        size_type&      platform_size();

        flags_type      flags() const;
        flags_type&     flags();

        size_type       cardinality() const;
        size_type&      cardinality();
};

template <typename Traits>
class abstract_type : public type
{
    public:
        using traits_type = Traits;
        using kind_type = typename traits_type::kind;

    protected:
        kind_type       my_kind; // kind of type

    public:
                        abstract_type(kind_type kind = abstract_type<traits_type>::kind_type::unknown_type);

                        abstract_type(const abstract_type& other) = default;
                        abstract_type(abstract_type&& other) = default;

        virtual        ~abstract_type() = 0;

        abstract_type&  operator = (const abstract_type& other) = default;
        abstract_type&  operator = (abstract_type&& other) = default;

        kind_type       kind() const;
        kind_type&      kind();
};

END_NAMESPACE

#endif // __TYPE_H__
