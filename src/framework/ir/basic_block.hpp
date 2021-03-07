//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

#pragma once

BEGIN_NAMESPACE(backend)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Instruction>
class basic_block : public dominator_vertex
{
    public:
        using instruction_type = Instruction;
        using code_type = code<instruction_type>;

        using symbol_type = typename instruction_type::symbol_type;
        using symbols_type = std::set<symbol_type, symtable::symbol::symbol_key_comparator>;

    private:
        code_type           my_code;

        symbols_type        my_ins;
        symbols_type        my_outs;

        symbols_type        my_gens;
        symbols_type        my_kills;

    public:
                            basic_block(const id_type& id, const string_type& name);
                           ~basic_block();

        const code_type&    code() const;
        code_type&          code();

        const symbols_type& ins() const;
        symbols_type&       ins();
        
        const symbols_type& outs() const;
        symbols_type&       outs();

        const symbols_type& gens() const;
        symbols_type&       gens();

        const symbols_type& kills() const;
        symbols_type&       kills();
};

template <typename Instruction>
inline const typename basic_block<Instruction>::code_type& basic_block<Instruction>::code() const
{
    return my_code;
}

template <typename Instruction>
inline typename basic_block<Instruction>::code_type& basic_block<Instruction>::code()
{
    return my_code;
}

template <typename Instruction>
inline const typename basic_block<Instruction>::symbols_type& basic_block<Instruction>::ins() const
{
    return my_ins;
}

template <typename Instruction>
inline typename basic_block<Instruction>::symbols_type& basic_block<Instruction>::ins()
{
    return my_ins;
}

template <typename Instruction>
inline const typename basic_block<Instruction>::symbols_type& basic_block<Instruction>::outs() const
{
    return my_outs;
}

template <typename Instruction>
inline typename basic_block<Instruction>::symbols_type& basic_block<Instruction>::outs()
{
    return my_outs;
}

template <typename Instruction>
inline const typename basic_block<Instruction>::symbols_type& basic_block<Instruction>::gens() const
{
    return my_gens;
}

template <typename Instruction>
inline typename basic_block<Instruction>::symbols_type& basic_block<Instruction>::gens()
{
    return my_gens;
}

template <typename Instruction>
inline const typename basic_block<Instruction>::symbols_type& basic_block<Instruction>::kills() const
{
    return my_kills;
}

template <typename Instruction>
inline typename basic_block<Instruction>::symbols_type& basic_block<Instruction>::kills()
{
    return my_kills;
}

END_NAMESPACE

#endif // __BASIC_BLOCK_H__
