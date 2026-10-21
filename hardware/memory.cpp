
#include "memory.h"

#include <QFile>
#include <QRegularExpression>
#include <QTextStream>
#include <QMap>

namespace BullFrogHardware
{
    MemoryInfo getMemoryInfo()
    {
        MemoryInfo info;

        info.total = "Unknown";
        info.used = "Unknown";
        info.available = "Unknown";
        info.usagePercent = 0;

        QFile file("/proc/meminfo");

        if (!file.open(QIODevice::ReadOnly |
                       QIODevice::Text)) {
            return info;
        }

        QMap<QString, qint64> values;
        QTextStream stream(&file);

        while (!stream.atEnd()) {
            const QString line = stream.readLine();
            const int colon = line.indexOf(':');

            if (colon < 0)
                continue;

            const QString key = line.left(colon).trimmed();
            const QString value = line.mid(colon + 1).trimmed();

            const QString number =
                value.section(' ', 0, 0);

            bool ok = false;
            const qint64 amount = number.toLongLong(&ok);

            if (ok)
                values.insert(key, amount);
        }

        const qint64 totalKb = values.value("MemTotal");
        const qint64 availableKb = values.value("MemAvailable");

        if (totalKb <= 0)
            return info;

        const qint64 usedKb =
            totalKb - availableKb;

        const auto toGiB = [](qint64 kb) {
            return QString("%1 GiB")
                .arg(kb / 1048576.0, 0, 'f', 2);
        };

        info.total = toGiB(totalKb);
        info.used = toGiB(usedKb);
        info.available = toGiB(availableKb);

        info.usagePercent = static_cast<int>(
            (usedKb * 100) / totalKb
        );

        return info;
    }
}
