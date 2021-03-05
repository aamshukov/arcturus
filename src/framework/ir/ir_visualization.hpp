//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __IR_VISUALIZATION_H__
#define __IR_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)
USINGNAMESPACE(symtable)

template <typename TreeTraits>
class ir_visualization : private noncopyable
{
    public:
        using symbol_type = typename grammar::symbol_type;
        using symbols_type = typename grammar::symbols_type;

        using set_type = typename frontend::symbol::set_type;
        using sets_type = typename frontend::symbol::sets_type;

        using rule_type = typename grammar::rule_type;
        using rules_type = typename grammar::rules_type;

        using ir_type = typename ir<TreeTraits>;
        using token_type = typename ir_type::token_type;
        using tree_traits_type = typename ir_type::tree_traits_type;

        using parse_tree_type = typename ir_type::parse_tree_type;
        using parse_trees_type = typename ir_type::parse_trees_type;

        using parse_dag_type = typename ir_type::parse_dag_type;
        using parse_dags_type = typename ir_type::parse_dags_type;

    private:
        static void collect_tree_dot_labels(const parse_tree_type& root, std::wostream& stream);
        static void build_tree_dot_graph(const parse_tree_type& tree, std::wostream& stream);

        static void collect_dag_dot_labels(const parse_dag_type& root, std::wostream& stream);
        static void build_dag_dot_graph(const parse_dag_type& dag, std::wostream& stream);

    public:
        static void print_tree(const parse_tree_type& tree, std::size_t level, std::wostream& stream);
        static void print_trees(const parse_trees_type& trees, std::wostream& stream);

        static void decorate_tree(const parse_tree_type& tree, const string_type& dot_file_name, std::size_t k);
        static void decorate_trees(const parse_trees_type& trees, const string_type& dot_file_name);

        static void decorate_dag(const parse_dag_type& dag, const string_type& dot_file_name);
};

END_NAMESPACE

#endif // __IR_VISUALIZATION_H__
