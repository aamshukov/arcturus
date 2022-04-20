//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <frontend/grammar/grammar_symbol.hpp>

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

typename grammar_symbol::symbol_type grammar_symbol::epsilon(factory::create<grammar_symbol>(std::numeric_limits<int32_t>::max() - 1, L"e", grammar_symbol::kind::terminal));
typename grammar_symbol::symbol_type grammar_symbol::eof(factory::create<grammar_symbol>(std::numeric_limits<int32_t>::max() - 2, L"$", grammar_symbol::kind::terminal));
typename grammar_symbol::symbol_type grammar_symbol::op_mark(factory::create<grammar_symbol>(std::numeric_limits<int32_t>::max() - 3, L"#", grammar_symbol::kind::terminal));

grammar_symbol::grammar_symbol(std::size_t id, const string_type& name, grammar_symbol::kind type)
              : my_id(id),
                my_name(name),
                my_kind(type),
                my_flags(flags_type::clear),
                my_nullable(false),
                my_productive(false),
                my_accessible(false),
                my_precedence(0),
                my_associativity(grammar_symbol::associativity_type::left),
                my_erroneous(false)
{
}

grammar_symbol::grammar_symbol(const grammar_symbol& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_name = other.my_name;

        my_kind = other.my_kind;

        my_flags = other.my_flags;

        my_nullable = other.my_nullable;
        my_productive = other.my_productive;
        my_accessible = other.my_accessible;

        my_precedence = other.my_precedence;
        my_associativity = other.my_associativity;

        my_erroneous = other.my_erroneous;

        my_first_sets = other.my_first_sets;
        my_follow_sets = other.my_follow_sets;

        my_eff_sets = other.my_eff_sets;

        my_la_sets = other.my_la_sets;

        my_semantic_action = other.my_semantic_action;
    }
}

grammar_symbol::grammar_symbol(grammar_symbol&& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_name = std::move(other.my_name);

        my_kind = other.my_kind;

        my_flags = other.my_flags;

        my_nullable = other.my_nullable;
        my_productive = other.my_productive;
        my_accessible = other.my_accessible;

        my_precedence = other.my_precedence;
        my_associativity = other.my_associativity;

        my_erroneous = other.my_erroneous;

        my_first_sets = std::move(other.my_first_sets);
        my_follow_sets = std::move(other.my_follow_sets);

        my_eff_sets = std::move(other.my_eff_sets);

        my_la_sets = std::move(other.my_la_sets);

        my_semantic_action = std::move(other.my_semantic_action);
    }
}

grammar_symbol::~grammar_symbol()
{
}

grammar_symbol& grammar_symbol::operator = (const grammar_symbol& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_name = other.my_name;

        my_kind = other.my_kind;

        my_flags = other.my_flags;

        my_nullable = other.my_nullable;
        my_productive = other.my_productive;
        my_accessible = other.my_accessible;

        my_precedence = other.my_precedence;
        my_associativity = other.my_associativity;

        my_erroneous = other.my_erroneous;

        my_first_sets = other.my_first_sets;
        my_follow_sets = other.my_follow_sets;

        my_eff_sets = other.my_eff_sets;

        my_la_sets = other.my_la_sets;

        my_semantic_action = other.my_semantic_action;
    }

    return *this;
}

grammar_symbol& grammar_symbol::operator = (grammar_symbol&& other)
{
    if(this != &other)
    {
        my_id = other.my_id;
        my_name = std::move(other.my_name);

        my_kind = other.my_kind;

        my_flags = other.my_flags;

        my_nullable = other.my_nullable;
        my_productive = other.my_productive;
        my_accessible = other.my_accessible;

        my_precedence = other.my_precedence;
        my_associativity = other.my_associativity;

        my_erroneous = other.my_erroneous;

        my_first_sets = std::move(other.my_first_sets);
        my_follow_sets = std::move(other.my_follow_sets);

        my_eff_sets = std::move(other.my_eff_sets);

        my_la_sets = std::move(other.my_la_sets);

        my_semantic_action = std::move(other.my_semantic_action);
    }

    return *this;
}

END_NAMESPACE
