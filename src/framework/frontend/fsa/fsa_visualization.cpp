//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>

#include <frontend/fsa/fsa_transition.hpp>
#include <frontend/fsa/fsa_state.hpp>
#include <frontend/fsa/fsa.hpp>
#include <frontend/fsa/fsa_state_set.hpp>
#include <frontend/fsa/fsa_algorithm.hpp>
#include <frontend/fsa/fsa_visualization.hpp>

BEGIN_NAMESPACE(frontend)
USINGNAMESPACE(core)

void fsa_visualization::generate_graphviz_file(const fsa_type& fsa, const string_type& file_name)
{
    log_info(L"Generating graphviz file ...");

    std::wofstream stream;

    try
    {
        stream.open(file_name.c_str(), std::ios::out | std::ios::binary);

        if(!stream.is_open() || stream.bad() || stream.fail())
        {
            log_error(L"Failed to generate graphviz file: stream is either open or in a bad condition.");
        }

        const char_type* indent(get_indent(4));

        stream << L"digraph FSA" << std::endl;
        stream << L"{" << std::endl;

        if(!(*fsa).final_states().empty())
        {
            stream << indent << "node [shape = doublecircle];" << std::endl;

            stream << indent;

            for(const auto& kvp : (*fsa).final_states())
            {
                const state_type& state(kvp.second);
                stream << get_state_label(state) << L" ";
            }

            stream << ";" << std::endl;
        }

        stream << indent << L"node [shape = circle];" << std::endl;
        stream << indent << L"rankdir = LR;" << std::endl;

        for(const auto& kvp : (*fsa).states())
        {
            const state_type& state(kvp.second);

            if((*state).transitions().empty())
            {
                stream << indent << get_state_label(state) << L";" << std::endl;
            }
            else
            {
                for(const auto& transition : (*state).transitions())
                {
                    const state_type& end_state((*fsa).states().at((*transition.second).end_state()));

                    stream << indent << get_state_label(state) << " -> " << get_state_label(end_state);
                    
                    string_type predicate;

                    if(!(*transition.second).predicate().empty())
                    {
                        predicate = (*transition.second).predicate();
                    }
                    else
                    {
                        if((*transition.second).switch_char() == text::epsilon_codepoint())
                        {
                            predicate = fsa_transition::epsilon_predicate();
                        }
                        else
                        {
                            predicate = static_cast<wchar_t>((*transition.second).switch_char());
                        }
                    }

                    stream << " [ " << "label = \"" << predicate << "\" ];" << std::endl;
                }
            }
        }

        stream << L"}" << std::endl;

        stream.flush();
        stream.clear();
        stream.close();
    }
    catch(const std::exception& ex)
    {
        log_exception(ex, L"Failed to generate graphviz file: error occurred.");
    }

    log_info(L"Generated graphviz file.");

    // D:\Soft\graphviz\2.38\release\bin\dot -Tpng d:\tmp\fsa.dot -o d:\tmp\fsa.png
}

string_type fsa_visualization::get_state_label(const typename fsa_visualization::state_type& state)
{
    return L"\"" + (*state).label() + L"_" + std::to_wstring((*state).id()) + L"_" + std::to_wstring((*state).token()) + L"\"";
}

END_NAMESPACE
