//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#pragma once

BEGIN_NAMESPACE(orchestration)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)

template <typename Token, typename OpCodeTraits>
class controller : private noncopyable
{
    public:
        using token_type = Token;
        using traits_type = OpCodeTraits;

        using parser_type = std::shared_ptr<parser<token_type>>;

        using ir_type = std::shared_ptr<ir<token_type, traits_type>>;

        using context_type = std::shared_ptr<context>;

        using pass_type = std::shared_ptr<pass>;
        using passes_type = std::vector<pass_type>;

        using codegen_type = std::shared_ptr<codegen>;

    protected:
        parser_type             my_parser;
        ir_type                 my_ir;
        passes_type             my_passes; // optimization
        codegen_type            my_codegen;
        context_type            my_context;

    protected:
        virtual void            initialize(const context_type& context) = 0;
        virtual void            parse(const context_type& context) = 0;
        virtual void            converge(const context_type& context) = 0;
        virtual void            optimize(const context_type& context) = 0;
        virtual void            codegen(const context_type& context) = 0;
        virtual void            finalize(const context_type& context) = 0;

        virtual void            compile() = 0;

    public:
                                controller(const parser_type& parser,
                                           const ir_type& ir,
                                           const passes_type& passes,
                                           const codegen_type& codegen,
                                           const context_type& context);
        virtual                ~controller();
};

template <typename Token, typename OpCodeTraits>
controller<Token, OpCodeTraits>::controller(const parser_type& parser,
                                      const ir_type& ir,
                                      const passes_type& passes,
                                      const codegen_type& codegen,
                                      const context_type& context)
                 : my_parser(parser),
                   my_ir(ir),
                   my_passes(passes),
                   my_codegen(codegen),
                   my_context(context)
{
}                 

template <typename Token, typename OpCodeTraits>
controller<Token, OpCodeTraits>::~controller()
{
}

END_NAMESPACE

#endif // __CONTROLLER_H__
