//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#pragma once

BEGIN_NAMESPACE(orchestration)

USINGNAMESPACE(core)
USINGNAMESPACE(symtable)
USINGNAMESPACE(frontend)
USINGNAMESPACE(backend)

template <typename Token>
class controller : private noncopyable
{
    public:
        using token_type = Token;

        using parser_type = std::shared_ptr<parser<token_type>>;

        using ir_type = std::shared_ptr<ir<token_type>>;

        using pass_type = std::shared_ptr<pass>;
        using passes_type = std::vector<pass_type>;

        using codegen_type = std::shared_ptr<codegen>;

    private:
        parser_type             my_parser;
        ir_type                 my_ir;
        passes_type             my_passes; // optimization
        codegen_type            my_codegen;

    protected:
        virtual void            initialize() = 0;
        virtual void            parse() = 0;
        virtual void            converge() = 0;
        virtual void            optimize() = 0;
        virtual void            codegen() = 0;
        virtual void            finalize() = 0;

        virtual void            compile() = 0;

    public:
                                controller(const parser_type& parser,
                                           const ir_type& ir,
                                           const passes_type& passes,
                                           const codegen_type& codegen);
        virtual                ~controller();
};

template <typename Token>
controller<Token>::controller(const parser_type& parser,
                              const ir_type& ir,
                              const passes_type& passes,
                              const codegen_type& codegen)
                 : my_parser(parser), my_ir(ir), my_passes(passes), my_codegen(codegen)
{
}                 

template <typename Token>
controller<Token>::~controller()
{
}

END_NAMESPACE

#endif // __CONTROLLER_H__
