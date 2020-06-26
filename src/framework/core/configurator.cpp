//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/noncopyable.hpp>

#include <core/domain_helper.hpp>

#include <core/factory.hpp>
#include <core/singleton.hpp>

#include <core/status.hpp>

#include <core/diagnostics.hpp>
#include <core/statistics.hpp>

#include <core/unicode.hpp>
#include <core/text.hpp>

#include <core/configurator.hpp>

BEGIN_NAMESPACE(core)

void configurator::populate()
{
    populate_options();
    populate_flags();
}

bool configurator::configure(int argc, char_type *argv[])
{
    bool result = true;

    populate();

    if(argc > 0)
    {
        size_type count = 0;

        std::shared_ptr<datum_type[]> codepoints;

        text::string_to_codepoints(argv[0], codepoints, count);

        std::basic_string<datum_type> application(codepoints.get());

        my_application.swap(application);

        for(auto k = 1; k < argc; k++)
        {
            const char_type* p = argv[k];

            if(*p++ == '-')
            {
                if(*p == '-')
                {
                    // --color=red  --color red  --debug
                    p++;
                }
                else
                {
                    // -c red -d
                }

                string_type option(p);
                string_type argument;

                std::vector<string_type> parts;

                text::split(option, L"=", parts);

                option.swap(parts[0]);

                if(parts.size() > 1)
                {
                    // color=red
                    argument = parts[1];
                }
                else
                {
                    if(my_master_flags.find(option) == my_master_flags.end())
                    {
                        // color red
                        if(k + 1 < argc)
                        {
                            argument = argv[k + 1];
                            k++;
                        }
                    }
                }

                if(my_master_options.find(option) != my_master_options.end())
                {
                    if(my_options.find(option) == my_options.end())
                    {
                        if(!argument.empty())
                        {
                            my_options.insert(std::pair(option, argument));
                        }
                        else
                        {
                            OPERATION_FAILED(status::custom_code::error,
                                             0,
                                             status::contributor::core,
                                             L"Invalid command line: missing argument for the '%s' option.", option.c_str())
                        }
                    }
                    else
                    {
                        OPERATION_FAILED(status::custom_code::error,
                                         0,
                                         status::contributor::core,
                                         L"Invalid command line: the option '%s'='%s' is already specified.", option.c_str(), argument.c_str())
                    }
                }
                else if(my_master_flags.find(option) != my_master_flags.end())
                {
                    if(my_flags.find(option) == my_flags.end())
                    {
                        my_flags.insert(std::pair(option, true));
                    }
                    else
                    {
                        OPERATION_FAILED(status::custom_code::error,
                                         0,
                                         status::contributor::core,
                                         L"Invalid command line: the flag '%s' is already specified.", option.c_str())
                    }
                }
                else
                {
                    OPERATION_FAILED(status::custom_code::error,
                                        0,
                                        status::contributor::core,
                                        L"Invalid command line: unknown option '%s'.", option.c_str())
                }
            }
        }
    }

    return result;
}

END_NAMESPACE
