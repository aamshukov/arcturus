//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_H__
#define __IR_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)
//
//template <typename Token>
//class ir : private noncopyable
//{
//    public:
//        using token_type = Token;
//
//        using parse_tree_type = typename parser<token_type, tree_traits_type>::parse_tree_type;
//        using parse_trees_type = typename parser<token_type, tree_traits_type>::parse_trees_type;
//
//        using parse_dag_type = typename parser<token_type, tree_traits_type>::parse_dag_type;
//        using parse_dags_type = typename parser<token_type, tree_traits_type>::parse_dags_type;
//
//        using symbol_type = std::shared_ptr<symtable::symbol<token_type>>;
//        using symbols_type = std::vector<symbol_type>;
//
//        using dag_key_pair = std::tuple<typename token_type::token_type,
//                                        typename token_type::codepoints_type,
//                                        typename symtable::symbol<token_type>::value_type>;
//        using dag_key_type = std::vector<dag_key_pair>;
//
//        struct dag_hash
//        {
//            std::size_t operator () (const dag_key_type& key) const
//            {
//                std::size_t result = key.size();
//
//                for(auto e : key)
//                {
//                    result ^= std::hash<std::size_t>{}(static_cast<std::size_t>(std::get<0>(e))) + 0x9E3779B9 + (result << 6) + (result >> 2); // aka boost hash_combine
//                }
//
//                return result;
//            }
//        };
//
//        struct dag_key_comparator
//        {
//            bool operator () (const dag_key_type& lhs, const dag_key_type& rhs) const
//            {
//                return lhs == rhs;
//            }
//        };
//
//        using dag_cache_type = std::unordered_map<dag_key_type, parse_dag_type, dag_hash, dag_key_comparator>;
//
//        using kids_type = std::vector<parse_dag_type>;
//
//        using quadruple_type = quadruple<token_type>;
//        using code_type = std::list<quadruple_type>;
//
//    private:
//        static dag_key_type     build_dag_key(const parse_tree_type& tree);
//        static parse_dag_type   find_dag(const dag_key_type& key, const dag_cache_type& cache);
//
//    public:
//        static void             cst_to_ast(parse_tree_type& cst);
//        static void             ast_to_asd(const parse_tree_type& ast, parse_dag_type& result_asd);
//};

END_NAMESPACE

#endif // __IR_H__
