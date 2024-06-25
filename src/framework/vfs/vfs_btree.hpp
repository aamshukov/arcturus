//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_BTREE_H__
#define __VIRTUAL_FILE_SYSTEM_BTREE_H__

#pragma once

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

template <typename TKey, typename TValue, typename TKeyCmp = std::less<TKey>, typename TKeyHash = std::hash<TKey>>
class btree : public noncopyable
{
    // https://en.wikipedia.org/wiki/B%2B_tree
    //  The order, or branching factor, b of a B+ tree measures the capacity of nodes (i.e., the number of children nodes)
    //  for internal nodes in the tree. The actual number of children for a node, referred to here as m, is constrained for
    //  internal nodes so that ⌈b/2⌉ <= m <= b, for order = 7: inner node 4 - 7, leaf 1 - 6,
    //  root 2 - 7 or 1 - 6 if root is the only one node in the tree (leaf)
    public:
        using type_traits = vfs_type_traits;

        using key_type = TKey;
        using key_cmp = TKeyCmp;
        using keys_type = std::vector<key_type>;

        using value_type = TValue;

        using kvp_type = std::pair<key_type, value_type>;
        using kvps_type = std::vector<kvp_type>;

        using size_type = typename type_traits::size_type;

        using paddr_type = size_type; // persistent address, LE
        using paddrs_type = std::vector<paddr_type>;

        using free_paddrs_type = std::queue<paddr_type>;

        using flags_type = typename type_traits::flags_type;

        using paging_type = std::shared_ptr<vfs_io_manager>;

    private:
        static const size_type INVALID_PAGE = std::numeric_limits<size_type>::min();

    public:
        // in-memory node
        struct node //: public tree
        {
            size_type level;  // level == 0 is leaf

            paddr_type  paddr;  // persistent address
            paddrs_type paddrs; // persistent addresses of kids

            node(size_type levl, size_type capacity) : level(levl), paddr(INVALID_PAGE)
            {
                //kids.reserve(capacity);
                paddrs.reserve(capacity);
            }

            bool leaf_node() const
            {
                return level == 0;
            }
        };

        using node_type = std::shared_ptr<node>;
        using nodes_type = std::vector<node_type>;

        static const int BINARY_SEARCH_THRESHOLD = 16; // either linear or binary search in leaf nodes

        // in-memory index node (branch, internal)
        struct index_node : public node
        {
            keys_type keys; // index node payload - keys

            index_node(size_t level, size_t capacity) : node(level, capacity)
            {
                //?? static_assert(capacity > 0, L"Invalid B+ tree order (branching factor).");
                assert(capacity > 0);
                keys.reserve(capacity - 1);
            }

            const key_type& key(size_type k) const
            {
                return keys[k];
            }

            bool full(size_type capacity) const
            {
                return keys.size() == capacity;
            }

            bool notfull(size_type capacity) const // less than or equal to my_index_node_min_count
            {
                return keys.size() <= capacity;
            }

            bool underflow(size_type capacity) const // less than my_index_node_min_count
            {
                return keys.size() < capacity;
            }
        };

        using index_node_type = std::shared_ptr<index_node>;

        // in-memory leaf node
        struct leaf_node : public node, public list
        {
            leaf_node* next_node; // next sibling (at the same level)
            leaf_node* prev_node; // prev sibling (at the same level)

            kvps_type kvps; // leaf node payload - key/value pairs

            leaf_node(size_t level, size_t capacity)
                : node(level, capacity), next_node(nullptr), prev_node(nullptr)
            {
                //?? static_assert(capacity > 0, L"Invalid B+ tree order (branching factor).");
                assert(capacity > 0);
                kvps.reserve(capacity);
            }

            const key_type& key(size_type k) const
            {
                return kvps[k].first;
            }

            const value_type& value(size_type k) const
            {
                return kvps[k].second;
            }

            bool full(size_type capacity) const
            {
                return kvps.size() == capacity;
            }

            bool notfull(size_type capacity) const // less than or equal to my_leaf_node_min_count
            {
                return kvps.size() <= capacity;
            }

            bool underflow(size_type capacity) const // less than my_leaf_node_min_count
            {
                return kvps.size() < capacity;
            }
        };

        using leaf_node_type = std::shared_ptr<leaf_node>;

    public:
        struct statistics
        {
        };

        using statistics_type = statistics;

    private:
        size_type               my_leaf_node_max_count;
        size_type               my_leaf_node_min_count;
        size_type               my_index_node_max_count;
        size_type               my_index_node_min_count;

        node_type               my_root;

        leaf_node_type          my_head_leaf;
        leaf_node_type          my_tail_leaf;

        size_type               my_height;  // current height of the tree
        nodes_type              my_nodes;   // cached in-memory nodes, usually populated while reading in a branch

        paging_type             my_paging;

        statistics_type         my_statistics;

    private:
        leaf_node_type          locate_leaf_node(const key_type& key); // search entire tree
        leaf_node_type          search_leaf_node(const key_type& key, value_type& value); // search in leaf node

    public:
                                btree(size_type order,      // 'order' is branching factor, maximum number of descendants
                                      paging_type& paging); // paging infrustructure with caching
                               ~btree();

        const statistics_type&  statistics() const;
};

template <typename TKey, typename TValue, typename TKeyCmp, typename TKeyHash>
btree<TKey, TValue, TKeyCmp, TKeyHash>::btree(size_type order,
                                              typename btree<TKey, TValue, TKeyCmp, TKeyHash>::paging_type& paging)
                                      : my_leaf_node_min_count((order + 2 - 1) / 2), // ⌈b/2⌉
                                        my_leaf_node_max_count(order),
                                        my_index_node_min_count((order + 2 - 1) / 2), // ⌈b/2⌉
                                        my_index_node_max_count(order),
                                        my_height(0),
                                        my_paging(paging)
{
}

template <typename TKey, typename TValue, typename TKeyCmp, typename TKeyHash>
btree<TKey, TValue, TKeyCmp, TKeyHash>::~btree()
{
}

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_BTREE_H__



//?? fopen(filename, "w+b");
