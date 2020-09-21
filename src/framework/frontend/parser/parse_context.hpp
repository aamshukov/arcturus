//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __PARSE_CONTEXT_H__
#define __PARSE_CONTEXT_H__

#pragma once

BEGIN_NAMESPACE(frontend)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)

template <typename Token>
class parse_context : public context
{
    public:
        using token_type = Token;
        using tree_type = parse_tree_base<token_type>;

    private:
        // parse tree

    public:
                    parse_context();
        virtual    ~parse_context();
};

template <typename Token>
parse_context<Token>::parse_context()
{
}

template <typename Token>
parse_context<Token>::~parse_context()
{
}

END_NAMESPACE

#endif // __PARSE_CONTEXT_H__
