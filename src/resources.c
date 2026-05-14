#include "resources.h"

#ifdef _WIN32
#  include <windows.h>
#undef small

double kissat_wall_clock_time(void)
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER t;
    t.LowPart = ft.dwLowDateTime;
    t.HighPart = ft.dwHighDateTime;
    // FILETIME is in 100-nanosecond intervals since Jan 1 1601
    return t.QuadPart * 1e-7;
}

#else
#  include <sys/time.h>

double kissat_wall_clock_time(void)
{
    struct timeval tv;
    if (gettimeofday(&tv, 0))
        return 0;
    return 1e-6 * tv.tv_usec + tv.tv_sec;
}
#endif

#ifndef QUIET

#include "internal.h"
#include "statistics.h"
#include "utilities.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#  include <psapi.h>
#  pragma comment(lib, "psapi.lib")

double kissat_process_time(void)
{
    FILETIME creation, exit, kernel, user;
    if (!GetProcessTimes(GetCurrentProcess(),
        &creation, &exit, &kernel, &user))
        return 0;
    ULARGE_INTEGER k, u;
    k.LowPart = kernel.dwLowDateTime;
    k.HighPart = kernel.dwHighDateTime;
    u.LowPart = user.dwLowDateTime;
    u.HighPart = user.dwHighDateTime;
    return (k.QuadPart + u.QuadPart) * 1e-7;
}

uint64_t kissat_maximum_resident_set_size(void)
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof pmc))
        return 0;
    return (uint64_t)pmc.PeakWorkingSetSize;
}

uint64_t kissat_current_resident_set_size(void)
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof pmc))
        return 0;
    return (uint64_t)pmc.WorkingSetSize;
}

#else
#  include <sys/resource.h>
#  include <sys/types.h>
#  include <unistd.h>

double kissat_process_time(void)
{
    struct rusage u;
    double res;
    if (getrusage(RUSAGE_SELF, &u))
        return 0;
    res = u.ru_utime.tv_sec + 1e-6 * u.ru_utime.tv_usec;
    res += u.ru_stime.tv_sec + 1e-6 * u.ru_stime.tv_usec;
    return res;
}

uint64_t kissat_maximum_resident_set_size(void)
{
    struct rusage u;
    if (getrusage(RUSAGE_SELF, &u))
        return 0;
    return ((uint64_t)u.ru_maxrss) << 10;
}

#  ifdef __APPLE__

#    include <mach/task.h>
mach_port_t mach_task_self(void);

uint64_t kissat_current_resident_set_size(void)
{
    struct task_basic_info info;
    mach_msg_type_number_t count = TASK_BASIC_INFO_COUNT;
    if (KERN_SUCCESS != task_info(mach_task_self(), TASK_BASIC_INFO,
        (task_info_t)&info, &count))
        return 0;
    return info.resident_size;
}

#  else

uint64_t kissat_current_resident_set_size(void)
{
    char path[48];
    sprintf(path, "/proc/%" PRIu64 "/statm", (uint64_t)getpid());
    FILE* file = fopen(path, "r");
    if (!file)
        return 0;
    uint64_t dummy, rss;
    int scanned = fscanf(file, "%" PRIu64 " %" PRIu64 "", &dummy, &rss);
    fclose(file);
    return scanned == 2 ? rss * sysconf(_SC_PAGESIZE) : 0;
}

#  endif
#endif

void kissat_print_resources(kissat* solver)
{
    uint64_t rss = kissat_maximum_resident_set_size();
    double t = kissat_time(solver);
    printf("%s"
        "%-" SFW1 "s "
        "%" SFW2 PRIu64 " "
        "%-" SFW3 "s "
        "%" SFW4 ".0f "
        "MB\n",
        solver->prefix, "maximum-resident-set-size:", rss, "bytes",
        rss / (double)(1 << 20));
#ifdef METRICS
    statistics* statistics = &solver->statistics;
    uint64_t max_allocated = statistics->allocated_max + sizeof(kissat);
    printf("%s"
        "%-" SFW1 "s "
        "%" SFW2 PRIu64 " "
        "%-" SFW3 "s "
        "%" SFW4 ".0f "
        "%%\n",
        solver->prefix, "max-allocated:", max_allocated, "bytes",
        kissat_percent(max_allocated, rss));
#endif
    {
        format buffer;
        memset(&buffer, 0, sizeof buffer);
        printf("%sprocess-time: %30s %18.2f seconds\n", solver->prefix,
            kissat_format_time(&buffer, t), t);
    }
    fflush(stdout);
}

#endif