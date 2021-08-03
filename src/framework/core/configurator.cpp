//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
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
        std::size_t count = 0;

        std::shared_ptr<cp_type[]> codepoints;

        text::string_to_codepoints(argv[0], codepoints, count);

        std::basic_string<cp_type> application(codepoints.get());

        my_application.swap(application);

        for(auto k = 1; k < argc; k++)
        {
            const char_type *p = argv[k];

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

                if(auto it_opt = my_master_options.find(option); it_opt != my_master_options.end())
                {
                    auto multi_value = std::get<1>((*it_opt).second);
                    auto validate = std::get<2>((*it_opt).second);

                    auto opt_exist = my_options.find(option) != my_options.end();
                    auto opt_add = true;

                    if(!multi_value && opt_exist)
                    {
                        opt_add = false;

                        OPERATION_FAILED(status::custom_code::error,
                                         0,
                                         status::contributor::core,
                                         L"Invalid command line: the option '%s'='%s' already specified.", option.c_str(), argument.c_str())
                    }
                    else if(validate)
                    {
                        opt_add = false;

                        if(!argument.empty())
                        {
                            // check if argument is valid
                            for(auto it_arg = my_master_options.find(option); it_arg != my_master_options.end(); ++it_arg) 
                            {
                                if(std::get<0>((*it_arg).second) == argument)
                                {
                                    opt_add = true;
                                    goto __found_arg;
                                }
                            }
                        }

                        OPERATION_FAILED(status::custom_code::error,
                                         0,
                                         status::contributor::core,
                                         L"Invalid command line: invalid argument for the '%s' option.", option.c_str())
__found_arg:
                        ;
                    }

                    if(opt_add)
                    {
                        my_options.insert(std::pair(option, argument));
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
                                         L"Invalid command line: the flag '%s' already specified.", option.c_str())
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

bool configurator::add_envvar(const char_type* envar_name)
{
    bool result = false;

    size_t envar_name_len = 0;

    auto envar_value = std::make_unique<char_type[]>(configurator::kEnvVarMaxLength);

    _wgetenv_s(&envar_name_len, const_cast<wchar_t*>(envar_name), configurator::kEnvVarMaxLength, envar_value.get());

    if(envar_value != nullptr)
    {
        string_type option(envar_name);
        string_type argument(envar_value.get());

        my_envars.insert(std::pair(option, argument));

        result = true;
    }

    return result;
}

END_NAMESPACE
