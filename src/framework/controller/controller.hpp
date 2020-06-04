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

        using lexical_analyzer_type = std::shared_ptr<lexical_analyzer<token_type>>;
        using parser_type = std::shared_ptr<parser<token_type>>;

        //using ir_type = std::shared_ptr<ir<token_type>>;

        //using optimization_type = std::shared_ptr<optimization<token_type>>;
        //using codegen_type = std::shared_ptr<codegen<token_type>>;

    private:
        //lexical_analyzer_type   my_lexer;
        //parser_type             my_parser;
        //ir_type                 my_ir;

        //optimization_type       my_optimization;
        //codegen_type            my_codegen;
         
    public:
    controller() //??
    {}


    public:
        virtual void initialize() = 0;
        virtual void parse() = 0;
        virtual void converge() = 0;
        virtual void optimize() = 0;
        virtual void codegen() = 0;
        virtual void finalize() = 0;
};

END_NAMESPACE

#endif // __CONTROLLER_H__
