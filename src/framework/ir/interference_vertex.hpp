//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_INTERFERENCE_VERTEX_H__
#define __GRAPH_INTERFERENCE_VERTEX_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)

class interference_vertex : public vertex<std::size_t>
{
    public:
        using symbol_type = std::shared_ptr<symtable::symbol>;

    protected:
        symbol_type         my_symbol;

    public:
                            interference_vertex(const symbol_type& symbol);
        virtual            ~interference_vertex();

        const symbol_type&  symbol() const;
        symbol_type&        symbol();

        ACCEPT_METHOD;
};

inline interference_vertex::interference_vertex(const typename interference_vertex::symbol_type& symbol)
                          : my_symbol(symbol)
{
}

inline interference_vertex::~interference_vertex()
{
}

inline const typename interference_vertex::symbol_type& interference_vertex::symbol() const
{
    return my_symbol;
}

inline typename interference_vertex::symbol_type& interference_vertex::symbol()
{
    return my_symbol;
}

END_NAMESPACE

#endif // __GRAPH_INTERFERENCE_VERTEX_H__
