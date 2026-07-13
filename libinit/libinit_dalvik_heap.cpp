/*
 * Copyright (C) 2021-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/libinit_dalvik_heap.h"
#include "include/libinit_utils.h"

#include <sys/sysinfo.h>

#include <string>

#define GB(b) (b * 1024ull * 1024 * 1024)

static const std::string kHeapStartSizeProp = "dalvik.vm.heapstartsize";
static const std::string kHeapGrowthLimitProp = "dalvik.vm.heapgrowthlimit";
static const std::string kHeapSizeProp = "dalvik.vm.heapsize";
static const std::string kHeapMinFreeProp = "dalvik.vm.heapminfree";
static const std::string kHeapMaxFreeProp = "dalvik.vm.heapmaxfree";
static const std::string kHeapTargetUtilizationProp = "dalvik.vm.heaptargetutilization";

struct dalvik_heap_info {
    std::string heapstartsize;
    std::string heapgrowthlimit;
    std::string heapsize;
    std::string heapminfree;
    std::string heapmaxfree;
    std::string heaptargetutilization;
};

// frameworks/native/build/phone-xhdpi-12288-dalvik-heap.mk
static const dalvik_heap_info dalvik_heap_info_12288 = {
        .heapstartsize = "24m",
        .heapgrowthlimit = "512m",
        .heapsize = "512m",
        .heapminfree = "8m",
        .heapmaxfree = "96m",
        .heaptargetutilization = "0.5",
};

// frameworks/native/build/phone-xhdpi-8192-dalvik-heap.mk
static const dalvik_heap_info dalvik_heap_info_8192 = {
        .heapstartsize = "16m",
        .heapgrowthlimit = "384m",
        .heapsize = "512m",
        .heapminfree = "8m",
        .heapmaxfree = "64m",
        .heaptargetutilization = "0.5",
};

void set_dalvik_heap() {
    struct sysinfo sys;
    const dalvik_heap_info* dhi;

    sysinfo(&sys);

    // POCO X7 Pro: 8GB and 12GB SKUs. Marketed capacity is not what sysinfo
    // reports (firmware/CMA reservations), so use a gap threshold:
    // 8GB units report ~7-8GiB, 12GB units report ~11-12GiB.
    if (sys.totalram > GB(10)) {
        dhi = &dalvik_heap_info_12288;
    } else {
        dhi = &dalvik_heap_info_8192;
    }

    property_override(kHeapStartSizeProp, dhi->heapstartsize);
    property_override(kHeapGrowthLimitProp, dhi->heapgrowthlimit);
    property_override(kHeapSizeProp, dhi->heapsize);
    property_override(kHeapMinFreeProp, dhi->heapminfree);
    property_override(kHeapMaxFreeProp, dhi->heapmaxfree);
    property_override(kHeapTargetUtilizationProp, dhi->heaptargetutilization);
}
