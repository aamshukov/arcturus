﻿//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __RULE_H__
#define __RULE_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class rule
{
    public:
        using symbol_type = typename symbol::symbol_type;
        using symbols_type = typename symbol::symbols_type;

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
                                    rule(std::size_t id, const string_type& name);

                                    rule(const rule& other);
                                    rule(rule&& other);

                                   ~rule();

        rule&                       operator = (const rule& other);
        rule&                       operator = (rule&& other);

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

        friend bool                 operator == (const rule& rule1, const rule& rule2);
        friend bool                 operator != (const rule& rule1, const rule& rule2);
};

inline std::size_t rule::id() const
{
    return my_id;
}

inline std::size_t& rule::id()
{
    return my_id;
}

inline const string_type& rule::name() const
{
    return my_name;
}

inline string_type& rule::name()
{
    return const_cast<string_type&>(static_cast<const rule&>(*this).name());
}

inline uint8_t rule::lhs_terminal_count() const
{
    return my_lhs_terminal_count;
}

inline uint8_t& rule::lhs_terminal_count()
{
    return my_lhs_terminal_count;
}

inline uint8_t rule::lhs_nonterminal_count() const
{
    return my_lhs_nonterminal_count;
}

inline uint8_t& rule::lhs_nonterminal_count()
{
    return my_lhs_nonterminal_count;
}

inline uint8_t rule::rhs_terminal_count() const
{
    return my_rhs_terminal_count;
}

inline uint8_t& rule::rhs_terminal_count()
{
    return my_rhs_terminal_count;
}

inline uint8_t rule::rhs_nonterminal_count() const
{
    return my_rhs_nonterminal_count;
}

inline uint8_t& rule::rhs_nonterminal_count()
{
    return my_rhs_nonterminal_count;
}

inline uint8_t rule::precedence() const
{
    return my_precedence;
}

inline uint8_t& rule::precedence()
{
    return my_precedence;
}

inline uint8_t rule::precedences() const
{
    return my_precedences;
}

inline uint8_t& rule::precedences()
{
    return my_precedences;
}

inline const rule::symbols_type& rule::lhs() const
{
    return my_lhs;
}

inline rule::symbols_type& rule::lhs()
{
    return const_cast<rule::symbols_type&>(static_cast<const rule&>(*this).lhs());
}

inline const rule::symbols_type& rule::rhs() const
{
    return my_rhs;
}

inline rule::symbols_type& rule::rhs()
{
    return const_cast<rule::symbols_type&>(static_cast<const rule&>(*this).rhs());
}

inline typename rule::flags_type rule::flags() const
{
    return my_flags;
}

inline typename rule::flags_type& rule::flags()
{
    return my_flags;
}

inline const typename rule::ast_operators_type& rule::ast_operators() const
{
    return my_ast_operators;
}

inline typename rule::ast_operators_type& rule::ast_operators()
{
    return my_ast_operators;
}

inline bool operator != (const rule& lhs, const rule& rhs)
{
    return !(lhs == rhs);
}

END_NAMESPACE

#endif // __RULE_H__
