//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __VIRTUAL_FILE_SYSTEM_BTREE_H__
#define __VIRTUAL_FILE_SYSTEM_BTREE_H__

#pragma once

BEGIN_NAMESPACE(backend)
USING_NAMESPACE(core)

template <typename TKey, typename TValue, typename TKeyCmp = std::less<TKey>, typename TKeyHash = std::hash<TKey>>
class btree
{
    // https://en.wikipedia.org/wiki/B%2B_tree
    //  The order, or branching factor, b of a B+ tree measures the capacity of nodes (i.e., the number of children nodes)
    //  for internal nodes in the tree. The actual number of children for a node, referred to here as m, is constrained for
    //  internal nodes so that ⌈b/2⌉ <= m <= b, for order = 7: inner node 4 - 7, leaf 1 - 6,
    //  root 2 - 7 or 1 - 6 if root is the only one node in tree (leaf)
    public:
        using key_type = TKey;
        using key_cmp = TKeyCmp;
        using keys_type = std::vector<key_type>;

        using value_type = TValue;

        using kvp_type = std::pair<key_type, value_type>;
        using kvps_type = std::vector<kvp_type>;

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

    public:

        // in-memory node
        struct node
        {
            std::size_t level; // level == 0 is leaf
            std::size_t count; // number of slots in use

            node(std::size_t l) : level(l), count(0)
            {
            }

            bool leaf_node() const
            {
                return level == 0;
            }
        };

        // in-memory index node (branch, internal)
        struct index_node : public node, public tree
        {
            using index_node_type = std::shared_ptr<index_node>;

            keys_type keys; // index node payload - keys

            index_node(std::size_t l) : node(l)
            {
                keys.resize(my_inner_node_count_max - 1);
                kids.resize(my_inner_node_count_max);
            }

            const key_type& key(std::size_t k) const
            {
                return keys[k];
            }
        };

        friend struct index_node;

        // in-memory leaf node
        struct leaf_node : public node
        {
            using leaf_node_type = std::shared_ptr<leaf_node>;

            leaf_node_type  next_node;  // next sibling (at the same level)
            leaf_node_type  prev_node;  // prev sibling (at the same level)

            kvps_type       kvps;       // leaf node payload - key/value pairs

            leaf_node() : next_node(nullptr), prev_node(nullptr)
            {
                kvps.resize(my_leaf_node_count_max);
            }

            const key_type& key(std::size_t k) const
            {
                return kvps[k].first;
            }

            const value_type& value(std::size_t k) const
            {
                return kvps[k].second;
            }
        };

        friend struct leaf_node;

    private:
        std::size_t my_leaf_node_count_max;
        std::size_t my_leaf_node_count_min;
        std::size_t my_inner_node_count_max;
        std::size_t my_inner_node_count_min;

    public:
                    btree(std::size_t order) // branching factor
                        : my_leaf_node_count_min((order + 2 - 1) / 2), // ⌈b/2⌉
                          my_leaf_node_count_max(order),
                          my_inner_node_count_min((order + 2 - 1) / 2), // ⌈b/2⌉
                          my_inner_node_count_max(order)
                    {
                    }

                   ~btree();

};

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_BTREE_H__
