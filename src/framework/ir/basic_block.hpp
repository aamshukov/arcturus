//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __BASIC_BLOCK_H__
#define __BASIC_BLOCK_H__

#pragma once

BEGIN_NAMESPACE(backend)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)

template <typename TInstruction>
class basic_block : public dominator_vertex
{
    public:
        using instruction_type = TInstruction;
        using code_type = code<instruction_type>;

        using symbol_type = typename instruction_type::symbol_type;
        using symbols_type = std::set<symbol_type, symtable::symbol::symbol_key_comparator>;

    private:
        code_type           my_code;

        symbols_type        my_ins;
        symbols_type        my_outs;

        symbols_type        my_defs;
        symbols_type        my_uses;

    public:
                            basic_block(const id_type& id, const string_type& name);
                           ~basic_block();

        const code_type&    code() const;
        code_type&          code();

        const symbols_type& ins() const;
        symbols_type&       ins();
        
        const symbols_type& outs() const;
        symbols_type&       outs();

        const symbols_type& defs() const;
        symbols_type&       defs();

        const symbols_type& uses() const;
        symbols_type&       uses();
};

template <typename TInstruction>
basic_block<TInstruction>::basic_block(const typename basic_block<TInstruction>::id_type& id, const string_type& label)
                         : dominator_vertex(id, label)
{
}

template <typename TInstruction>
basic_block<TInstruction>::~basic_block()
{
}

template <typename TInstruction>
inline const typename basic_block<TInstruction>::code_type& basic_block<TInstruction>::code() const
{
    return my_code;
}

template <typename TInstruction>
inline typename basic_block<TInstruction>::code_type& basic_block<TInstruction>::code()
{
    return my_code;
}

template <typename TInstruction>
inline const typename basic_block<TInstruction>::symbols_type& basic_block<TInstruction>::ins() const
{
    return my_ins;
}

template <typename TInstruction>
inline typename basic_block<TInstruction>::symbols_type& basic_block<TInstruction>::ins()
{
    return my_ins;
}

template <typename TInstruction>
inline const typename basic_block<TInstruction>::symbols_type& basic_block<TInstruction>::outs() const
{
    return my_outs;
}

template <typename TInstruction>
inline typename basic_block<TInstruction>::symbols_type& basic_block<TInstruction>::outs()
{
    return my_outs;
}

template <typename TInstruction>
inline const typename basic_block<TInstruction>::symbols_type& basic_block<TInstruction>::defs() const
{
    return my_defs;
}

template <typename TInstruction>
inline typename basic_block<TInstruction>::symbols_type& basic_block<TInstruction>::defs()
{
    return my_defs;
}

template <typename TInstruction>
inline const typename basic_block<TInstruction>::symbols_type& basic_block<TInstruction>::uses() const
{
    return my_uses;
}

template <typename TInstruction>
inline typename basic_block<TInstruction>::symbols_type& basic_block<TInstruction>::uses()
{
    return my_uses;
}

END_NAMESPACE

#endif // __BASIC_BLOCK_H__
