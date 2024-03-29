﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __SCOPE_H__
#define __SCOPE_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

class scope : public tree
{
    public:
        using symbol_type = std::shared_ptr<symbol>;

        using key_type = cps_type;
        using value_type = symbol_type;

        using symbols_type = std::unordered_map<key_type, value_type>;
        using types_type = int; //??

        using scope_type = std::shared_ptr<scope>;

    public:
        //enum class kind //?? inheritance instead
        //{
        //    scope_namespace = 1,
        //    scope_structure = 2,
        //    scope_function  = 3,
        //    scope_parameter = 4
        //    scope_block = 5,
        //};

    private:
        //??scope::kind     my_kind;
        scope_type      my_papa;    // parent scope

        std::size_t     my_level;   // depth
        string_type     my_name;    // x:level

        symbols_type    my_symbols; // content
        types_type      my_types;   // introduced types

    public:
                       // scope();
                       //~scope();

        const symbols_type& symbols() const;
        symbols_type&       symbols();
};

inline const typename scope::symbols_type& scope::symbols() const
{
    return my_symbols;
}

inline typename scope::symbols_type& scope::symbols()
{
    return my_symbols;
}

END_NAMESPACE

#endif // __SCOPE_H__
