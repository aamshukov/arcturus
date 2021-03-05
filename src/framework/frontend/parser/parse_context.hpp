//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __PARSE_CONTEXT_H__
#define __PARSE_CONTEXT_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

class parse_context : public context
{
    public:
        using token_type = token<token_traits>;
        using tree_type = parse_tree_base;

    private:
        // parse tree

    public:
                    parse_context();
        virtual    ~parse_context();
};

inline parse_context::parse_context()
{
}

inline parse_context::~parse_context()
{
}

END_NAMESPACE

#endif // __PARSE_CONTEXT_H__
