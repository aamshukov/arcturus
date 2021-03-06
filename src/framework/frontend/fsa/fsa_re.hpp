//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FSA_RE_H__
#define __FSA_RE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

#define IDENTIFIER_START (L"S") // should not be a part of any set of chars - keywords or any special symbols in language
#define IDENTIFIER_PART  (L"Q") // should not be a part of any set of chars - keywords or any special symbols in language

class fsa_re : private noncopyable
{
    public:
        using token_type = typename fsa::token_type;

        using state_type = typename fsa::state_type;
        using fsa_type = typename fsa::fsa_type;

    public:
        struct fsa_tree : public tree
        {
            cp_type symbol;

            std::size_t index;

            std::vector<std::size_t> firstpos;
            std::vector<std::size_t> lastpos;

            bool nullable;
        };

        using tree_type = std::shared_ptr<fsa_tree>;
        using trees_type = std::vector<tree_type>;

        using followpos_type = std::vector<std::vector<std::size_t>>;

        using dstate_type = std::pair<std::vector<std::size_t>, state_type>; // = [{1,2,3} : state]
        using dstates_type = std::vector<dstate_type>;

        struct tree_key_comparator
        {
            bool operator() (const tree_type& lhs, const tree_type& rhs) const
            {
                return (*lhs).symbol < (*rhs).symbol;
            }
        };

        using leaves_type = std::multimap<tree_type, std::size_t, tree_key_comparator>;

    private:
        static cp_type      get_codepoint(const cp_type*& p_src);
        static int8_t       get_operator_precedence(cp_type op);

        static bool         is_literal(cp_type ch);

        static std::size_t  preprocess(const std::shared_ptr<cp_type[]>& infix_re,
                                       std::size_t count,
                                       std::shared_ptr<cp_type[]>& processed_re);

        static bool         infix_to_postfix(const std::shared_ptr<cp_type[]>& infix_re,
                                             std::size_t count,
                                             std::shared_ptr<cp_type[]>& postfix_refsa);
        static tree_type    postfix_to_tree(const std::shared_ptr<cp_type[]>& postfix_re,
                                            leaves_type& leaves,
                                            std::size_t& finalpos,
                                            std::size_t& terminals);

        static string_type  postfix_re_to_string(const std::shared_ptr<cp_type[]>& postfix_re, std::size_t count);

        static bool         process_combine(std::stack<fsa::fsa_type>& fragments);
        static bool         process_concatenate(std::stack<fsa::fsa_type>& fragments);
        static bool         process_zero_or_more(std::stack<fsa::fsa_type>& fragments);
        static bool         process_one_or_more(std::stack<fsa::fsa_type>& fragments);
        static bool         process_zero_or_one(std::stack<fsa::fsa_type>& fragments);
        static bool         process_literal(const cp_type*& p_src, std::stack<fsa::fsa_type>& fragments);

        static void         adjust_predicates(fsa_type& fsa0);
        static void         add_escape_state(fsa_type& fsa0, token_type escape_token, const string_type& escape_predicate);

        static void         calculate_nullable(tree_type& tree);

        static void         calculate_first_position(tree_type& tree);
        static void         calculate_last_position(tree_type& tree);

        static void         calculate_follow_position(const tree_type& node, followpos_type& result);

        static void         make_vector_unique(std::vector<std::size_t>& sequence);

        static std::size_t  get_unmarked_state(const dstates_type& dstates);
        static std::size_t  get_state(const dstates_type& dstates, const std::vector<std::size_t>& dstate);
        static string_type  build_state_label(const std::vector<std::size_t>& indices);

        static void         print_fsa_tree(const tree_type& tree, std::size_t level, std::wostream& stream);
        static void         print_fsa_tree(const tree_type& tree, std::wostream& stream);

        static void         print_fsa_followpos(const followpos_type& followpos, std::wostream& stream);

    public:
        static bool         re_to_fsa(const std::shared_ptr<cp_type[]>& re,
                                      std::size_t count,
                                      token_type token,
                                      token_type escape_token,
                                      const string_type& escape_predicate,
                                      fsa_type& result_fsa);
        static bool         re_to_dfa(const string_type& re,
                                      token_type token,
                                      token_type escape_token,
                                      const string_type& escape_predicate,
                                      fsa_type& result_fsa);

        static bool         fsa_to_re_kleene(const fsa_type& fsa, std::basic_string<cp_type>& re);
        static bool         fsa_to_re_state_elimination(const fsa_type& fsa, std::basic_string<cp_type>& re);
        static bool         fsa_to_re_arden(const fsa_type& fsa, std::basic_string<cp_type>& re);
};

END_NAMESPACE

#endif // __FSA_RE_H__
