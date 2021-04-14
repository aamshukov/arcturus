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
        using key_type = TKey;
        using key_cmp = TKeyCmp;
        using keys_type = std::vector<key_type>;

        using value_type = TValue;

        using kvp_type = std::pair<key_type, value_type>;
        using kvps_type = std::vector<kvp_type>;

        using paddr_type = std::size_t; // physical address
        using paddrs_type = std::vector<paddr_type>;

        using free_paddrs_type = std::queue<paddr_type>;

    public:
        enum class flag : uint64_t
        {
            clear = 0x0000,
            index = 0x0002,
            leaf  = 0x0004,
            error = 0x0008
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

    private:
        using paging_type = std::shared_ptr<vfs_paging>;

    private:
        static const std::size_t INVALID_PAGE = std::numeric_limits<std::size_t>::min();

    public:
        // in-memory node
        struct node : public tree
        {
            using node_type = std::shared_ptr<node>;

            std::size_t level;  // level == 0 is leaf

            paddr_type  paddr;  // persistent address, might be a start page number of the node
            paddrs_type paddrs; // persistent addresses of kids, size should be in sync with tree::kids

            node(std::size_t levl, std::size_t capacity) : level(levl), paddr(INVALID_PAGE)
            {
                kids.reserve(capacity);
                paddrs.reserve(capacity);
            }

            bool leaf_node() const
            {
                return level == 0;
            }
        };

        using nodes_type = std::vector<typename node::node_type>;

        static const int BINARY_SEARCH_THRESHOLD = 16; // either linear or binary search in leaf nodes

        // in-memory index node (branch, internal)
        struct index_node : public node
        {
            using node_type = std::shared_ptr<index_node>;

            keys_type keys; // index node payload - keys

            index_node(std::size_t level, std::size_t capacity) : node(level, capacity)
            {
                static_assert(capacity > 0, L"Invalid B+ tree order (branching factor).");
                keys.reserve(capacity - 1);
            }

            const key_type& key(std::size_t k) const
            {
                return keys[k];
            }

            bool full(std::size_t capacity) const
            {
                return keys.size() == capacity;
            }

            bool notfull(std::size_t capacity) const // less than or equal to my_index_node_min_count
            {
                return keys.size() <= capacity;
            }

            bool underflow(std::size_t capacity) const // less than my_index_node_min_count
            {
                return keys.size() < capacity;
            }
        };

        // in-memory leaf node
        struct leaf_node : public node, public list
        {
            using node_type = std::shared_ptr<leaf_node>;

            node_type next_node; // next sibling (at the same level)
            node_type prev_node; // prev sibling (at the same level)

            kvps_type kvps; // leaf node payload - key/value pairs

            leaf_node(size_t level, std::size_t capacity)
                : node(level, capacity), next_node(nullptr), prev_node(nullptr)
            {
                static_assert(capacity > 0, L"Invalid B+ tree order (branching factor).");
                kvps.reserve(capacity);
            }

            const key_type& key(std::size_t k) const
            {
                return kvps[k].first;
            }

            const value_type& value(std::size_t k) const
            {
                return kvps[k].second;
            }

            bool full(std::size_t capacity) const
            {
                return kvps.size() == capacity;
            }

            bool notfull(std::size_t capacity) const // less than or equal to my_leaf_node_min_count
            {
                return kvps.size() <= capacity;
            }

            bool underflow(std::size_t capacity) const // less than my_leaf_node_min_count
            {
                return kvps.size() < capacity;
            }
        };

    public:
        struct statistics
        {
        };

        using statistics_type = statistics;

    private:
        std::size_t             my_leaf_node_max_count;
        std::size_t             my_leaf_node_min_count;
        std::size_t             my_index_node_max_count;
        std::size_t             my_index_node_min_count;

        typename node::node_type
                                my_root;

        typename leaf_node::node_type
                                my_head_leaf;
        typename leaf_node::node_type
                                my_tail_leaf;

        std::size_t             my_height;  // current height of the tree
        nodes_type              my_nodes;   // cached in-memory nodes, usually populated while reading in a branch

        paging_type             my_paging;

        statistics_type         my_statistics;

    private:
        typename leaf_node::node_type
                                LocateLeafNode(const key_type& key); // search entire tree

        typename leaf_node::node_type
                                SearchLeafNode(const key_type& key, value_type& value); // search a leaf node

    public:
                                btree(std::size_t order,    // 'order' is branching factor, maximum number of descendants
                                      paging_type& paging); // paging infrustructure with caching
                               ~btree();

        const statistics_type&  statistics() const;

};

template <typename TKey, typename TValue, typename TKeyCmp, typename TKeyHash>
btree<TKey, TValue, TKeyCmp, TKeyHash>::btree(std::size_t order,
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

