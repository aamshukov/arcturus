//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitor.hpp>
#include <core/tree.hpp>

#include <frontend/grammar/grammar_symbol.hpp>
#include <frontend/grammar/grammar_rule.hpp>

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

grammar_rule::grammar_rule(std::size_t id, const string_type& name)
            : my_id(id),
              my_name(name),
              my_lhs_terminal_count(0),
              my_lhs_nonterminal_count(0),
              my_rhs_terminal_count(0),
              my_rhs_nonterminal_count(0),
              my_precedence(0),
              my_precedences(0),
              my_flags(flags_type::clear)
{
}

grammar_rule::grammar_rule(const grammar_rule& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_name = other.my_name;

        my_lhs_terminal_count = other.my_lhs_terminal_count;
        my_lhs_nonterminal_count = other.my_lhs_nonterminal_count;

        my_rhs_terminal_count = other.my_rhs_terminal_count;
        my_rhs_nonterminal_count = other.my_rhs_nonterminal_count;

        my_precedence = other.my_precedence;
        my_precedences = other.my_precedences;

        my_lhs = other.my_lhs;
        my_rhs = other.my_rhs;

        my_flags = other.my_flags;
    }
}

grammar_rule::grammar_rule(grammar_rule&& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_name = std::move(other.my_name);

        my_lhs_terminal_count = other.my_lhs_terminal_count;
        my_lhs_nonterminal_count = other.my_lhs_nonterminal_count;

        my_rhs_terminal_count = other.my_rhs_terminal_count;
        my_rhs_nonterminal_count = other.my_rhs_nonterminal_count;

        my_precedence = other.my_precedence;
        my_precedences = other.my_precedences;

        my_lhs = std::move(other.my_lhs);
        my_rhs = std::move(other.my_rhs);

        my_flags = other.my_flags;
    }
}

grammar_rule::~grammar_rule()
{
}

grammar_rule& grammar_rule::operator = (const grammar_rule& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_name = other.my_name;

        my_lhs_terminal_count = other.my_lhs_terminal_count;
        my_lhs_nonterminal_count = other.my_lhs_nonterminal_count;

        my_rhs_terminal_count = other.my_rhs_terminal_count;
        my_rhs_nonterminal_count = other.my_rhs_nonterminal_count;

        my_precedence = other.my_precedence;
        my_precedences = other.my_precedences;

        my_lhs = other.my_lhs;
        my_rhs = other.my_rhs;

        my_flags = other.my_flags;
    }

    return *this;
}

grammar_rule& grammar_rule::operator = (grammar_rule&& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_name = std::move(other.my_name);

        my_lhs_terminal_count = other.my_lhs_terminal_count;
        my_lhs_nonterminal_count = other.my_lhs_nonterminal_count;

        my_rhs_terminal_count = other.my_rhs_terminal_count;
        my_rhs_nonterminal_count = other.my_rhs_nonterminal_count;

        my_precedence = other.my_precedence;
        my_precedences = other.my_precedences;

        my_lhs = std::move(other.my_lhs);
        my_rhs = std::move(other.my_rhs);

        my_flags = other.my_flags;
    }

    return *this;
}

bool grammar_rule::empty() const
{
    bool result = !lhs().empty() && !rhs().empty();

    if(result)
    {
        const grammar_symbol& lhs_symbol(*lhs().front());
        const grammar_symbol& rhs_symbol(*rhs().front());

        result = lhs_symbol.nonterminal() && rhs_symbol.id() == (*grammar_symbol::epsilon).id();
    }

    return result;
}

void grammar_rule::add_lhs_symbol(const typename grammar_rule::symbol_type& sym)
{
    my_lhs.emplace_back(sym);

    if((*sym).nonterminal())
    {
        my_lhs_nonterminal_count++;
    }
    else if((*sym).terminal())
    {
        my_lhs_terminal_count++;
    }
}

void grammar_rule::add_rhs_symbol(const typename grammar_rule::symbol_type& sym)
{
    my_rhs.emplace_back(sym);

    if((*sym).nonterminal())
    {
        my_rhs_nonterminal_count++;
    }
    else if((*sym).terminal())
    {
        my_rhs_terminal_count++;
    }
}

bool operator == (const grammar_rule& rule1, const grammar_rule& rule2)
{
    bool result = rule1.lhs().size() == rule2.lhs().size() && rule1.rhs().size() == rule2.rhs().size();

    if(result)
    {
        result = std::equal(rule1.lhs().begin(),
                            rule1.lhs().end(),
                            rule2.lhs().begin(),
                            [] (const auto& sym1, const auto& sym2)
                            {
                                return (*sym1).id() == (*sym2).id();
                            });
        if(result)
        {
            result = std::equal(rule1.rhs().begin(),
                                rule1.rhs().end(),
                                rule2.rhs().begin(),
                                [] (const auto& sym1, const auto& sym2)
                                {
                                    return (*sym1).id() == (*sym2).id();
                                });
        }
    }

    return result;
}

END_NAMESPACE
