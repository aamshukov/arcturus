//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <core/visitor.hpp>
#include <core/tree.hpp>

#include <frontend/grammar/grammar_symbol.hpp>
#include <frontend/grammar/grammar_rule.hpp>
#include <frontend/grammar/grammar.hpp>
#include <frontend/grammar/grammar_algorithm.hpp>
#include <frontend/grammar/grammar_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

string_type grammar_visualization::decorate_set(const typename grammar_visualization::set_type& set, bool add_brackets)
{
    string_type result;

    if(add_brackets)
    {
        result += L"{ ";
    }

    for(const auto& symbol : set)
    {
        result += (*symbol).name();
        result += L" ";
    }

    if(add_brackets)
    {
        result += L"} ";
    }

    return result;
}

string_type grammar_visualization::decorate_sets(const typename grammar_visualization::sets_type& sets, bool add_brackets)
{
    string_type result;

    if(add_brackets)
    {
        result += L"{ ";
    }

    for(const auto& set : sets)
    {
        result += decorate_set(set);
    }

    if(add_brackets)
    {
        result += L"} ";
    }

    return result;
}

string_type grammar_visualization::decorate_symbol(const typename grammar_visualization::symbol_type& sym)
{
    string_type result;

    char_type buffer[64];

    snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%4ld] ", static_cast<uint32_t>((*sym).id()));

    result += buffer;
    result += (*sym).name();

    if((*sym).name().compare(L":") == 0)
    {
        result += L" (colon) ";
    }
    else if((*sym).name().compare(L"::") == 0)
    {
        result += L" (colon)(colon) ";
    }

    result += L": ";

    if((*sym).nonterminal())
    {
        result += L"NON-TERMINAL  ";
    }
    else
    {
        result += L"TERMINAL  ";
    }

    if((*sym).nonterminal())
    {
        if((*sym).nullable())
        {
            result += L"NULLABLE  ";
        }
        else
        {
            result += L"NON-NULLABLE  ";
        }
    }

    if((*sym).nonterminal())
    {
        if((*sym).productive())
        {
            result += L"TERMINAL DERIVABLE (PRODUCTIVE): TRUE  ";
        }
        else
        {
            result += L"TERMINAL DERIVABLE (PRODUCTIVE): FALSE  ";
        }

        if((*sym).accessible())
        {
            result += L"ACCESSIBLE: TRUE  ";
        }
        else
        {
            result += L"ACCESSIBLE: FALSE  ";
        }
    }

    snwprintf(buffer, array_size(buffer), TRUNCATE, L"PRECEDENCE: %ld  ", (*sym).precedence());
    result += buffer;

    snwprintf(buffer, array_size(buffer), TRUNCATE, L"ASSOCIATIVITY: %ld  ", (*sym).associativity());
    result += buffer;

    if((*sym).erroneous())
    {
        result += L"ERRONEOUS: TRUE  ";
    }
    else
    {
        result += L"ERRONEOUS: FALSE  ";
    }

    result += L"\n        FIRST SET:  ";
    result += decorate_sets((*sym).first_sets());

    result += L"\n        EFF SET:    ";
    result += decorate_sets((*sym).eff_sets());

    result += L"\n        FOLLOW SET: ";
    result += decorate_sets((*sym).follow_sets());

    result += L"\n        LA SET: ";
    result += decorate_sets((*sym).la_sets());

    result += L"\n\n        SEMANTICS: ";

    result += (*sym).semantic_action();

    result += L"\n\n";

    return result;
}

string_type grammar_visualization::decorate_rule(const typename grammar_visualization::rule_type& rule, bool decorate)
{
    string_type result;

    char_type buffer[64];

    for(const auto& sym : (*rule).lhs())
    {
        if(decorate)
        {
            snwprintf(buffer, array_size(buffer), TRUNCATE, L"[%4ld] ", static_cast<uint32_t>((*rule).id()));

            result += buffer;

            if((*sym).terminal() && (*sym).id() != (*grammar_symbol::epsilon).id() && (*sym).id() != (*grammar_symbol::op_mark).id())
            {
                result += L"'";
            }
        }

        result += (*sym).name();

        if(decorate)
        {
            if((*sym).terminal() && (*sym).id() != (*grammar_symbol::epsilon).id() && (*sym).id() != (*grammar_symbol::op_mark).id())
            {
                result += L"'";
            }
        }

        result += L" ";
    }

    result += L": ";

    for(const auto& sym : (*rule).rhs())
    {
        if(decorate)
        {
            if((*sym).terminal() && (*sym).id() != (*grammar_symbol::epsilon).id() && (*sym).id() != (*grammar_symbol::op_mark).id())
            {
                result += L"'";
            }
        }

        result += (*sym).name();

        if(decorate)
        {
            if((*sym).terminal() && (*sym).id() != (*grammar_symbol::epsilon).id() && (*sym).id() != (*grammar_symbol::op_mark).id())
            {
                result += L"'";
            }
        }

        result += L" ";
    }

    return result;
}

string_type grammar_visualization::decorate_symbols(const grammar& grammar)
{
    string_type result;

    std::vector<symbol_type> symbols;

    std::for_each(grammar.pool().begin(), grammar.pool().end(), [&symbols](const auto& kvp) { symbols.emplace_back(kvp.second); });

    std::sort(symbols.begin(),
              symbols.end(),
              [] (const symbol_type& sym1, const symbol_type& sym2)
              {
                   return (*sym1).id() < (*sym2).id();
              });

    auto last = std::unique(symbols.begin(), symbols.end());

    for(const auto& sym : symbols)
    {
        result += decorate_symbol(sym);
    }

    result += decorate_symbol(grammar_symbol::epsilon);
    result += decorate_symbol(grammar_symbol::eof);
    result += decorate_symbol(grammar_symbol::op_mark);

    return result;
}

string_type grammar_visualization::decorate_grammar(const grammar& grammar)
{
    string_type result;

    for(const auto& rule : grammar.rules())
    {
        result += decorate_rule(rule);
        result += L"\n";
    }

    return result;
}

END_NAMESPACE
