//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_INL__
#define __IR_INL__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token, typename TreeTraits>
void ir<Token, TreeTraits>::cst_to_ast(typename ir<Token, TreeTraits>::parse_tree_type& cst)
{
    // will be split later ...
    std::stack<parse_tree_type> stack;

    std::queue<parse_tree_type> queue;
    std::queue<parse_tree_type> empty_queue;

    // apply ! operator
    queue.emplace(cst);

    while(!queue.empty())
    {
        auto node(queue.front());

        queue.pop();

        // ignore 'deleted' nodes
        if(((*node).flags & tree::flags::deleted) == tree::flags::deleted)
        {
            continue;
        }

        if(((*node).flags & tree::flags::not_in_ast) == tree::flags::not_in_ast)
        {
            auto papa((*node).papa);

            if(papa != nullptr)
            {
                (*papa).kids.erase((std::remove((*papa).kids.begin(), (*papa).kids.end(), node)));
                (*node).flags |= tree::flags::deleted;
            }

            continue;
        }

        for(std::size_t k = 0, n = (*node).kids.size(); k < n; k++)
        {
            auto kid = (*node).kids[k];
            queue.emplace(std::dynamic_pointer_cast<parse_tree<token_type, tree_traits_type>>(kid));
        }
    }

    // collapse single parents
    std::swap(queue, empty_queue);
    queue.emplace(cst);

    while(!queue.empty())
    {
        auto node(queue.front());

        queue.pop();

        stack.push(node);

        for(std::size_t k = 0, n = (*node).kids.size(); k < n; k++)
        {
            auto kid = (*node).kids[k];
            queue.emplace(std::dynamic_pointer_cast<parse_tree<token_type, tree_traits_type>>(kid));
        }
    }

    while(!stack.empty())
    {
        auto node(stack.top());

        stack.pop();

        // ignore 'deleted' nodes
        if(((*node).flags & tree::flags::deleted) == tree::flags::deleted)
        {
            continue;
        }

        for(;;)
        {
            auto papa((*node).papa);

            if(papa == nullptr) // node is already root
            {
                break;
            }

            if((*papa).papa == nullptr && (*papa).kids.size() == 1)
            {
                (*papa).kids.erase(std::remove((*papa).kids.begin(), (*papa).kids.end(), node));
                (*node).papa = nullptr;

                cst = node;

                break;
            }

            if((*papa).kids.size() == 1)
            {
                (*(*node).papa).flags |= tree::flags::deleted;

                auto it = std::find((*(*papa).papa).kids.begin(), (*(*papa).papa).kids.end(), (*node).papa);

                std::size_t index = it - (*(*papa).papa).kids.begin();

                (*(*papa).papa).kids.erase(it);

                (*node).papa = (*papa).papa;

                (*(*papa).papa).kids.emplace((*(*papa).papa).kids.begin() + index, node);
            }
            else
            {
                break;
            }
        }
    }

    // apply ^ operator
    std::swap(queue, empty_queue);
    queue.emplace(cst);

    while(!queue.empty())
    {
        auto node(queue.front());

        queue.pop();

        stack.push(node);

        for(std::size_t k = 0, n = (*node).kids.size(); k < n; k++)
        {
            auto kid = (*node).kids[k];
            queue.emplace(std::dynamic_pointer_cast<parse_tree<token_type, tree_traits_type>>(kid));
        }
    }

    while(!stack.empty())
    {
        auto node(stack.top());

        stack.pop();

        // ignore 'deleted' nodes
        if(((*node).flags & tree::flags::deleted) == tree::flags::deleted)
        {
            continue;
        }

        if(((*node).flags & tree::flags::root_in_ast) == tree::flags::root_in_ast)
        {
            auto papa((*node).papa);

            if(papa == nullptr) // node is already root
            {
                continue;
            }

            (*papa).flags |= tree::flags::deleted;

            std::for_each((*papa).kids.begin(),
                          (*papa).kids.end(),
                          [&node](const auto& kid)
                          {
                              if(kid != node) (*node).kids.emplace_back(kid);
                          });

            std::for_each((*node).kids.begin(),
                          (*node).kids.end(),
                          [&node](const auto& kid)
                          {
                              (*kid).papa = node;;
                          });

            (*node).papa = (*papa).papa;

            if((*papa).papa != nullptr)
            {
                auto it = std::find((*(*papa).papa).kids.begin(), (*(*papa).papa).kids.end(), papa);

                std::size_t index = it - (*(*papa).papa).kids.begin();

                (*(*papa).papa).kids.erase(it);

                (*(*papa).papa).kids.emplace((*(*papa).papa).kids.begin() + index, node);
            }
            else
            {
                cst = node;
            }
        }
    }
}

template <typename Token, typename TreeTraits>
typename ir<Token, TreeTraits>::dag_key_type ir<Token, TreeTraits>::build_dag_key(const typename ir<Token, TreeTraits>::parse_tree_type& tree)
{
    dag_key_type result;

    result.emplace_back(std::make_tuple((*(*tree).ir_symbol).token().type, (*(*tree).ir_symbol).token().literal, (*(*tree).ir_symbol).value()));

    std::for_each((*tree).kids.begin(),
                  (*tree).kids.end(),
                  [&result](const auto& kid)
                  {
                      auto tree_kid(std::dynamic_pointer_cast<parse_tree<token_type, tree_traits_type>>(kid));
                      result.emplace_back(std::make_tuple((*(*tree_kid).ir_symbol).token().type,
                                                          (*(*tree_kid).ir_symbol).token().literal,
                                                          (*(*tree_kid).ir_symbol).value()));
                  });

    return result;
}

template <typename Token, typename TreeTraits>
typename ir<Token, TreeTraits>::parse_dag_type ir<Token, TreeTraits>::find_dag(const typename ir<Token, TreeTraits>::dag_key_type& key,
                                                                               const typename ir<Token, TreeTraits>::dag_cache_type& cache)
{
    parse_dag_type result;

    auto it = cache.find(key);

    if(it != cache.end())
    {
        result = (*it).second;
    }

    return result;
}

template <typename Token, typename TreeTraits>
void ir<Token, TreeTraits>::ast_to_asd(const typename ir<Token, TreeTraits>::parse_tree_type& ast, typename ir<Token, TreeTraits>::parse_dag_type& result_asd)
{
    // collect nodes

    // 1. Push root to first stack.
    // 2. Loop while first stack is not empty.
    //      2.1 Pop a node from first stack and push it to second stack.
    //      2.2 Push left and right children of the popped node to first stack.
    // 3. Process contents of second stack.
    std::stack<parse_tree_type> stack1;
    std::stack<parse_tree_type> stack2;

    // 1. Push root to first stack.
    stack1.push(ast);

    // 2. Loop while first stack is not empty.
    while(!stack1.empty())
    {
        // 2.1 Pop a node from first stack ...
        auto tree(stack1.top());

        stack1.pop();

        // ... and push it to second stack.
        stack2.push(tree);

        // 2.2 Push left and right children of the popped node to first stack.
        for(std::size_t k = 0, n = (*tree).kids.size(); k < n; k++)
        {
            auto kid = (*tree).kids[k];
            stack1.push(std::dynamic_pointer_cast<parse_tree<token_type, tree_traits_type>>(kid));
        }
    }

    // build dag
    parse_dag_type dag;
    dag_cache_type cache;

    std::size_t id = 0;

    using map_type = std::map<parse_tree_type, parse_dag_type>;
    map_type map; // mapping an original tree node to the new dag node

    while(!stack2.empty())
    {
        auto tree(stack2.top());

        stack2.pop();

        dag_key_type key(build_dag_key(tree));

        dag = find_dag(key, cache);

        //if(dag need to be invalidated, assignement = ...)
        //{
        //    dag = nullptr;
        //}

        if(dag == nullptr)
        {
            dag = factory::create<parse_dag<token_type, tree_traits_type>>();

            (*dag).gr_symbol = (*tree).gr_symbol;
            (*dag).ir_symbol = (*tree).ir_symbol;

            (*dag).id = ++id;

            cache.emplace(std::make_pair(key, dag));

            for(const auto& tree_kid : (*tree).kids)
            {
                auto it(map.find(std::dynamic_pointer_cast<parse_tree<token_type, tree_traits_type>>(tree_kid)));
                auto dag_kid((*it).second);

                (*dag).kids.emplace(dag_kid);
                (*dag_kid).papas.emplace_back(dag);
            }
        }

        map.emplace(std::make_pair(tree, dag));
    }

    result_asd.swap(dag);
}

END_NAMESPACE

#endif // __IR_INL__
