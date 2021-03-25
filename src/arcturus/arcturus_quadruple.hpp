//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __ARCTURUS_QUADRUPLE_H__
#define __ARCTURUS_QUADRUPLE_H__

#pragma once

BEGIN_NAMESPACE(arcturus)

USING_NAMESPACE(core)
USING_NAMESPACE(frontend)
USING_NAMESPACE(symtable)
USING_NAMESPACE(backend)

struct arcturus_operation_code_traits
{
    DECLARE_ENUM
    (
        operation_code,
        std::size_t,
        unknown = 0,

        noop = 5,

        // HIR - high-level intermediate representation (IR), usually AST/CST forest
        hir_size,


        // MIR - mid-level intermediate representation (IR)
        declaration_mir = hir_size + 128,

        assignment_mir,

        unary_op_plus_mir,
        unary_op_negation_mir,
        unary_op_multiply_mir,
        unary_op_divide_mir,
        unary_op_negate_mir,
        unary_op_complement_mir,    // ~

        binary_op_add_mir,
        binary_op_subtract_mir,
        binary_op_multiply_mir,
        binary_op_divide_mir,
        binary_op_equal_mir,
        binary_op_notequal_mir,
        binary_op_lessthan_mir,
        binary_op_biggerthan_mir,
        binary_op_logical_and_mir,  // &&
        binary_op_logical_or_mir,   // ||
        binary_op_and_mir,          // &
        binary_op_inclusive_or_mir, // |
        binary_op_exclusive_or_mir, // ^

        if_true_mir,
        if_false_mir,
        goto_mir,
        label_mir,

        function_mir,               // foo
        function_formal_param_mir,
        function_begin_mir,
        function_return_mir,
        function_end_mir,

        function_argument_mir,
        function_call_mir,

        mir_size,

        // LIR - low-level intermediate representation (IR)
        address_of          = mir_size + 128,

        //load_integer        = 20,
        //store_integer       = 21, // assignment
        //add_integer         = 22,
        //subtract_integer    = 23,
        //multiply_integer    = 24,
        //divide_integer      = 25,
        //left_shift_integer  = 26,
        //right_shift_integer = 27,
        //bitwise_and_integer = 28,
        //bitwise_or_integer  = 29,
        //bitwise_xor_integer = 30,
        //eq_integer          = 31,
        //ne_integer          = 32,
        //lt_integer          = 33,
        //le_integer          = 34,
        //gt_integer          = 35,
        //ge_integer          = 36,
        //negation_integer    = 37, // -

        //load_real           = 50,
        //store_real          = 51, // assignment
        //add_real            = 52,
        //subtract_real       = 53,
        //multiply_real       = 54,
        //divide_real         = 55,
        //eq_real             = 56,
        //ne_real             = 57,
        //lt_real             = 58,
        //le_real             = 59,
        //gt_real             = 60,
        //ge_real             = 61,
        //negation_real       = 62, // - =

        //logical_negation    = 80, // !
        //bitwise_negation    = 81, // ~

        //array_access        = 100, // []

        //// subroutine, procedure, function ... definition
        //procedure_begin     = 200,
        //procedure_end       = 201,
        //parameter           = 202,
        //return_statement    = 203,

        //// subroutine, procedure, function ... call (callsite)
        //procedure_call      = 204,
        //argument            = 205,

        //label               = 300,
        //goto_statement      = 301,

        // SSA
        phi                 = 5000, // 𝛗
        //phi_param,                  // 𝛗-param

        sentinel            = 10000
    )
};

struct arcturus_quadruple : public quadruple<arcturus_symbol, arcturus_operation_code_traits>
{
    using quadruple_base = quadruple<arcturus_symbol, arcturus_operation_code_traits>;

    using argument_type = typename quadruple_base::argument_type;
    using phi_params_type = typename quadruple_base::phi_params_type;

    using id_type = typename quadruple_base::id_type;
    using flags_type = typename quadruple_base::flags_type;

    arcturus_quadruple(const id_type& id)
        : quadruple_base(id)
    {
    }

    arcturus_quadruple(const id_type& id,
                       const operation_code& operation)
        : quadruple_base(id, operation)
    {
    }

    arcturus_quadruple(const id_type& id,
                       const operation_code& operation,
                       const result_type& result)
        : quadruple_base(id, operation, result)
    {
    }

    arcturus_quadruple(const id_type& id,
                       const operation_code& operation,
                       const argument_type& argument1,
                       const result_type& result)
        : quadruple_base(id, operation, argument1, result)
    {
    }

    arcturus_quadruple(const id_type& id,
                       const operation_code& operation,
                       const argument_type& argument1,
                       const argument_type& argument2,
                       const result_type& result)
        : quadruple_base(id, operation, argument1, argument2, result)
    {
    }

    static bool is_assignment(const typename arcturus_operation_code_traits::operation_code& opcode)
    {
        return opcode == arcturus_operation_code_traits::operation_code::assignment_mir;
    }

    static bool is_x_y_op_z(const typename arcturus_operation_code_traits::operation_code& opcode)
    {
        return opcode == arcturus_operation_code_traits::operation_code::assignment_mir ||
               opcode == arcturus_operation_code_traits::operation_code::unary_op_plus_mir ||
               opcode == arcturus_operation_code_traits::operation_code::unary_op_negation_mir ||
               opcode == arcturus_operation_code_traits::operation_code::unary_op_multiply_mir ||
               opcode == arcturus_operation_code_traits::operation_code::unary_op_divide_mir ||
               opcode == arcturus_operation_code_traits::operation_code::unary_op_negate_mir ||
               opcode == arcturus_operation_code_traits::operation_code::unary_op_complement_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_add_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_subtract_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_multiply_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_divide_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_equal_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_notequal_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_lessthan_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_biggerthan_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_logical_and_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_logical_or_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_and_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_inclusive_or_mir ||
               opcode == arcturus_operation_code_traits::operation_code::binary_op_exclusive_or_mir;
    }

    static string_type opcode_name(const typename arcturus_operation_code_traits::operation_code& opcode)
    {
        if(arcturus_operation_code_traits::mapping.empty())
        {
            arcturus_operation_code_traits::initialize();
        }

        return arcturus_operation_code_traits::name(opcode);
    }

    string_type to_string() override
    {
        string_type text;

        // operator
        string_type op;

        switch(operation)
        {
            case arcturus_operation_code_traits::operation_code::noop:
                op = L"noop";
                break;
            case arcturus_operation_code_traits::operation_code::assignment_mir:
                op = L"=";
                break;
            case arcturus_operation_code_traits::operation_code::unary_op_plus_mir:
                op = L"+";
                break;
            case arcturus_operation_code_traits::operation_code::unary_op_negation_mir:
                op = L"-";
                break;
            case arcturus_operation_code_traits::operation_code::unary_op_multiply_mir:
                op = L"*";
                break;
            case arcturus_operation_code_traits::operation_code::unary_op_divide_mir:
                op = L"/";
                break;
            case arcturus_operation_code_traits::operation_code::unary_op_negate_mir:
                op = L"!";
                break;
            case arcturus_operation_code_traits::operation_code::unary_op_complement_mir:
                op = L"~";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_add_mir:
                op = L"+";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_subtract_mir:
                op = L"-";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_multiply_mir:
                op = L"*";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_divide_mir:
                op = L"/";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_equal_mir:
                op = L"==";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_notequal_mir:
                op = L"!=";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_lessthan_mir:
                op = L"<";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_biggerthan_mir:
                op = L">";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_logical_and_mir:
                op = L"&&";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_logical_or_mir:
                op = L"||";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_and_mir:
                op = L"&";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_inclusive_or_mir:
                op = L"|";
                break;
            case arcturus_operation_code_traits::operation_code::binary_op_exclusive_or_mir:
                op = L"^";
                break;
            case arcturus_operation_code_traits::operation_code::if_true_mir:
                op = L"if-true";
                break;
            case arcturus_operation_code_traits::operation_code::if_false_mir:
                op = L"if-false";
                break;
            case arcturus_operation_code_traits::operation_code::goto_mir:
                op = L"goto";
                break;
            case arcturus_operation_code_traits::operation_code::label_mir:
                op = L"label";
                break;
            case arcturus_operation_code_traits::operation_code::declaration_mir:
                op = L"decl";
                break;
            case arcturus_operation_code_traits::operation_code::function_return_mir:
                op = L"return";
                break;
            case arcturus_operation_code_traits::operation_code::function_argument_mir:
                op = L"arg";
                break;
            case arcturus_operation_code_traits::operation_code::function_call_mir:
                op = L"call";
                break;
            case arcturus_operation_code_traits::operation_code::phi:
                op = L"phi";
                break;
        }

        if(operation == arcturus_operation_code_traits::operation_code::phi)
        {
            // Vk <- 𝛗(V0, V1, ... , Vm)
            const auto& symbol(*argument1.first);
            auto symbol_name(symbol.to_string());

            text = symbol_name + L" - phi(";

            const auto& params(std::get<2>(result));

            for(const auto& param : params)
            {
                symbol_name = ((*param.first.first).to_string());
                text += symbol_name + L", ";
            }

            text = text.substr(0, text.size() - 2);

            text += L")";
        }
        else
        {
            // argument1
            string_type arg1;

            if(argument1.first != nullptr)
            {
                arg1 = (*argument1.first).to_string();
            }

            // argument2
            string_type arg2;

            if(argument2.first != nullptr)
            {
                arg2 = (*argument2.first).to_string();
            }

            // result
            string_type res;

            if(std::holds_alternative<argument_type>(result))
            {
                const auto& symbol_res(std::get<0>(result));

                if(symbol_res.first != nullptr)
                {
                    res = (*symbol_res.first).to_string();
                }
            }
            else if(std::holds_alternative<quadruple_type>(result))
            {
                const auto& result_quadruple(std::get<1>(result));

                if(result_quadruple != nullptr)
                {
                    res = std::to_wstring((*result_quadruple).id);
                    //res = L"(" + (*result_quadruple).to_string() + L")";
                }
            }

            text = format(L"%-4d%-8s%+18s%+18s%+18s", id, op.c_str(), arg1.c_str(), arg2.c_str(), res.c_str());
        }

        return text;
    }
};

END_NAMESPACE

#endif // __ARCTURUS_QUADRUPLE_H__
