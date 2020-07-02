//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/unicode.hpp>
#include <core/text.hpp>
#include <core/enum.hpp>

#include <core/domain_helper.hpp>

#include <core/factory.hpp>

#include <frontend/grammar/symbol.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

typename symbol::symbol_type symbol::epsilon(factory::create<symbol>(std::numeric_limits<int32_t>::max() - 1, L"e", symbol::kind::terminal));
typename symbol::symbol_type symbol::eof(factory::create<symbol>(std::numeric_limits<int32_t>::max() - 2, L"$", symbol::kind::terminal));
typename symbol::symbol_type symbol::op_mark(factory::create<symbol>(std::numeric_limits<int32_t>::max() - 3, L"#", symbol::kind::terminal));

symbol::symbol(uint32_t id, const string_type& name, symbol::kind type)
      : my_id(id),
        my_name(name),
        my_kind(type),
        my_flags(flags_type::clear),
        my_nullable(false),
        my_productive(false),
        my_accessible(false),
        my_precedence(0),
        my_associativity(symbol::associativity_type::left),
        my_erroneous(false)
{
}

symbol::symbol(const symbol& other)
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

symbol::symbol(symbol&& other)
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

symbol::~symbol()
{
}

symbol& symbol::operator = (const symbol& other)
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

symbol& symbol::operator = (symbol&& other)
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
