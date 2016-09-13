#ifndef PROGRESS_MAIN_HPP
#define PROGRESS_MAIN_HPP

#include "progress_perf.hpp"

#include "perf_data.hpp"

void start_progress_monitor(PerfData* perf_data, int delay) {
    while(true) {
        ProgressesPerf* progresses_perf = new ProgressesPerf();
        progresses_perf->readStat();

        perf_data->progress_perf_buffer.add(progresses_perf);

        sleep(delay);
    }
}

#endif