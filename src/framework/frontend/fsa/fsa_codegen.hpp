//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FSA_CODEGEN_H__
#define __FSA_CODEGEN_H__

#pragma once

#define MAX_TRANSITION_RANK (5)

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class fsa_codegen : private noncopyable
{
    public:
        using state_type = typename fsa::state_type;
        using states_type = typename fsa::states_type;

        using transition_type = typename fsa::transition_type;
        using transitions_type = typename fsa::transitions_type;

        using fsa_type = typename fsa::fsa_type;

    private:
        static string_type build_predicate(const string_type& predicate);

    public:
        template <typename T>
        static void generate(const fsa_type& fsa,
                             const string_type& file_name,
                             const string_type& label_prefix = empty_string(),
                             bool case_mode = false);
};

template <typename T>
void fsa_codegen::generate(const typename fsa_codegen::fsa_type& fsa, const string_type& file_name, const string_type& label_prefix, bool case_mode)
{
    using token_type = T;

    log_info(L"Generating code to '%s' ...", file_name.c_str());

    try
    {
        const int8_t indent_size = 4;

        const char_type* indent1(get_indent(1 * indent_size));
        const char_type* indent3(get_indent(3 * indent_size));
        const char_type* indent4(get_indent(4 * indent_size));
        const char_type* indent5(get_indent(5 * indent_size));

#ifdef _WINDOWS_
        const char_type* new_line(L"\r\n");
#else
        const char_type* new_line(L"\n");
#endif
        string_type code;

        code += indent3;

        for(const auto& state_kvp : (*fsa).states())
        {
            const auto& state(state_kvp.second);

            string_type token_name(token_type::name(static_cast<typename token_type::traits::type>((*state).token())));

            bool final_state = (*fsa).is_final_state(state);

            std::size_t index = 0;

            bool local_case_mode = case_mode && (*state).transitions().size() > 1;

            code += new_line;
            code += L"_q_" + label_prefix + L"_";
            code += std::to_wstring((*state).id());
            code += L":";

            if(final_state)
            {
                code += new_line;
                code += indent3;
                code += L"// final state - [";
                code += token_name;
                code += L"]";
                code += new_line;
                code += indent3;
                code += L"my_token.type = token_type::traits::type::" + token_name + L";";
            }

            if(!(*state).transitions().empty())
            {
                code += new_line;
                code += indent3;
                code += L"if(my_ptr >= my_end_content)";
                code += new_line;
                code += indent4;
                code += L"goto ";
                code += final_state ? (L"_exit_" + label_prefix) : (L"_eos_" + label_prefix);
                code += L";";
            }

            if(local_case_mode)
            {
                code += new_line;
                code += indent3;
                code += L"switch(codepoint)";
                code += new_line;
                code += indent3;
                code += L"{";
            }

            for(int16_t rank = 0; rank < MAX_TRANSITION_RANK; rank++)
            {
                for(const auto& transition_kvp : (*state).transitions())
                {
                    const auto& transition(transition_kvp.second);

                    if((*transition).rank() == rank)
                    {
                        bool is_default_case = false;

                        code += new_line;
                        code += indent3;

                        if(index++ > 0 && !local_case_mode)
                        {
                            code += L"else ";
                        }

                        if((*transition).predicate().compare(IDENTIFIER_START) == 0)
                        {
                            if(local_case_mode)
                            {
                                code += indent1;
                                code += L"default:";
                                code += new_line;
                                code += indent5;

                                is_default_case = true;
                            }

                            code += L"if(text::is_java_identifier_start(codepoint))";
                        }
                        else if((*transition).predicate().compare(IDENTIFIER_PART) == 0)
                        {
                            if(local_case_mode)
                            {
                                code += indent1;
                                code += L"default:";
                                code += new_line;
                                code += indent5;

                                is_default_case = true;
                            }

                            code += L"if(text::is_java_identifier_part(codepoint) || text::is_identifier_ignorable(codepoint))";
                        }
                        else
                        {
                            if(local_case_mode)
                            {
                                code += indent1;
                                code += L"case ";
                            }
                            else
                            {
                                code += L"if(codepoint == ";
                            }
                            code += build_predicate((*transition).predicate());
                            if(local_case_mode)
                            {
                                code += L":";
                            }
                            else
                            {
                                code += L")";
                            }
                        }

                        code += new_line;
                        code += local_case_mode ? indent4 : indent3;
                        code += is_default_case ? indent1 : empty_string();
                        code += L"{";

                        code += new_line;
                        code += local_case_mode ? indent5 : indent4;
                        code += is_default_case ? indent1 : empty_string();
                        code += L"codepoint = advance();";

                        code += new_line;
                        code += local_case_mode ? indent5 : indent4;
                        code += is_default_case ? indent1 : empty_string();
                        code += L"goto _q_" + label_prefix + L"_";
                        code += std::to_wstring((*transition).end_state());
                        code += L";";

                        code += new_line;
                        code += local_case_mode ? indent4 : indent3;
                        code += is_default_case ? indent1 : empty_string();
                        code += L"}";
                    }
                }
            }

            if(final_state && (*state).transitions().empty())
            {
                code += new_line;
                code += indent3;
                code += L"advance();";
            }

            if(local_case_mode)
            {
                code += new_line;
                code += indent3;
                code += L"}";
            }

            code += new_line;
            code += indent3;
            code += L"goto _exit_" + label_prefix + L";";
        }

        code += new_line;
        code += L"_eos_" + label_prefix + L":";
        code += new_line;
        code += indent3;
        code += L"my_token.type = token_type::traits::type::eos;";
        code += new_line;

        code += L"_exit_" + label_prefix + L":";
        code += new_line;
        code += indent3;
        code += L"my_ptr--;"; //??
        code += new_line;

        UFILE* ufile(u_fopen_u(reinterpret_cast<const UChar*>(file_name.c_str()), "wb", NULL, "UTF-8"));

        if(ufile)
        {
            u_fputs(reinterpret_cast<const UChar*>(code.c_str()), ufile);

            u_fflush(ufile);
            u_fclose(ufile);
        }
        else
        {
            log_error(L"Unable generate FSA code, invlid file name '%s'.", file_name.c_str());
        }
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate code: error occurred.")
    }

    log_info(L"Generated code to '%s'.", file_name.c_str());
}

END_NAMESPACE

#endif // __FSA_CODEGEN_H__
