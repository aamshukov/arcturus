//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TREE_H__
#define __TREE_H__

#pragma once

BEGIN_NAMESPACE(core)

class tree : private noncopyable
{
    public:
        using tree_type = std::shared_ptr<tree>;
        using trees_type = std::vector<tree_type>;

        enum class flag : uint64_t
        {
            clear       = 0x0000,
            deleted     = 0x0001,
            root_in_ast = 0x0002,   // aka ANTLR, expr : mexpr (’+’^ mexpr)* EOF! ;  \  mutually exclusive
            not_in_ast  = 0x0004,   // aka ANTLR, expr : mexpr (’+’^ mexpr)* EOF! ;  /
            visited     = 0x0008
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

        using id_type = std::size_t;

    protected:
        id_type             my_id; // key

        tree_type           my_papa; // parent
        trees_type          my_kids;

        flags_type          my_flags;

    public:
                            tree();
        virtual            ~tree() = 0;

        id_type             id() const;
        id_type&            id();

        const tree_type&    papa() const;
        tree_type&          papa();

        const trees_type&   kids() const;
        trees_type&         kids();

        const flags_type&   flags() const;
        flags_type&         flags();
};

inline typename tree::id_type tree::id() const
{
    return my_id;
}

inline typename tree::id_type& tree::id()
{
    return my_id;
}

inline const typename tree::tree_type& tree::papa() const
{
    return my_papa;
}

inline typename tree::tree_type& tree::papa()
{
    return my_papa;
}

inline const typename tree::trees_type& tree::kids() const
{
    return my_kids;
}

inline typename tree::trees_type& tree::kids()
{
    return my_kids;
}

inline const typename tree::flags_type& tree::flags() const
{
    return my_flags;
}

inline typename tree::flags_type& tree::flags()
{
    return my_flags;
}

END_NAMESPACE

#endif // __TREE_H__
