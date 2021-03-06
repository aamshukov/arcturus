//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __TIMER_H__
#define __TIMER_H__

#pragma once

BEGIN_NAMESPACE(core)

class qpf_timer : private noncopyable
{
    private:
        const static uint64_t kNanosecs = 10000000;
        const static uint64_t kMillisecs = 10000;
        const static uint64_t kFrequency = 1;

        LARGE_INTEGER       my_time;

        LARGE_INTEGER       my_start;
        LARGE_INTEGER       my_stop;

        LARGE_INTEGER       my_frequency;

        double              my_elapsed_time;       // 100-nanosec (FILETIME), time between stop/start calls without pause/resume calls
        double              my_total_elapsed_time; // 100-nanosec (FILETIME), time between stop/start calls with pause/resume calls

    private:
        static string_type  format(const LARGE_INTEGER& time, double eapsed_time);

    public:
                            qpf_timer();

        double              elapsed_time() const;
        double              total_elapsed_time() const;

        void                start();
        void                stop();
        void                pause();
        void                resume();

        string_type         time_as_string() const;
        string_type         elapsed_time_as_string() const;
        string_type         total_elapsed_time_as_string() const;
};

END_NAMESPACE

#endif // __TIMER_H__
