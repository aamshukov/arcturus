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
        using status_type = status;

        using data_type = std::vector<status_type>;
        using size_type = std::size_t;

    private:
        data_type           my_data;
        bool                my_state; // quick state check, true - valid (continue), false - erroneous

        size_type           my_spurious_errors; // how many spurious error before termination

    public:
                            diagnostics();

        const data_type     warnings() const;
        const data_type     errors() const;

        const data_type&    data() const;

        const status_type   last_status() const;

        bool                state() const;
        bool&               state();

        size_type           spurious_errors() const;
        size_type&          spurious_errors();

        void                add(status_type&& status);
};

inline diagnostics::diagnostics()
                  : my_spurious_errors(100), my_state(true)
{
}

inline const typename diagnostics::data_type diagnostics::warnings() const
{
    data_type result;

    std::for_each(my_data.begin(),
                  my_data.end(),
                  [&result](const auto& status)
                  {
                      if(status.custom_code() == status::custom_code::warning)
                      {
                          result.emplace_back(status);
                      }
                  });

    return result;
}

inline const typename diagnostics::data_type diagnostics::errors() const
{
    data_type result;

    std::for_each(my_data.begin(),
                  my_data.end(),
                  [&result](const auto& status)
                  {
                      if(status.custom_code() == status::custom_code::error)
                      {
                          result.emplace_back(status);
                      }
                  });

    return result;
}

inline const typename diagnostics::data_type& diagnostics::data() const
{
    return my_data;
}

inline const typename diagnostics::status_type diagnostics::last_status() const
{
    if(!my_data.empty())
    {
        return my_data[my_data.size() - 1];
    }
    else
    {
        return status();
    }
}

inline bool diagnostics::state() const
{
    return my_state;
}

inline bool& diagnostics::state()
{
    return my_state;
}

inline typename diagnostics::size_type diagnostics::spurious_errors() const
{
    return my_spurious_errors;
}

inline typename diagnostics::size_type& diagnostics::spurious_errors()
{
    return my_spurious_errors;
}

inline void typename diagnostics::add(typename diagnostics::status_type&& status)
{
    if(my_data.size() < my_spurious_errors)
    {
        my_data.emplace_back(std::move(status));
    }
}

END_NAMESPACE

#endif // __DIAGNOSTICS_H__
