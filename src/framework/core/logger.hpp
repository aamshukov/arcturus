//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __LOGGER_H__
#define __LOGGER_H__

#pragma once

#if DEBUG || _DEBUG || DBG || _DBG
#   define DEFAULT_LOG_SEVERITY_LEVEL logger::severity::debug
#else
#   define DEFAULT_LOG_SEVERITY_LEVEL logger::severity::critical
#endif

BEGIN_NAMESPACE(core)

class logger : private noncopyable
{
    public:
        enum class severity
        {
            info = 0,
            warning,
            error,
            critical,
            debug
        };

    private:
        std::wofstream  my_stream;
        severity        my_severity;

    private:
        static void     format_time(char_type* buffer, uint16_t buffer_size);

    public:
                        logger();
                       ~logger();

    public:
        static logger&  instance();

        bool            initialize(const string_type& file_name, operation_status& status);
        bool            uninitialize(operation_status& status);

        void            set_severity(severity level);
        void            set_severity(const string_type& level);

        void            log(const char_type* file_name, uint32_t line, const char_type* function_name, severity level, const char_type* format_template, ...);
};

inline logger& logger::instance()
{
    static logger our_logger;
    return our_logger;
}

#if DEBUG || _DEBUG
#   define log_debug(__template, ...)   { logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), logger::severity::debug, __template, ##__VA_ARGS__); }
#else
#   define log_debug(__template, ...)
#endif
#define log_info(__template, ...)       { logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), logger::severity::info, __template, ##__VA_ARGS__); }
#define log_warning(__template, ...)    { logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), logger::severity::warning, __template, ##__VA_ARGS__); }
#define log_error(__template, ...)      { logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), logger::severity::error, __template, ##__VA_ARGS__); }
#define log_critical(__template, ...)   { logger::instance().log(TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__), logger::severity::critical, __template, ##__VA_ARGS__); }

#define log_exception(__ex, __template, ...)                                \
{                                                                           \
    log_critical(__template, ##__VA_ARGS__);                                \
                                                                            \
    if(__ex.what() != nullptr)                                              \
    {                                                                       \
        log_critical(string_to_wstring(__ex.what()).c_str(), ##__VA_ARGS__);\
    }                                                                       \
}

END_NAMESPACE

#endif // __LOGGER_H__
