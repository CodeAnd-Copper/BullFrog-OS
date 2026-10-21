
#include "cpu.h"

#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

namespace BullFrogHardware
{
    CpuInfo getCpuInfo()
    {
        CpuInfo info;

        info.model = "Unknown";
        info.architecture = "Unknown";
        info.coreCount = "Unknown";
        info.frequency = "Unknown";

        QFile cpuFile("/proc/cpuinfo");

        if (cpuFile.open(QIODevice::ReadOnly |
                         QIODevice::Text)) {
            QTextStream stream(&cpuFile);

            int processorCount = 0;

            while (!stream.atEnd()) {
                const QString line = stream.readLine();

                if (line.startsWith("model name")) {
                    const int colon = line.indexOf(':');

                    if (colon >= 0 && info.model == "Unknown")
                        info.model = line.mid(colon + 1).trimmed();
                }

                if (line.startsWith("processor"))
                    ++processorCount;
            }

            if (processorCount > 0)
                info.coreCount = QString::number(processorCount);
        }

        QFile archFile("/proc/sys/kernel/ostype");

        if (archFile.open(QIODevice::ReadOnly |
                          QIODevice::Text)) {
            // Use uname's machine architecture below instead.
            archFile.close();
        }

        QFile unameFile("/proc/version");

        if (unameFile.open(QIODevice::ReadOnly |
                           QIODevice::Text)) {
            info.architecture = "Linux";
        }

        QFile cpuSetFile("/sys/devices/system/cpu/online");

        if (cpuSetFile.open(QIODevice::ReadOnly |
                            QIODevice::Text)) {
            const QString online =
                QString::fromUtf8(cpuSetFile.readAll()).trimmed();

            if (!online.isEmpty())
                info.coreCount += " logical CPUs online (" + online + ")";
        }

        QFile freqFile(
            "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
        );

        if (freqFile.open(QIODevice::ReadOnly |
                          QIODevice::Text)) {
            bool ok = false;

            const double khz =
                QString::fromUtf8(freqFile.readAll())
                    .trimmed().toDouble(&ok);

            if (ok && khz > 0) {
                info.frequency = QString("%1 GHz")
                    .arg(khz / 1000000.0, 0, 'f', 2);
            }
        }

        return info;
    }
}
