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
    public:
        using key_type = TKey;
        using value_type = TValue;

    public:
        struct node
        {
            std::size_t keys_number;
        };

        struct index : public node
        {
        };

        struct leaf : public node
        {
        };

    public:
                btree();
               ~btree();

};

END_NAMESPACE

#endif // __VIRTUAL_FILE_SYSTEM_BTREE_H__
