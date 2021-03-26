//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
// LRU cache
// great explanation https://timday.bitbucket.io/lru.html
#ifndef __CACHE_H__
#define __CACHE_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TKey,
          typename TValue,
          typename TKeyEqComparator = std::equal_to<TKey>,
          typename TKeyHashFunction = std::hash<TKey>>
class cache : private noncopyable
{
    public:
        using key_type = TKey;
        using key_eq_comparator = TKeyEqComparator;
        using key_hash_function = TKeyHashFunction;
        using keys_type = std::list<key_type>;          // key access history, most recent at back
        using keys_it = typename keys_type::iterator;   // key access history iterator, part of cache entry
        using value_type = TValue;
        using cache_entry = std::pair<value_type, keys_it>;
        using map_type = std::unordered_map<key_type, cache_entry, key_hash_function, key_eq_comparator>;

    private:
        std::size_t my_size; // max cache size
        map_type    my_map;  // cache itself
        keys_type   my_keys; // history semantic

    public:
                    cache(std::size_t size);

        std::size_t size() const;

        std::optional<value_type>
                    get(const key_type& key);
        bool        put(const key_type& key, const value_type& value);
};

template <typename TKey, typename TValue, typename TKeyEqComparator, typename TKeyHashFunction>
cache<TKey, TValue, TKeyEqComparator, TKeyHashFunction>::cache(std::size_t size)
                                                       : my_size(size)
{
    my_map.reserve(size);
}

template <typename TKey, typename TValue, typename TKeyEqComparator, typename TKeyHashFunction>
inline std::size_t cache<TKey, TValue, TKeyEqComparator, TKeyHashFunction>::size() const
{
    return my_size;
}

template <typename TKey, typename TValue, typename TKeyEqComparator, typename TKeyHashFunction>
std::optional<typename cache<TKey, TValue, TKeyEqComparator, TKeyHashFunction>::value_type>
cache<TKey, TValue, TKeyEqComparator, TKeyHashFunction>::get(const typename cache<TKey, TValue, TKeyEqComparator, TKeyHashFunction>::key_type& key)
{
    std::optional<value_type> result;

    const auto it(my_map.find(key));

    if(it != my_map.end())
    {
        my_keys.splice(my_keys.end(), my_keys, (*it).second.second); // update access record by moving accessed key to back of list
        result = (*it).second.first; // value
    }

    return result;
}

template <typename TKey, typename TValue, typename TKeyEqComparator, typename TKeyHashFunction>
bool cache<TKey, TValue, TKeyEqComparator, TKeyHashFunction>::put(const typename cache<TKey, TValue, TKeyEqComparator, TKeyHashFunction>::key_type& key,
                                                                  const typename cache<TKey, TValue, TKeyEqComparator, TKeyHashFunction>::value_type& value)
{
    bool result = false;

    if(my_map.find(key) == my_map.end()) // does not exists - proceed
    {
        if(my_map.size() == my_size)
        {
            // evict and make room
            if(!my_keys.empty())
            {
                const auto it(my_map.find(my_keys.front()));

                my_map.erase(it); // it must exist
                my_keys.pop_front();
            }
        }

        keys_it it(my_keys.insert(my_keys.end(), key)); // key is the most recently used key - push back

        my_map.insert(std::make_pair(key, std::make_pair(value, it))); // make new cache entry

        result = true;
    }

    return result;
}

END_NAMESPACE

#endif // __CACHE_H__
