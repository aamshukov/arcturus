//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
// based on https://algs4.cs.princeton.edu/15uf/UF.java.html
// by Robert Sedgewick and Kevin Wayne
#ifndef __DISJOINT_SETS_H__
#define __DISJOINT_SETS_H__

#pragma once

BEGIN_NAMESPACE(core)

template <typename TElements, typename TElement>
class disjoint_sets : private noncopyable
{
    public:
        using element_type = TElement;
        using elements_type = TElements;

        using parents_type = std::vector<std::size_t>;
        using ranks_type = std::vector<std::size_t>;

        using element_index_map_type = std::unordered_map<element_type, std::size_t>;

    private:
        parents_type            my_parents;
        ranks_type              my_ranks;

        element_index_map_type  my_element_index_map;

        std::size_t             my_count;

    public:
        explicit                disjoint_sets(const elements_type& elements);
                               ~disjoint_sets();

        std::size_t             count() const;

        std::size_t             find(const element_type& element);
        void                    union_sets(const element_type& element1, const element_type& element2);
};

template <typename TElements, typename TElement>
inline disjoint_sets<TElements, TElement>::disjoint_sets(const typename disjoint_sets<TElements, TElement>::elements_type& elements)
{
    my_count = elements.size();

    my_parents.resize(my_count);
    my_ranks.resize(my_count);

    auto k = 0;

    for(const auto& element : elements)
    {
        my_parents[k] = k;
        my_ranks[k] = 0;

        my_element_index_map[element] = k++;
    }
}

template <typename TElements, typename TElement>
inline disjoint_sets<TElements, TElement>::~disjoint_sets()
{
}

template <typename TElements, typename TElement>
inline std::size_t disjoint_sets<TElements, TElement>::count() const
{
    return my_count;
}

template <typename TElements, typename TElement>
std::size_t disjoint_sets<TElements, TElement>::find(const typename disjoint_sets<TElements, TElement>::element_type& element)
{
    auto r = my_element_index_map[element]; // get index

    // locate root
    while(r != my_parents[r])
    {
        my_parents[r] = my_parents[my_parents[r]]; // path compression by halving,
        r = my_parents[r];                         // full path compression is more involving (another loop from the original element and up to the root)
    }

    return r;
}

template <typename TElements, typename TElement>
void disjoint_sets<TElements, TElement>::union_sets(const typename disjoint_sets<TElements, TElement>::element_type& element1,
                                                    const typename disjoint_sets<TElements, TElement>::element_type& element2)
{
    auto r1(find(element1));
    auto r2(find(element2));

    if(r1 != r2)
    {
        if(my_ranks[r1] < my_ranks[r2])
        {
            my_parents[r1] = r2;
        }
        else if(my_ranks[r1] > my_ranks[r2])
        {
            my_parents[r2] = r1;
        }
        else
        {
            my_parents[r2] = r1;
            my_ranks[r1]++;
        }

        my_count--;
    }
}

END_NAMESPACE

#endif // __DISJOINT_SETS_H__
