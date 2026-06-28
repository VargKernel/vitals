#pragma once

#include "common.h"
#include "procfs.h"

// Gradient type
enum GradType { GRAD_CPU, GRAD_MEM, GRAD_TEMP, GRAD_STORAGE, GRAD_HIST };

// Hwmon sensor data
struct HwmonSensor {
    std::string label;
    double      temp_celsius = 0.0;
    double      temp_max     = 0.0; // temp*_max  (high trip point)
    double      temp_crit    = 0.0; // temp*_crit
    bool        is_package   = false;
    bool        has_max      = false;
    bool        has_crit     = false;
};

struct HwmonChip {
    std::string              name;
    std::vector<HwmonSensor> sensors;
};

// Global application state
struct AppState {
    cpustat                prev_cpu  = {};
    std::vector<cpustat>   prev_core;
    std::deque<double>     cpu_hist; // Rolling CPU% history for sparkline
    std::vector<netdev>    prev_net;
    std::vector<diskstats> prev_disk;
    Clock::time_point      t_prev;
    double                 dt        = 1.0;
    double                 peak_rx   = 0.0;
    double                 peak_tx   = 0.0;
    cpuinfo                ci        = {};
    systemuname            un        = {};
    std::string            local_ip  = "N/A";
};

// Defined in main.cpp, referenced throughout.
extern AppState G;
