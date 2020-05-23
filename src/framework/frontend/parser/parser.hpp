//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PARSER_H__
#define __PARSER_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

template <typename Token, typename TreeTraits>
class parser : private noncopyable
{
    public:
        using token_type = typename parse_tree<Token, TreeTraits>::token_type;
        using tokens_type = std::vector<token_type>;

        using tree_traits_type = typename parse_tree<Token, TreeTraits>::tree_traits_type;

        using parse_tree_type = std::shared_ptr<parse_tree<token_type, tree_traits_type>>;
        using parse_trees_type = std::vector<parse_tree_type>;

        using parse_dag_type = std::shared_ptr<parse_dag<token_type, tree_traits_type>>;
        using parse_dags_type = std::vector<parse_dag_type>;

        using lexical_analyzer_type = std::shared_ptr<lexical_analyzer<token_type>>;
        using lexical_analyzers_type = std::vector<lexical_analyzer_type>;

    protected:
        lexical_analyzer_type   my_lexical_analyzer;    // master lexer
        lexical_analyzers_type  my_lexical_analyzers;   // slave lexers, for example migh be introduced by #include(C/C++) or by import(arktur)

        parse_trees_type        my_trees;

        operation_status        my_status;

    protected:
        virtual void            parse() = 0;

        virtual tokens_type     synchronization_tokens(); // anchor sets, not pure virtual cause another method for error handling might be used ...

    public:
                                parser(const lexical_analyzer_type& lexical_analyzer);
                               ~parser();

        const parse_trees_type& trees() const;

        const operation_status& status() const;
        operation_status&       status();
};

template <typename Token, typename TreeTraits>
const typename parser<Token, TreeTraits>::parse_trees_type& parser<Token, TreeTraits>::trees() const
{
    return my_trees;
}

template <typename Token, typename TreeTraits>
const operation_status& parser<Token, TreeTraits>::status() const
{
    return my_status;
}

template <typename Token, typename TreeTraits>
operation_status& parser<Token, TreeTraits>::status()
{
    return my_status;
}

END_NAMESPACE

#endif // __PARSER_H__
