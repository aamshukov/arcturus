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

class arcturus_controller : public controller<arcturus_token, arcturus_operation_code_traits>
{
    public:
        using token_type = arcturus_token;
        using traits_type = arcturus_operation_code_traits;

        using controller_base = controller<arcturus_token, arcturus_operation_code_traits>;

        using parser_type = controller_base::parser_type;

        using ir_type = controller_base::ir_type;

        using context_type = controller_base::context_type;

        using pass_type = controller_base::pass_type;
        using passes_type = controller_base::passes_type;

        using codegen_type = controller_base::codegen_type;

    private:

    private:
        void        initialize(const context_type& context) override;
        void        parse(const context_type& context) override;
        void        converge(const context_type& context) override;
        void        optimize(const context_type& context) override;
        void        codegen(const context_type& context) override;
        void        finalize(const context_type& context) override;

    public:
                    arcturus_controller(const parser_type& parser,
                                        const ir_type& ir,
                                        const passes_type& passes,
                                        const codegen_type& codegen,
                                        const context_type& context);
        virtual    ~arcturus_controller();

        void        compile() override;
};

END_NAMESPACE

#endif // __ARCTURUS_CONTROLLER_H__
