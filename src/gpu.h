#pragma once

#include <string>
#include <vector>

enum class GpuVendor { AMD, Intel, Nvidia, Unknown };

// One GPU. Fields default to -1.0 meaning "not available" — the panel renders
// "n/a" rather than a bogus zero, since availability differs a lot by vendor
// and driver (e.g. Intel iGPUs rarely expose VRAM or power via sysfs).
struct GpuInfo {
    GpuVendor   vendor = GpuVendor::Unknown;
    std::string name;

    double util_pct    = -1.0; // 0-100
    double mem_used_mb  = -1.0;
    double mem_total_mb = -1.0;
    double temp_c        = -1.0;
    double power_w        = -1.0;
};

// Enumerates all GPUs found on the system:
//  - AMD / Intel: pure sysfs (/sys/class/drm/card*/device), no external deps.
//  - NVIDIA: sysfs exposes almost nothing useful under the proprietary driver,
//    so this shells out to `nvidia-smi` instead. If the binary isn't present
//    (no NVIDIA driver installed), this silently contributes zero rows.
// Never throws; on any read failure a GPU is simply omitted.
std::vector<GpuInfo> parse_gpus();
