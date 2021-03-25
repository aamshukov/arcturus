//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __GRAMMAR_SYMBOL_H__
#define __GRAMMAR_SYMBOL_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class symbol
{
    public:
        using symbol_type = std::shared_ptr<symbol>;
        using symbols_type = std::vector<symbol_type>;

        struct symbol_key_comparator
        {
            bool operator() (const symbol_type& lhs, const symbol_type& rhs) const
            {
                return (*lhs).id() < (*rhs).id();
            }
        };

        using set_type = std::vector<symbol_type>;
        using sets_type = std::vector<set_type>;

        enum class kind
        {
            nonterminal,
            terminal
        };

        enum class associativity_type
        {
            left,
            right
        };

        enum class tree_action
        {
            propagate,
            remove
        };

        enum class flag : uint64_t
        {
            clear = 0x0000
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

        static symbol_type              epsilon;
        static symbol_type              eof;
        static symbol_type              op_mark; // operator precedence mark symbol, #

    private:
        std::size_t                     my_id;              // enumerable type, will be cast to specific enum values
        string_type                     my_name;            // name (label) of the symbol

        kind                            my_kind;            // terminal or non-terminal

        flags_type                      my_flags;           // flags

        bool                            my_nullable;        // if A ->* e
        bool                            my_productive;      // if A ->* w (can generate terminal strings), productive nonterminal
        bool                            my_accessible;      // reachable, if [S ->* a(lpha) X b(eta)], any symbol X is accessable

        uint8_t                         my_precedence;      // precedence level of terminal (shift/reduce parsers)
        associativity_type              my_associativity;   // associativity of terminals (shift/reduce parsers)

        bool                            my_erroneous;       // if a non-terminal can be part of a A -> 'error' rule
        bool                            my_synched;         // true if the keyword belongs to the synch set for error recovery,
                                                            // keywords that start statements in SYNCH(A)

        sets_type                       my_first_sets;      // 'first set' is the set of terminals that can start a string that is derivable from this non-terminal
        sets_type                       my_eff_sets;        // 'epsilon-free first set', EFF
        sets_type                       my_follow_sets;     // 'follow set' is a set of terminals which may follow A (be immediately after A) in a valid derivation

        sets_type                       my_la_sets;         // lookahead set

        string_type                     my_semantic_action; // semantic action (C++ code)

    public:
                                        symbol(std::size_t id, const string_type& name, kind type);

                                        symbol(const symbol& other);
                                        symbol(symbol&& other);

                                       ~symbol();

        symbol&                         operator = (const symbol& other);
        symbol&                         operator = (symbol&& other);

        std::size_t                     id() const;
        std::size_t&                    id();

        const string_type&              name() const;

        bool                            terminal() const;
        bool                            nonterminal() const;

        flags_type                      flags() const;
        flags_type&                     flags();

        bool                            nullable() const;
        bool&                           nullable();

        bool                            productive() const;
        bool&                           productive();

        bool                            accessible() const;
        bool&                           accessible();

        uint8_t                         precedence() const;
        uint8_t&                        precedence();
        
        associativity_type              associativity() const;
        associativity_type&             associativity();

        bool                            erroneous() const;
        bool&                           erroneous();

        bool                            synched() const;
        bool&                           synched();

        const sets_type&                first_sets() const;
        sets_type&                      first_sets();

        const sets_type&                eff_sets() const;
        sets_type&                      eff_sets();

        const sets_type&                follow_sets() const;
        sets_type&                      follow_sets();

        const sets_type&                la_sets() const;
        sets_type&                      la_sets();

        const string_type&              semantic_action() const;
        string_type&                    semantic_action();
};

inline std::size_t symbol::id() const
{
    return my_id;
}

inline std::size_t& symbol::id()
{
    return my_id;
}

inline const string_type& symbol::name() const
{
    return my_name;
}

inline bool symbol::terminal() const
{
    return my_kind == kind::terminal;
}

inline bool symbol::nonterminal() const
{
    return my_kind == kind::nonterminal;
}

inline typename symbol::flags_type symbol::flags() const
{
    return my_flags;
}

inline typename symbol::flags_type& symbol::flags()
{
    return my_flags;
}

inline bool symbol::nullable() const
{
    return my_nullable;
}

inline bool& symbol::nullable()
{
    return my_nullable;
}

inline bool symbol::productive() const
{
    return my_productive;
}

inline bool& symbol::productive()
{
    return my_productive;
}

inline bool symbol::accessible() const
{
    return my_accessible;
}

inline bool& symbol::accessible()
{
    return my_accessible;
}

inline uint8_t symbol::precedence() const
{
    return my_precedence;
}

inline uint8_t& symbol::precedence()
{
    return my_precedence;
}
        
inline symbol::associativity_type symbol::associativity() const
{
    return my_associativity;
}

inline symbol::associativity_type& symbol::associativity()
{
    return my_associativity;
}

inline bool symbol::erroneous() const
{
    return my_erroneous;
}

inline bool& symbol::erroneous()
{
    return my_erroneous;
}

inline bool symbol::synched() const
{
    return my_synched;
}

inline bool& symbol::synched()
{
    return my_synched;
}

inline const symbol::sets_type& symbol::first_sets() const
{
    return my_first_sets;
}

inline symbol::sets_type& symbol::first_sets()
{
    return my_first_sets;
}

inline const symbol::sets_type& symbol::eff_sets() const
{
    return my_eff_sets;
}

inline symbol::sets_type& symbol::eff_sets()
{
    return my_eff_sets;
}

inline const symbol::sets_type& symbol::follow_sets() const
{
    return my_follow_sets;
}

inline symbol::sets_type& symbol::follow_sets()
{
    return my_follow_sets;
}

inline const symbol::sets_type& symbol::la_sets() const
{
    return my_la_sets;
}

inline symbol::sets_type& symbol::la_sets()
{
    return my_la_sets;
}

inline const string_type& symbol::semantic_action() const
{
    return my_semantic_action;
}

inline string_type& symbol::semantic_action()
{
    return const_cast<string_type&>(static_cast<const symbol&>(*this).semantic_action());
}

END_NAMESPACE

#endif // __GRAMMAR_SYMBOL_H__
