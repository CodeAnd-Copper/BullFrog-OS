
#include "network.h"

#include <QHostInfo>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

namespace BullFrogHardware
{
    NetworkInfo getNetworkInfo()
    {
        NetworkInfo info;

        info.hostname = QHostInfo::localHostName();
        info.connectionState = "No active interface detected";

        const auto interfaces =
            QNetworkInterface::allInterfaces();

        for (const QNetworkInterface &interface : interfaces) {
            const auto flags = interface.flags();

            if (!(flags & QNetworkInterface::IsUp))
                continue;

            if (flags & QNetworkInterface::IsLoopBack)
                continue;

            info.interfaces.append(interface.humanReadableName());

            for (const QNetworkAddressEntry &entry :
                 interface.addressEntries()) {
                const QHostAddress address = entry.ip();

                if (address.isNull())
                    continue;

                if (address.protocol() == QAbstractSocket::IPv4Protocol ||
                    address.protocol() == QAbstractSocket::IPv6Protocol) {
                    info.addresses.append(address.toString());
                }
            }

            if (flags & QNetworkInterface::IsRunning)
                info.connectionState = "Active interface detected";
        }

        if (info.interfaces.isEmpty())
            info.connectionState = "No active interface detected";

        return info;
    }
}
