//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __CONFIGURATOR_H__
#define __CONFIGURATOR_H__

#pragma once

BEGIN_NAMESPACE(core)

class configurator : private noncopyable
{
    public:
        using datum_type = text::datum_type;
        using data_type = std::shared_ptr<datum_type[]>;

        using text_type = std::basic_string<datum_type>;

        using options_type = std::unordered_multimap<string_type, string_type>;
        using flags_type = std::unordered_multimap<string_type, bool>;

                                                    //                                 no spaces
    protected:                                      //                                      |
        options_type            my_options;         // option with argument: -c red  --color=red  --color red
        flags_type              my_flags;           // option (switch) without argument (boolean): --debug  -d 

        text_type               my_application;

        options_type            my_master_options;  // compare against
        flags_type              my_master_flags;    // compare against

    private:
        void                    populate();

        virtual void            populate_options() = 0;
        virtual void            populate_flags() = 0;

    public:
                                configurator() = default;

        const options_type&     options() const;
        options_type&           options();

        const flags_type&       flags() const;
        flags_type&             flags();

        bool                    configure(int argc, char_type *argv[]);
};

inline const typename configurator::options_type& configurator::options() const
{
    return my_options;
}

inline typename configurator::options_type& configurator::options()
{
    return my_options;
}

inline const typename configurator::flags_type& configurator::flags() const
{
    return my_flags;
}

inline typename configurator::flags_type& configurator::flags()
{
    return my_flags;
}

END_NAMESPACE

#endif // __CONFIGURATOR_H__
