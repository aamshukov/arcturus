//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#include <core/pch.hpp>

#include <core/visitor.hpp>
#include <core/tree.hpp>

#include <frontend/grammar/symbol.hpp>
#include <frontend/grammar/rule.hpp>
#include <frontend/grammar/grammar.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

grammar::grammar()
{
}

grammar::~grammar()
{
}

typename grammar::symbol_type grammar::start_symbol() const
{
    symbol_type result;

    if(!my_rules.empty() && !(*my_rules.front()).lhs().empty())
    {
        result = (*my_rules.front()).lhs().front();
    }

    return result;
}


bool grammar::char_to_skip(char_type ch, const char_type* delimiters)
{
    bool result = false;

    for(std::size_t k = 0; delimiters[k] != 0; k++)
    {
        if(ch == delimiters[k])
        {
            result = true;
            break;
        }
    }

    return result;
}

void grammar::load(const string_type& file_name)
{
    log_info(L"Loading grammar from the file '%s' ...", file_name.c_str());

    #define MAX_LINE_SIZE (4096)

    my_rules.clear();

    const char_type* delimiters(L" \t\n\r");
    const char_type* illegals(L"./");

    using stream_type = std::wifstream;

    stream_type stream;

    stream.open(file_name, std::ios::in | std::ios::binary);

    if(!stream.is_open() || stream.bad() || stream.fail())
    {
        log_error(L"Loading grammar: unable to read grammar from file '%s'.", file_name.c_str());
    }
    else
    {
        std::size_t symbol_number = 0;
        std::size_t rule_number = 0;

        char_type buffer[MAX_LINE_SIZE];

        string_type lhs_symbol_name;

        while(!stream.eof())
        {
            buffer[0] = 0;

            stream.getline(buffer, array_size(buffer));

            buffer[stream.gcount() > 0 ? stream.gcount() - 1 : 0] = 0;

            if(buffer[0] == 0)
            {
                if(!stream.eof())
                {
                    log_error(L"Loading grammar: unable to read grammar from file '%s' - invalid line", file_name.c_str());
                }

                break;
            }

            std::size_t offset = 0;

            while(buffer[offset] != 0 && char_to_skip(buffer[offset], delimiters)) // skip delimiters
            {
                offset++;
            }

            if(buffer[offset] == 0 || char_to_skip(buffer[offset], illegals)) // skip delimiters and comments
            {
                continue;
            }

            if(lhs_symbol_name.empty())
            {
                std::size_t lhs_start = offset;
                std::size_t lhs_count = 0;

                while(buffer[offset] != 0 && !char_to_skip(buffer[offset], delimiters) && buffer[offset] != L':') // populate lhs
                {
                    lhs_count++;
                    offset++;
                }

                while(buffer[offset] != 0 && char_to_skip(buffer[offset], delimiters)) // skip delimiters
                {
                    offset++;
                }

                if(buffer[offset] == 0)
                {
                    log_error(L"Loading grammar: unable to read grammar from file '%s' - invalid line", file_name.c_str());
                    break;
                }
                else
                {
                    offset++; // skip ':'
                }

                if(lhs_count == 0)
                {
                    continue;
                }

                lhs_symbol_name.assign(buffer, lhs_start, std::max(static_cast<std::size_t>(0), lhs_count));
            }
            else
            {
                while(buffer[offset] != 0 && char_to_skip(buffer[offset], delimiters) && buffer[offset] != L'|' && buffer[offset] != L';')
                {
                    offset++;
                }

                if(buffer[offset] == 0 || buffer[offset] == L';')
                {
                    lhs_symbol_name.clear();
                    continue;
                }

                offset++; // skip '|'
            }

            // add lhs
            auto lhs_it(my_pool.find(lhs_symbol_name));

            symbol_type lhs_symbol;

            if(lhs_it != my_pool.end())
            {
                lhs_symbol = (*lhs_it).second;
            }
            else
            {
                lhs_symbol = (factory::create<symbol>(symbol_number++, lhs_symbol_name, symbol::kind::nonterminal));

                my_pool.insert(pool_type::value_type(lhs_symbol_name, lhs_symbol));
                my_indexed_pool.insert(pool_index_type::value_type((*lhs_symbol).id(), lhs_symbol));
            }

            rule_type rule0(factory::create<rule>(rule_number++, lhs_symbol_name)); // rule name is the LHS's symbol name

            (*rule0).add_lhs_symbol(lhs_symbol);

            my_rules.emplace_back(rule0);

            // add to nonterminal-rules map
            auto nt_it(my_nt_rules.find((*lhs_symbol).name()));

            if(nt_it == my_nt_rules.end())
            {
                // new entry
                rules_type rules;
                rules.emplace_back(rule0);
                my_nt_rules.emplace(nts_rules_type::value_type((*lhs_symbol).name(), std::move(rules)));
            }
            else
            {
                auto& rules((*nt_it).second);
                rules.emplace_back(rule0);
            }

            // add rhs
            string_type rhs_line(buffer + offset);

            std::size_t current;
            std::size_t next = string_type::npos;

            do
            {
                current = next + 1;

                next = rhs_line.find_first_of(delimiters, current);

                string_type rhs_symbol_name(rhs_line.substr(current, next - current));

                if(!rhs_symbol_name.empty())
                {
                    if(rhs_symbol_name.compare(L"λ") == 0 || rhs_symbol_name.compare(L"ε") == 0 || rhs_symbol_name.compare(L"e(psilon)") == 0)
                    {
                        rhs_symbol_name = (*symbol::epsilon).name();
                    }
                    else if(rhs_symbol_name.compare(L"#") == 0)
                    {
                        rhs_symbol_name = (*symbol::op_mark).name();
                    }

                    if(rhs_symbol_name.compare((*symbol::epsilon).name()) == 0)
                    {
                        (*rule0).add_rhs_symbol(symbol::epsilon);

                        my_pool.insert(pool_type::value_type(rhs_symbol_name, symbol::epsilon));
                        my_indexed_pool.insert(pool_index_type::value_type((*symbol::epsilon).id(), symbol::epsilon));
                    }
                    else if(rhs_symbol_name.compare((*symbol::op_mark).name()) == 0)
                    {
                        (*rule0).add_rhs_symbol(symbol::op_mark);

                        my_pool.insert(pool_type::value_type(rhs_symbol_name, symbol::op_mark));
                        my_indexed_pool.insert(pool_index_type::value_type((*symbol::op_mark).id(), symbol::op_mark));
                    }
                    else
                    {
                        symbol::kind kind = symbol::kind::nonterminal;

                        symbol_type rhs_symbol;

                        if(rhs_symbol_name[rhs_symbol_name.size() - 1] == L'^')
                        {
                            rhs_symbol_name = rhs_symbol_name.substr(0, rhs_symbol_name.size() - 1);
                            (*rule0).ast_operators()[(*rule0).rhs().size()] = tree::flags_type::root_in_ast;
                        }
                        else if(rhs_symbol_name[rhs_symbol_name.size() - 1] == L'!')
                        {
                            rhs_symbol_name = rhs_symbol_name.substr(0, rhs_symbol_name.size() - 1);
                            (*rule0).ast_operators()[(*rule0).rhs().size()] = tree::flags_type::not_in_ast;
                        }

                        if(rhs_symbol_name[0] == L'\'' && rhs_symbol_name.size() > 1)
                        {
                            rhs_symbol_name = rhs_symbol_name.substr(1, rhs_symbol_name.size() - 2); // -2 for ''
                            kind = symbol::kind::terminal;
                        }

                        auto rhs_it(my_pool.find(rhs_symbol_name));

                        if(rhs_it != my_pool.end())
                        {
                            rhs_symbol = (*rhs_it).second;
                        }
                        else
                        {
                            rhs_symbol = (factory::create<symbol>(symbol_number++, rhs_symbol_name, kind));

                            my_pool.insert(pool_type::value_type(rhs_symbol_name, rhs_symbol));
                            my_indexed_pool.insert(pool_index_type::value_type((*rhs_symbol).id(), rhs_symbol));
                        }

                        (*rule0).add_rhs_symbol(rhs_symbol);
                    }
                }
            }
            while(next != string_type::npos);
        }
    }

    stream.close();

    cleanup();

    log_info(L"Loaded grammar from the file '%s'.", file_name.c_str());
}

void grammar::cleanup()
{
    for(const auto& symb_kvp : my_pool)
    {
        const auto& pool_symb(symb_kvp.second);

        (*pool_symb).first_sets().clear();
        (*pool_symb).eff_sets().clear();
        (*pool_symb).follow_sets().clear();

        (*pool_symb).nullable() = false;
    }
}

END_NAMESPACE
