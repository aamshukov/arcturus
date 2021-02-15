//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __TYPE_H__
#define __TYPE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class type
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
