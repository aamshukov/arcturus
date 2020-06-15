//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token>
class symbol_table : public singleton<symbol_table<Token>>
{
    // hashtable and stack, LeBlanc/Cook

    //public:
    //    using symbol_attribute_type = std::shared_ptr<symbol_attribute<T>>;
    //    using symbol_attributes_type = std::map<std::size_t, symbol_attribute_type>;
    //    using stack_type = std::stack<symbol_attribute_type>;

    //private:
    //    symbol_attributes_type  table;
    //    stack_type              stack; // display

    public:
        using token_type = Token;

        using size_type = std::size_t;
        using scope_type = std::shared_ptr<scope<token_type>>;

    private:
        scope_type              my_root;    // root of scope tree, might represent 'global' scope

        size_type               my_level;   // current scope level
        scope_type              my_scope;   // current scope

    public:
                                symbol_table();
                               ~symbol_table();
    //    // enter scope ++my_level
    //    // leave scope --my_level

    //    // add symbol
    //    // get/lookup symbol
};

END_NAMESPACE

#endif // __SYMBOL_TABLE_H__
