//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __DIAGNOSTICS_H__
#define __DIAGNOSTICS_H__

#pragma once

BEGIN_NAMESPACE(core)

class diagnostics : public singleton<diagnostics>
{
    public:
        using data_type = std::vector<status>;
        using size_type = std::size_t;

    private:
        data_type           my_data;
        size_type           my_spurious_errors; // how many spurious error before termination
        bool                my_state; // quick state check, true - valid (continue), false - erroneous

    public:
                            diagnostics(size_type spurious_errors);

        const data_type&    warnings() const;
        const data_type&    errors() const;

        const data_type&    data() const;

        bool                state() const;

        // add diagnostic
        // dump diagnostics
        // add status
};

inline diagnostics::diagnostics(typename diagnostics::size_type spurious_errors = 100)
                  : my_spurious_errors(spurious_errors), my_state(true)
{
}

inline const typename diagnostics::data_type& diagnostics::warnings() const
{
    data_type result;
    return result;
}

inline const typename diagnostics::data_type& diagnostics::errors() const
{
    data_type result;
    return result;
}

inline const typename diagnostics::data_type& diagnostics::data() const
{
    return my_data;
}

inline bool diagnostics::state() const
{
    return my_state;
}

END_NAMESPACE

#endif // __DIAGNOSTICS_H__
