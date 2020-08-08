//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAPH_VERTEX_H__
#define __GRAPH_VERTEX_H__

#pragma once

BEGIN_NAMESPACE(core)

struct vertex : public visitable
{
    using vertices_type = std::list<std::weak_ptr<vertex>>;
    using index_type = int64_t;

    index_type id = 0;
    vertices_type adjacencies;

    ACCEPT_METHOD;
};

END_NAMESPACE

#endif // __GRAPH_VERTEX_H__
