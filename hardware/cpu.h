
#ifndef BULLFROG_CPU_H
#define BULLFROG_CPU_H

#include <QString>

namespace BullFrogHardware
{
    struct CpuInfo
    {
        QString model;
        QString architecture;
        QString coreCount;
        QString frequency;
    };

    CpuInfo getCpuInfo();
}

#endif
