
#ifndef BULLFROG_MEMORY_H
#define BULLFROG_MEMORY_H

#include <QString>

namespace BullFrogHardware
{
    struct MemoryInfo
    {
        QString total;
        QString used;
        QString available;
        int usagePercent = 0;
    };

    MemoryInfo getMemoryInfo();
}

#endif
