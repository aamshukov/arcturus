//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SYMBOL_TABLE_BUILDER_H__
#define __SYMBOL_TABLE_BUILDER_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token, typename Tree>
class symbol_table_builder : public visitor<bool, char, symbol<Token>>
{
    public:
        using token_type = Token;
        using tree_type = Tree;

        using parse_tree_type = std::shared_ptr<tree_type>;

    public:
       //         symbol_table_builder();
       //        ~symbol_table_builder();

       //void     build(const parse_tree_type& tree);
};

END_NAMESPACE

#endif // __SYMBOL_TABLE_BUILDER_H__
