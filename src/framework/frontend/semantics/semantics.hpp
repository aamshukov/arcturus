//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token>
class semantics : private noncopyable
{
    public:
        using token_type = typename parser<Token>::token_type;

        using parse_tree_type = std::shared_ptr<parse_tree<token_type>>;
        using parse_trees_type = std::vector<parse_tree_type>;

    public:
                semantics();
               ~semantics();

        void    populate_symbol_table(const parse_tree_type& tree);

};

template <typename Token>
semantics<Token>::semantics()
{
}

template <typename Token>
semantics<Token>::~semantics()
{
}

template <typename Token>
void semantics<Token>::populate_symbol_table(const typename semantics<Token>::parse_tree_type& tree)
{
}

END_NAMESPACE

#endif // __SEMANTICS_H__
