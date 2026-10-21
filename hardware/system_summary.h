
#ifndef BULLFROG_SYSTEM_SUMMARY_H
#define BULLFROG_SYSTEM_SUMMARY_H

#include "cpu.h"
#include "memory.h"
#include "network.h"
#include "bluetooth.h"

namespace BullFrogHardware
{
    struct SystemSummary
    {
        CpuInfo cpu;
        MemoryInfo memory;
        NetworkInfo network;
        BluetoothInfo bluetooth;
    };

    SystemSummary getSystemSummary();
}

#endif
