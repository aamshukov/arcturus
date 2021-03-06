//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_PARSE_TREE_H__
#define __ARCTURUS_PARSE_TREE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)

struct arcturus_tree_traits
{
    DECLARE_ENUM
    (
        kind,
        std::size_t,
        unknown = 0,

        identifier = 100,

        function = 500, //?? might hold CFG with ready to generate code

        // the following one (1) entry MUST be the last entry in the enum
        size
    )
};

class arcturus_parse_tree : public parse_tree
{
    public:
        using token_type = typename parse_tree::token_type;

        using traits_type = arcturus_tree_traits;
        using tree_kind = traits_type::kind;

    protected:
        tree_kind           my_kind;

    public:
                            arcturus_parse_tree(const tree_kind& kind);
        virtual            ~arcturus_parse_tree();

        const tree_kind&    kind() const;
        tree_kind&          kind();

        string_type         name() const;
        static string_type  name(const tree_kind& kind);

        ACCEPT_METHOD;
};

inline const typename arcturus_parse_tree::tree_kind& arcturus_parse_tree::kind() const
{
    return my_kind;
}

inline typename arcturus_parse_tree::tree_kind& arcturus_parse_tree::kind()
{
    return my_kind;
}

inline string_type arcturus_parse_tree::name() const
{
    return arcturus_parse_tree::name(my_kind);
}

inline string_type arcturus_parse_tree::name(const typename arcturus_parse_tree::tree_kind& kind)
{
    if(traits_type::mapping.empty())
    {
        traits_type::initialize();
    }

    return traits_type::name(kind);
}

END_NAMESPACE

#endif // __ARCTURUS_PARSE_TREE_H__
