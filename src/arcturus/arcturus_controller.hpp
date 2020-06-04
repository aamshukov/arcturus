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

    //    using lexical_analyzer_type = std::shared_ptr<lexical_analyzer<token_type>>;
    //    using parser_type = std::shared_ptr<parser<token_type>>;

    //    using ir_type = std::shared_ptr<ir<token_type>>;

    //    //using optimization_type = std::shared_ptr<optimization<token_type>>;
    //    //using codegen_type = std::shared_ptr<codegen<token_type>>;



    //private:
    //    //lexical_analyzer_type   my_lexer;
    //    //parser_type             my_parser;
    //    //ir_type                 my_ir;

    //    //optimization_type       my_optimization;
    //    //codegen_type            my_codegen;

                    arcturus_controller() {} //??

    public:
        virtual void initialize() override {}
        virtual void parse() override {}
        virtual void converge() override {}
        virtual void optimize() override {}
        virtual void codegen() override {}
        virtual void finalize() override {}
};

END_NAMESPACE

#endif // __ARCTURUS_CONTROLLER_H__
