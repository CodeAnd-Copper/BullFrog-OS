
#ifndef BULLFROG_NETWORK_H
#define BULLFROG_NETWORK_H

#include <QString>
#include <QStringList>

namespace BullFrogHardware
{
    struct NetworkInfo
    {
        QString hostname;
        QString connectionState;
        QStringList interfaces;
        QStringList addresses;
    };

    NetworkInfo getNetworkInfo();
}

#endif
