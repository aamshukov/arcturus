//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#include <core/pch.hpp>
#include <core/timer.hpp>

BEGIN_NAMESPACE(core)

#define TIME_BUFFER_SIZE (64)

qpf_timer::qpf_timer()
         : my_time(), my_start(), my_stop(), my_frequency(), my_elapsed_time(), my_total_elapsed_time()
{
    FILETIME fileTime;
    FILETIME localFileTime;

    GetSystemTimeAsFileTime(&fileTime);
    FileTimeToLocalFileTime(&fileTime, &localFileTime);

    my_time.LowPart = localFileTime.dwLowDateTime;
    my_time.HighPart = localFileTime.dwHighDateTime;

    QueryPerformanceFrequency(&my_frequency);
}

double qpf_timer::elapsed_time() const
{
    return my_elapsed_time;
}

double qpf_timer::total_elapsed_time() const
{
    return my_elapsed_time;
}

void qpf_timer::start()
{
    my_elapsed_time = 0.0;

    DWORD_PTR thread_affinity_mask(SetThreadAffinityMask(GetCurrentThread(), 0));

    QueryPerformanceCounter(&my_start);

    SetThreadAffinityMask(GetCurrentThread(), thread_affinity_mask);
}

void qpf_timer::stop()
{
    DWORD_PTR thread_affinity_mask(SetThreadAffinityMask(GetCurrentThread(), 0));

    QueryPerformanceCounter(&my_stop);

    SetThreadAffinityMask(GetCurrentThread(), thread_affinity_mask);

    my_elapsed_time = (((my_stop.QuadPart - my_start.QuadPart)*static_cast<double>(qpf_timer::kNanosecs)) / (my_frequency.QuadPart != 0 ?
                                                                                                             my_frequency.QuadPart :
                                                                                                             static_cast<double>(qpf_timer::kFrequency)));
    my_total_elapsed_time += my_elapsed_time;
}

void qpf_timer::pause()
{
    stop();
}

void qpf_timer::resume()
{
    start();
}

string_type qpf_timer::time_as_string() const
{
    return qpf_timer::format(my_time, 0.0);
}

string_type qpf_timer::elapsed_time_as_string() const
{
    string_type result;

    wchar_t buffer[TIME_BUFFER_SIZE];

    _snwprintf_s(buffer,
                 sizeof(buffer) / sizeof(buffer[0]),
                 _TRUNCATE,
                 L"%ld ms",
                 static_cast<unsigned long>(my_elapsed_time / static_cast<double>(qpf_timer::kMillisecs)));

    result = buffer;

    return result;
}

string_type qpf_timer::total_elapsed_time_as_string() const
{
    string_type result;

    wchar_t buffer[TIME_BUFFER_SIZE];

    _snwprintf_s(buffer,
                 sizeof(buffer)/sizeof(buffer[0]),
                 _TRUNCATE,
                 L"%ld ms",
                 static_cast<unsigned long>(my_total_elapsed_time / static_cast<double>(qpf_timer::kMillisecs)));

    result = buffer;

    return result;
}

string_type qpf_timer::format(const LARGE_INTEGER& time0, double elapsed_time)
{
    string_type result;

    LARGE_INTEGER time;

    time.QuadPart = (LONGLONG)(static_cast<double>(time0.QuadPart) + elapsed_time);

    FILETIME fileTime;

    fileTime.dwLowDateTime = time.LowPart;
    fileTime.dwHighDateTime = time.HighPart;

    SYSTEMTIME sysTime;

    FileTimeToSystemTime(&fileTime, &sysTime);

    wchar_t buffer[TIME_BUFFER_SIZE];

    _snwprintf_s(buffer,
                 sizeof(buffer) / sizeof(buffer[0]),
                 _TRUNCATE,
                 L"%.2d%.2d%.2d %.2d:%.2d:%.2d.%.3d",
                 sysTime.wYear,
                 sysTime.wMonth,
                 sysTime.wDay,
                 sysTime.wHour,
                 sysTime.wMinute,
                 sysTime.wSecond,
                 sysTime.wMilliseconds);
          
    result = buffer;

    return result;
}

END_NAMESPACE
