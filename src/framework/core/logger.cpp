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

#include <core/logger.hpp>

BEGIN_NAMESPACE(core)

logger::logger()
      : my_severity(severity::debug)
{
}

logger::~logger()
{
    try
    {
        uninitialize();
    }
    catch(const std::exception&)
    {
    }
}

bool logger::initialize(const string_type& file_name)
{
    bool result = false;

    if(my_stream.is_open())
    {
        my_stream.flush();
        my_stream.close();
    }

    try
    {
        my_stream.open(file_name.c_str(), std::ios::out | std::ios::app);

        if(!my_stream.is_open() || my_stream.bad() || my_stream.fail())
        {
            OPERATION_FAILED(status::custom_code::error,
                             0,
                             status::contributer::core,
                             L"Failed to initialize logger: stream is either open or in a bad condition.")
            log_error(diagnostics::instance().last_status().text().c_str());
        }

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::core,
                            L"Failed to initialize logger: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

bool logger::uninitialize()
{
    bool result = false;

    try
    {
        if(my_stream.is_open())
        {
            my_stream.flush();
            my_stream.close();
        }

        result = true;
    }
    catch(const std::exception& ex)
    {
        OPERATION_FAILED_EX(ex,
                            status::custom_code::error,
                            status::contributer::core,
                            L"Failed to uninitialize logger: error occurred.")
        log_exception(ex, diagnostics::instance().last_status().text().c_str());
    }

    return result;
}

void logger::format_time(char_type* buffer, uint16_t buffer_size)
{
    time_t current_time;

    time(&current_time);

    tm new_time;

    localtime_s(&new_time, &current_time);

    snwprintf(buffer,
              buffer_size,
              TRUNCATE,
              L"%02d%02d%04d %02d:%02d:%02d",
              new_time.tm_mday,
              new_time.tm_mon + 1,
              new_time.tm_year + 1900,
              new_time.tm_hour,
              new_time.tm_min,
              new_time.tm_sec);
}

void logger::set_severity(logger::severity level)
{
    my_severity = level;
}

void logger::set_severity(const string_type& level)
{
    if(level == L"DEBUG" || level == L"debug")
    {
        set_severity(logger::severity::debug);
    }
    else if(level == L"INFO" || level == L"info")
    {
        set_severity(logger::severity::info);
    }
    else if(level == L"WARNING" || level == L"warning")
    {
        set_severity(logger::severity::warning);
    }
    else if(level == L"ERROR" || level == L"error")
    {
        set_severity(logger::severity::error);
    }
    else if(level == L"CRITICAL" || level == L"critical")
    {
        set_severity(logger::severity::critical);
    }
    else // default
    {
        set_severity(DEFAULT_LOG_SEVERITY_LEVEL);
    }
}

void logger::log(const char_type* file_name, uint32_t line, const char_type* function_name, severity level, const char_type* format_template, ...)
{
    // [filename(line):function]:[datetime]:[level] message
    // datetime - ISO(day month year) hh:mm:ss.ms
    if(my_stream.is_open() && !(my_stream.bad() || my_stream.fail()))
    {
        if(level <= my_severity)
        {
            char_type time_buffer[32];

            format_time(time_buffer, static_cast<uint16_t>(array_size(time_buffer)));

            const char_type* severity_name;

            switch(level)
            {
                case logger::severity::debug:
                    severity_name = L"DEBUG";
                    break;
                case logger::severity::info:
                    severity_name = L"INFO";
                    break;
                case logger::severity::warning:
                    severity_name = L"WARNING";
                    break;
                case logger::severity::error:
                    severity_name = L"ERROR";
                    break;
                case logger::severity::critical:
                    severity_name = L"CRITICAL";
                    break;
                default:
                    severity_name = L"UNKNOWN";
                    break;
            }

            char_type buffer[256 * 1024];

            snwprintf(buffer,
                      array_size(buffer),
                      TRUNCATE,
                      L"[%s]:[%s(%ld):%s][%s]  ",
                      time_buffer,
                      file_name,
                      line,
                      function_name,
                      severity_name);

            my_stream << buffer;

            va_list arguments;

            va_start(arguments, format_template);

            vsnwprintf(buffer, array_size(buffer), TRUNCATE, format_template, arguments);

            va_end(arguments);

            my_stream << buffer << std::endl;

            my_stream.flush();

            std::wcout << buffer << std::endl;
        }

        my_stream.clear();
    }
}

END_NAMESPACE
