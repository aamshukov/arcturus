//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __FSA_VISUALIZATION_H__
#define __FSA_VISUALIZATION_H__

#pragma once

BEGIN_NAMESPACE(frontend)
USING_NAMESPACE(core)

class fsa_visualization : private noncopyable
{
    public:
        using token_type = typename fsa::token_type;

        using state_type = typename fsa::state_type;
        using states_type = typename fsa::states_type;

        using transition_type = typename fsa::transition_type;
        using transitions_type = typename fsa::transitions_type;

        using fsa_type = typename fsa::fsa_type;

    private:
        static string_type  get_state_label(const typename fsa_visualization::state_type& state);

    public:
        static void         generate_graphviz_file(const fsa_type& fsa, const string_type& file_name);
};

END_NAMESPACE

#endif // __FSA_VISUALIZATION_H__
