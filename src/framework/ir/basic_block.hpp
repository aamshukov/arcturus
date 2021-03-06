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

    private:
        code_type           my_code;

    public:
                            basic_block(const id_type& id, const string_type& name);
                           ~basic_block();

        const code_type&    code() const;
        code_type&          code();
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

END_NAMESPACE

#endif // __BASIC_BLOCK_H__
