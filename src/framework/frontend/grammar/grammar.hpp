//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class grammar : private noncopyable
{
    public:
        using symbol_type = typename grammar_rule::symbol_type;
        using symbols_type = typename grammar_rule::symbols_type;

        using pool_type = std::map<string_type, symbol_type>;
        using pool_index_type = std::map<std::size_t, symbol_type>;

        using rule_type = std::shared_ptr<grammar_rule>;
        using rules_type = std::vector<rule_type>;

        using nts_rules_type = std::map<string_type, rules_type>; // mapping nonterminal to rules

        using set_type = typename grammar_symbol::set_type;
        using sets_type = typename grammar_symbol::sets_type;

    private:
        rules_type              my_rules;
        pool_type               my_pool;
        pool_index_type         my_indexed_pool;
        nts_rules_type          my_nt_rules;

    private:
        static bool             char_to_skip(char_type ch, const char_type* delimiters);

    public:
                                grammar();
                               ~grammar();

        symbol_type             start_symbol() const;

        const rules_type&       rules() const;
        rules_type&             rules();

        const pool_type&        pool() const;
        pool_type&              pool();

        const pool_index_type&  indexed_pool() const;
        pool_index_type&        indexed_pool();

        const nts_rules_type&   nt_rules() const;
        nts_rules_type&         nt_rules();

        void                    load(const string_type& file_name);
        void                    cleanup();
};

inline const typename grammar::rules_type& grammar::rules() const
{
    return my_rules;
}

inline typename grammar::rules_type& grammar::rules()
{
    return my_rules;
}

inline const typename grammar::pool_type& grammar::pool() const
{
    return my_pool;
}

inline typename grammar::pool_type& grammar::pool()
{
    return my_pool;
}

inline const typename grammar::pool_index_type& grammar::indexed_pool() const
{
    return my_indexed_pool;
}

inline typename grammar::pool_index_type& grammar::indexed_pool()
{
    return my_indexed_pool;
}

inline const typename grammar::nts_rules_type& grammar::nt_rules() const
{
    return my_nt_rules;
}

inline typename grammar::nts_rules_type& grammar::nt_rules()
{
    return my_nt_rules;
}

END_NAMESPACE

#endif // __GRAMMAR_H__
