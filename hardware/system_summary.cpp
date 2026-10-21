
#include "system_summary.h"

namespace BullFrogHardware
{
    SystemSummary getSystemSummary()
    {
        SystemSummary summary;

        summary.cpu = getCpuInfo();
        summary.memory = getMemoryInfo();
        summary.network = getNetworkInfo();
        summary.bluetooth = getBluetoothInfo();

        return summary;
    }
}
