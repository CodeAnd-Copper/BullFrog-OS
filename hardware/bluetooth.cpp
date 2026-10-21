
#include "bluetooth.h"

#include <QProcess>
#include <QStringList>

namespace BullFrogHardware
{
    BluetoothInfo getBluetoothInfo()
    {
        BluetoothInfo info;

        info.available = false;
        info.powered = false;
        info.status = "Bluetooth unavailable";
        info.controller = "No controller detected";

        QProcess process;
        process.start("bluetoothctl", {"show"});

        if (!process.waitForStarted(1500)) {
            info.status = "bluetoothctl is not installed";
            return info;
        }

        if (!process.waitForFinished(2500)) {
            process.kill();
            process.waitForFinished();

            info.status = "Bluetooth query timed out";
            return info;
        }

        const QString output =
            QString::fromUtf8(process.readAllStandardOutput());

        if (process.exitCode() != 0 ||
            !output.contains("Controller ")) {
            info.status = "No Bluetooth controller available";
            return info;
        }

        info.available = true;

        const QStringList lines = output.split('\n');

        for (const QString &line : lines) {
            const QString trimmed = line.trimmed();

            if (trimmed.startsWith("Controller ")) {
                info.controller = trimmed;
            } else if (trimmed == "Powered: yes") {
                info.powered = true;
            }
        }

        info.status = info.powered
            ? "Controller powered on"
            : "Controller available but powered off";

        return info;
    }
}
