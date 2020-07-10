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

class arcturus_controller : public controller<arcturus_token>
{
    public:
        using token_type = arcturus_token;

        using parser_type = controller<arcturus_token>::parser_type;

        using ir_type = controller<arcturus_token>::ir_type;

        using context_type = controller<arcturus_token>::context_type;

        using pass_type = controller<arcturus_token>::pass_type;
        using passes_type = controller<arcturus_token>::passes_type;

        using codegen_type = controller<arcturus_token>::codegen_type;

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
