
#ifndef BULLFROG_BLUETOOTH_H
#define BULLFROG_BLUETOOTH_H

#include <QString>

namespace BullFrogHardware
{
    struct BluetoothInfo
    {
        bool available = false;
        bool powered = false;
        QString status;
        QString controller;
    };

    BluetoothInfo getBluetoothInfo();
}

#endif
