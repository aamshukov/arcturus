//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_CONTROLLER_H__
#define __ARCTURUS_CONTROLLER_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)
USINGNAMESPACE(orchestration)

class arcturus_controller : public controller<token<arcturus_token_traits>>
{
    public:
        using token_type = token<arcturus_token_traits>;

        using parser_type = controller<token<arcturus_token_traits>>::parser_type;

        using ir_type = controller<token<arcturus_token_traits>>::ir_type;

        using pass_type = controller<token<arcturus_token_traits>>::pass_type;
        using passes_type = controller<token<arcturus_token_traits>>::passes_type;

        using codegen_type = controller<token<arcturus_token_traits>>::codegen_type;

    private:
        void        initialize() override;
        void        parse() override;
        void        converge() override;
        void        optimize() override;
        void        codegen() override;
        void        finalize() override;

    public:
                    arcturus_controller(const parser_type& parser,
                                        const ir_type& ir,
                                        const passes_type& passes,
                                        const codegen_type& codegen);
        virtual    ~arcturus_controller();

        void        compile() override;
};

END_NAMESPACE

#endif // __ARCTURUS_CONTROLLER_H__
