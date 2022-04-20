//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __RULE_H__
#define __RULE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class grammar_rule
{
    public:
        using symbol_type = typename grammar_symbol::symbol_type;
        using symbols_type = typename grammar_symbol::symbols_type;

        enum class flag : uint64_t
        {
            clear = 0x00
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

        using ast_operators_type = std::map<std::size_t, tree::flags_type>;

    private:
        std::size_t                 my_id; // enumerable type, will be cast to specific enum values
        string_type                 my_name;

        uint8_t                     my_lhs_terminal_count;
        uint8_t                     my_lhs_nonterminal_count;

        uint8_t                     my_rhs_terminal_count;
        uint8_t                     my_rhs_nonterminal_count;

        uint8_t                     my_precedence;  // production either inherits the precedence number of the rightmost terminal in the RHS or has explicitly set it up
        uint8_t                     my_precedences; // context dependent precedence level of terminals for the rule (shift/reduce parsers)

        symbols_type                my_lhs;
        symbols_type                my_rhs;

        flags_type                  my_flags;

        ast_operators_type          my_ast_operators; // indices of RHS

    public:
                                    grammar_rule(std::size_t id, const string_type& name);

                                    grammar_rule(const grammar_rule& other);
                                    grammar_rule(grammar_rule&& other);

                                   ~grammar_rule();

        grammar_rule&                       operator = (const grammar_rule& other);
        grammar_rule&                       operator = (grammar_rule&& other);

        std::size_t                 id() const;
        std::size_t&                id();

        const string_type&          name() const;
        string_type&                name();

        uint8_t                     lhs_terminal_count() const;
        uint8_t&                    lhs_terminal_count();

        uint8_t                     lhs_nonterminal_count() const;
        uint8_t&                    lhs_nonterminal_count();

        uint8_t                     rhs_terminal_count() const;
        uint8_t&                    rhs_terminal_count();

        uint8_t                     rhs_nonterminal_count() const;
        uint8_t&                    rhs_nonterminal_count();

        uint8_t                     precedence() const;
        uint8_t&                    precedence();

        uint8_t                     precedences() const;
        uint8_t&                    precedences();

        const symbols_type&         lhs() const;
        symbols_type&               lhs();

        const symbols_type&         rhs() const;
        symbols_type&               rhs();

        flags_type                  flags() const;
        flags_type&                 flags();

        const ast_operators_type&   ast_operators() const;
        ast_operators_type&         ast_operators();

        bool                        empty() const; // A -> λ

        void                        add_lhs_symbol(const symbol_type& sym);
        void                        add_rhs_symbol(const symbol_type& sym);

        friend bool                 operator == (const grammar_rule& rule1, const grammar_rule& rule2);
        friend bool                 operator != (const grammar_rule& rule1, const grammar_rule& rule2);
};

inline std::size_t grammar_rule::id() const
{
    return my_id;
}

inline std::size_t& grammar_rule::id()
{
    return my_id;
}

inline const string_type& grammar_rule::name() const
{
    return my_name;
}

inline string_type& grammar_rule::name()
{
    return const_cast<string_type&>(static_cast<const grammar_rule&>(*this).name());
}

inline uint8_t grammar_rule::lhs_terminal_count() const
{
    return my_lhs_terminal_count;
}

inline uint8_t& grammar_rule::lhs_terminal_count()
{
    return my_lhs_terminal_count;
}

inline uint8_t grammar_rule::lhs_nonterminal_count() const
{
    return my_lhs_nonterminal_count;
}

inline uint8_t& grammar_rule::lhs_nonterminal_count()
{
    return my_lhs_nonterminal_count;
}

inline uint8_t grammar_rule::rhs_terminal_count() const
{
    return my_rhs_terminal_count;
}

inline uint8_t& grammar_rule::rhs_terminal_count()
{
    return my_rhs_terminal_count;
}

inline uint8_t grammar_rule::rhs_nonterminal_count() const
{
    return my_rhs_nonterminal_count;
}

inline uint8_t& grammar_rule::rhs_nonterminal_count()
{
    return my_rhs_nonterminal_count;
}

inline uint8_t grammar_rule::precedence() const
{
    return my_precedence;
}

inline uint8_t& grammar_rule::precedence()
{
    return my_precedence;
}

inline uint8_t grammar_rule::precedences() const
{
    return my_precedences;
}

inline uint8_t& grammar_rule::precedences()
{
    return my_precedences;
}

inline const grammar_rule::symbols_type& grammar_rule::lhs() const
{
    return my_lhs;
}

inline grammar_rule::symbols_type& grammar_rule::lhs()
{
    return const_cast<grammar_rule::symbols_type&>(static_cast<const grammar_rule&>(*this).lhs());
}

inline const grammar_rule::symbols_type& grammar_rule::rhs() const
{
    return my_rhs;
}

inline grammar_rule::symbols_type& grammar_rule::rhs()
{
    return const_cast<grammar_rule::symbols_type&>(static_cast<const grammar_rule&>(*this).rhs());
}

inline typename grammar_rule::flags_type grammar_rule::flags() const
{
    return my_flags;
}

inline typename grammar_rule::flags_type& grammar_rule::flags()
{
    return my_flags;
}

inline const typename grammar_rule::ast_operators_type& grammar_rule::ast_operators() const
{
    return my_ast_operators;
}

inline typename grammar_rule::ast_operators_type& grammar_rule::ast_operators()
{
    return my_ast_operators;
}

inline bool operator != (const grammar_rule& lhs, const grammar_rule& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE

#endif // __RULE_H__
