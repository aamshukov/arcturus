//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TYPE_H__
#define __TYPE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

class type : private noncopyable
{
    public:
        using index_type = int32_t;
        using size_type = std::size_t;

        enum class flags : uint64_t
        {
            clear = 0x00
        };

        using flags_type = flags;

    private:
        size_type       my_size;        // size in bits, abstract width, like C type hierarchy
        flags_type      my_flags;

        size_type       my_cardinality; // scalar (0), vector/1D-array(1), matrix/2D-array(2), etc.

    public:
                        type();
        virtual        ~type() = 0;

        size_type       size() const;
        size_type&      size();

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
        virtual        ~abstract_type() = 0;

        kind_type       kind() const;
        kind_type&      kind();
};

END_NAMESPACE

#endif // __TYPE_H__
