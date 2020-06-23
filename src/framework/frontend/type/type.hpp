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
        using size_type = std::size_t;

        enum class flags : uint64_t
        {
            clear = 0x00
        };

        using flags_type = flags;

    private:
        size_type       my_size;    // size in bits, width for runtime allocation
        flags_type      my_flags;

    public:
                        type();
        virtual        ~type() = 0;

        virtual bool    operator == (const type& other) = 0;
        virtual bool    operator != (const type& other) = 0;

        size_type       size() const;
        size_type&      size();

        flags_type      flags() const;
        flags_type&     flags();
};

template <typename Traits>
class abstract_type : public type
{
    public:
        using traits_type = Traits;
        using kind_type = typename traits_type::kind;

    private:
        kind_type       my_kind; // kind of type

    public:
                        abstract_type();
        virtual        ~abstract_type() = 0;

        kind_type       kind() const;
        kind_type&      kind();
};

END_NAMESPACE

#endif // __TYPE_H__
