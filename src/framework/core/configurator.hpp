//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONFIGURATOR_H__
#define __CONFIGURATOR_H__

#pragma once

BEGIN_NAMESPACE(core)

class configurator : public singleton<configurator>
{
    public:
        using kvp_type = std::unordered_map<string_type, string_type>;

    private:
        kvp_type    my_options;
        kvp_type    my_flags;
        kvp_type    my_features;

    public:
                            configurator() = default;

        const kvp_type&     options() const;
        kvp_type&           options();

        const kvp_type&     flags() const;
        kvp_type&           flags();

        const kvp_type&     features() const;
        kvp_type&           features();
};

inline const typename configurator::kvp_type& configurator::options() const
{
    return my_options;
}

inline typename configurator::kvp_type& configurator::options()
{
    return my_options;
}

inline const typename configurator::kvp_type& configurator::flags() const
{
    return my_flags;
}

inline typename configurator::kvp_type& configurator::flags()
{
    return my_flags;
}

inline const typename configurator::kvp_type& configurator::features() const
{
    return my_features;
}

inline typename configurator::kvp_type& configurator::features()
{
    return my_features;
}

END_NAMESPACE

#endif // __CONFIGURATOR_H__
