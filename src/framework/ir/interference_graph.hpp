//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __INTERFERENCE_GRAPH_H__
#define __INTERFERENCE_GRAPH_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)

class interference_graph : public graph<interference_vertex>
{
    public:
                    interference_graph();
        virtual    ~interference_graph();
};

inline interference_graph::interference_graph()
                         : graph<interference_vertex>(false)
{
}

inline interference_graph::~interference_graph()
{
}

END_NAMESPACE

#endif // __INTERFERENCE_GRAPH_H__
