#include "activity_monitor.h"

#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QStorageInfo>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <QDir>
#include <QRegularExpression>
#include <QProcess>

ActivityMonitor::ActivityMonitor(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("BullFrog Activity Monitor");
    setMinimumSize(720, 520);

    setStyleSheet(R"(
        QMainWindow {
            background-color: #202127;
            color: #ffffff;
        }

        QWidget {
            color: #ffffff;
            font-family: "Ubuntu", "DejaVu Sans";
        }

        QLabel#title {
            font-size: 24px;
            font-weight: bold;
            color: #8ee36b;
        }

        QLabel#section {
            font-size: 15px;
            font-weight: bold;
            color: #8ee36b;
        }

        QLabel#info {
            font-size: 13px;
            color: #dedee7;
        }

        QProgressBar {
            background-color: #30313c;
            border: none;
            border-radius: 7px;
            height: 16px;
            text-align: center;
            color: white;
        }

        QProgressBar::chunk {
            background-color: #6da84f;
            border-radius: 7px;
        }

        QPushButton {
            background-color: #393a47;
            color: white;
            border: none;
            border-radius: 7px;
            padding: 8px 16px;
        }

        QPushButton:hover {
            background-color: #50515f;
        }
    )");

    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *mainLayout = new QVBoxLayout(central);

    mainLayout->setContentsMargins(
        24,
        20,
        24,
        20
    );

    mainLayout->setSpacing(16);

    // =========================================================
    // Title
    // =========================================================

    auto *title =
        new QLabel(
            "🐸 BullFrog Activity Monitor",
            central
        );

    title->setObjectName("title");

    mainLayout->addWidget(title);

    auto *subtitle =
        new QLabel(
            "Live system performance and hardware information",
            central
        );

    subtitle->setObjectName("info");

    mainLayout->addWidget(subtitle);


    // =========================================================
    // CPU
    // =========================================================

    auto *cpuSection =
        new QLabel(
            "CPU",
            central
        );

    cpuSection->setObjectName("section");

    mainLayout->addWidget(cpuSection);

    cpuLabel =
        new QLabel(
            "CPU usage: reading...",
            central
        );

    cpuLabel->setObjectName("info");

    mainLayout->addWidget(cpuLabel);

    cpuBar =
        new QProgressBar(central);

    cpuBar->setRange(0, 100);
    cpuBar->setValue(0);
    cpuBar->setFormat("%p%");

    mainLayout->addWidget(cpuBar);


    // =========================================================
    // Memory
    // =========================================================

    auto *memorySection =
        new QLabel(
            "Memory",
            central
        );

    memorySection->setObjectName("section");

    mainLayout->addWidget(memorySection);

    memoryLabel =
        new QLabel(
            "Memory usage: reading...",
            central
        );

    memoryLabel->setObjectName("info");

    mainLayout->addWidget(memoryLabel);

    memoryBar =
        new QProgressBar(central);

    memoryBar->setRange(0, 100);
    memoryBar->setValue(0);
    memoryBar->setFormat("%p%");

    mainLayout->addWidget(memoryBar);


    // =========================================================
    // System
    // =========================================================

    auto *systemSection =
        new QLabel(
            "System",
            central
        );

    systemSection->setObjectName("section");

    mainLayout->addWidget(systemSection);

    systemLabel =
        new QLabel(
            "Reading system information...",
            central
        );

    systemLabel->setObjectName("info");
    systemLabel->setWordWrap(true);

    mainLayout->addWidget(systemLabel);


    // =========================================================
    // GPU
    // =========================================================

    auto *gpuSection =
        new QLabel(
            "Graphics",
            central
        );

    gpuSection->setObjectName("section");

    mainLayout->addWidget(gpuSection);

    gpuLabel =
        new QLabel(
            "Detecting graphics hardware...",
            central
        );

    gpuLabel->setObjectName("info");
    gpuLabel->setWordWrap(true);

    mainLayout->addWidget(gpuLabel);


    // =========================================================
    // Storage
    // =========================================================

    auto *storageSection =
        new QLabel(
            "Storage",
            central
        );

    storageSection->setObjectName("section");

    mainLayout->addWidget(storageSection);

    storageLabel =
        new QLabel(
            "Reading storage...",
            central
        );

    storageLabel->setObjectName("info");
    storageLabel->setWordWrap(true);

    mainLayout->addWidget(storageLabel);


    // =========================================================
    // Close button
    // =========================================================

    auto *buttonLayout =
        new QHBoxLayout();

    buttonLayout->addStretch();

    auto *closeButton =
        new QPushButton(
            "Close",
            central
        );

    connect(
        closeButton,
        &QPushButton::clicked,
        this,
        &QWidget::close
    );

    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);


    // =========================================================
    // Timer
    // =========================================================

    updateTimer =
        new QTimer(this);

    connect(
        updateTimer,
        &QTimer::timeout,
        this,
        &ActivityMonitor::updateStats
    );

    updateTimer->start(1000);

    updateStats();
}


// =============================================================
// CPU usage
// =============================================================

double ActivityMonitor::readCpuUsage()
{
    QFile file("/proc/stat");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0.0;

    QTextStream stream(&file);

    QString line;

    while (!stream.atEnd())
    {
        line = stream.readLine();

        if (!line.startsWith("cpu "))
            continue;

        const QStringList values =
            line.split(
                QRegularExpression("\\s+"),
                Qt::SkipEmptyParts
            );

        if (values.size() < 5)
            return 0.0;

        quint64 user =
            values[1].toULongLong();

        quint64 nice =
            values[2].toULongLong();

        quint64 system =
            values[3].toULongLong();

        quint64 idle =
            values[4].toULongLong();

        quint64 iowait = 0;

        if (values.size() > 5)
        {
            iowait =
                values[5].toULongLong();
        }

        quint64 total =
            user +
            nice +
            system +
            idle +
            iowait;

        if (previousTotal == 0)
        {
            previousIdle =
                idle + iowait;

            previousTotal =
                total;

            return 0.0;
        }

        quint64 idleDifference =
            (idle + iowait) -
            previousIdle;

        quint64 totalDifference =
            total -
            previousTotal;

        previousIdle =
            idle + iowait;

        previousTotal =
            total;

        if (totalDifference == 0)
            return 0.0;

        double usage =
            100.0 *
            (1.0 -
             static_cast<double>(idleDifference) /
             static_cast<double>(totalDifference));

        if (usage < 0.0)
            usage = 0.0;

        if (usage > 100.0)
            usage = 100.0;

        return usage;
    }

    return 0.0;
}


// =============================================================
// CPU
// =============================================================

void ActivityMonitor::updateCpu()
{
    const double usage =
        readCpuUsage();

    cpuBar->setValue(
        static_cast<int>(usage)
    );

    QFile cpuInfo(
        "/proc/cpuinfo"
    );

    QString model =
        "Unknown CPU";

    int cores = 0;

    if (
        cpuInfo.open(
            QIODevice::ReadOnly |
            QIODevice::Text
        )
    )
    {
        QTextStream stream(
            &cpuInfo
        );

        while (!stream.atEnd())
        {
            const QString line =
                stream.readLine();

            if (
                line.startsWith(
                    "model name"
                )
            )
            {
                model =
                    line.section(
                        ':',
                        1
                    ).trimmed();

                break;
            }
        }
    }

    QDir cpuDir(
        "/sys/devices/system/cpu"
    );

    const QStringList cpus =
        cpuDir.entryList(
            QStringList() << "cpu[0-9]*",
            QDir::Dirs
        );

    cores =
        cpus.size();

    cpuLabel->setText(
        QString(
            "Usage: %1%\n"
            "Processor: %2\n"
            "Logical CPUs: %3"
        )
        .arg(
            QString::number(
                usage,
                'f',
                1
            )
        )
        .arg(model)
        .arg(cores)
    );
}


// =============================================================
// Memory
// =============================================================

void ActivityMonitor::updateMemory()
{
    QFile file(
        "/proc/meminfo"
    );

    if (
        !file.open(
            QIODevice::ReadOnly |
            QIODevice::Text
        )
    )
    {
        return;
    }

    QTextStream stream(
        &file
    );

    quint64 total = 0;
    quint64 available = 0;

    while (!stream.atEnd())
    {
        const QString line =
            stream.readLine();

        if (
            line.startsWith(
                "MemTotal:"
            )
        )
        {
            total =
                line.split(
                    QRegularExpression("\\s+"),
                    Qt::SkipEmptyParts
                )[1]
                .toULongLong();
        }

        if (
            line.startsWith(
                "MemAvailable:"
            )
        )
        {
            available =
                line.split(
                    QRegularExpression("\\s+"),
                    Qt::SkipEmptyParts
                )[1]
                .toULongLong();
        }
    }

    if (total == 0)
        return;

    const quint64 used =
        total - available;

    const double percentage =
        (
            static_cast<double>(used) /
            static_cast<double>(total)
        ) * 100.0;

    memoryBar->setValue(
        static_cast<int>(percentage)
    );

    const double totalGB =
        static_cast<double>(total) /
        1024.0 /
        1024.0;

    const double usedGB =
        static_cast<double>(used) /
        1024.0 /
        1024.0;

    memoryLabel->setText(
        QString(
            "Usage: %1%\n"
            "Used: %2 GB\n"
            "Total: %3 GB\n"
            "Available: %4 GB"
        )
        .arg(
            QString::number(
                percentage,
                'f',
                1
            )
        )
        .arg(
            QString::number(
                usedGB,
                'f',
                2
            )
        )
        .arg(
            QString::number(
                totalGB,
                'f',
                2
            )
        )
        .arg(
            QString::number(
                static_cast<double>(available) /
                1024.0 /
                1024.0,
                'f',
                2
            )
        )
    );
}


// =============================================================
// System
// =============================================================

void ActivityMonitor::updateSystem()
{
    QFile uptimeFile(
        "/proc/uptime"
    );

    double uptimeSeconds = 0;

    if (
        uptimeFile.open(
            QIODevice::ReadOnly |
            QIODevice::Text
        )
    )
    {
        QTextStream stream(
            &uptimeFile
        );

        stream >> uptimeSeconds;
    }

    const qint64 totalSeconds =
        static_cast<qint64>(
            uptimeSeconds
        );

    const qint64 days =
        totalSeconds / 86400;

    const qint64 hours =
        (totalSeconds % 86400) / 3600;

    const qint64 minutes =
        (totalSeconds % 3600) / 60;

    QDir proc(
        "/proc"
    );

    const QStringList processes =
        proc.entryList(
            QDir::Dirs |
            QDir::NoDotAndDotDot
        );

    int processCount = 0;

    for (const QString &entry : processes)
    {
        bool ok = false;

        entry.toInt(&ok);

        if (ok)
            processCount++;
    }

    QFile hostnameFile(
        "/etc/hostname"
    );

    QString hostname =
        "Unknown";

    if (
        hostnameFile.open(
            QIODevice::ReadOnly |
            QIODevice::Text
        )
    )
    {
        QTextStream stream(
            &hostnameFile
        );

        hostname =
            stream.readLine().trimmed();
    }

    QString kernel =
        "Unknown";

    QProcess uname;

    uname.start(
        "uname",
        QStringList()
            << "-r"
    );

    if (
        uname.waitForFinished(1000)
    )
    {
        kernel =
            QString::fromUtf8(
                uname.readAllStandardOutput()
            ).trimmed();
    }

    systemLabel->setText(
        QString(
            "Hostname: %1\n"
            "Kernel: %2\n"
            "Uptime: %3 days, %4 hours, %5 minutes\n"
            "Processes: %6"
        )
        .arg(hostname)
        .arg(kernel)
        .arg(days)
        .arg(hours)
        .arg(minutes)
        .arg(processCount)
    );
}


// =============================================================
// GPU
// =============================================================

void ActivityMonitor::updateGpu()
{
    QProcess process;

    process.start(
        "lspci",
        QStringList()
            << "-nn"
    );

    if (
        !process.waitForFinished(1500)
    )
    {
        gpuLabel->setText(
            "Graphics information unavailable.\n"
            "Install pciutils for GPU detection."
        );

        return;
    }

    const QString output =
        QString::fromUtf8(
            process.readAllStandardOutput()
        );

    QStringList lines =
        output.split(
            '\n',
            Qt::SkipEmptyParts
        );

    QStringList graphics;

    for (const QString &line : lines)
    {
        if (
            line.contains(
                "VGA compatible controller",
                Qt::CaseInsensitive
            ) ||
            line.contains(
                "3D controller",
                Qt::CaseInsensitive
            ) ||
            line.contains(
                "Display controller",
                Qt::CaseInsensitive
            )
        )
        {
            graphics.append(
                line
            );
        }
    }

    if (graphics.isEmpty())
    {
        gpuLabel->setText(
            "No dedicated graphics controller detected."
        );

        return;
    }

    gpuLabel->setText(
        graphics.join("\n")
    );
}


// =============================================================
// Storage
// =============================================================

void ActivityMonitor::updateStorage()
{
    const auto volumes =
        QStorageInfo::mountedVolumes();

    QString result;

    for (const QStorageInfo &storage : volumes)
    {
        if (!storage.isValid())
            continue;

        if (!storage.isReady())
            continue;

        const qint64 total =
            storage.bytesTotal();

        const qint64 free =
            storage.bytesFree();

        if (total <= 0)
            continue;

        const qint64 used =
            total - free;

        const double percentage =
            (
                static_cast<double>(used) /
                static_cast<double>(total)
            ) * 100.0;

        const double totalGB =
            static_cast<double>(total) /
            1024.0 /
            1024.0 /
            1024.0;

        const double usedGB =
            static_cast<double>(used) /
            1024.0 /
            1024.0 /
            1024.0;

        result +=
            QString(
                "%1   %2 GB / %3 GB   (%4%)\n"
            )
            .arg(
                storage.rootPath()
            )
            .arg(
                QString::number(
                    usedGB,
                    'f',
                    1
                )
            )
            .arg(
                QString::number(
                    totalGB,
                    'f',
                    1
                )
            )
            .arg(
                QString::number(
                    percentage,
                    'f',
                    1
                )
            );
    }

    if (result.isEmpty())
    {
        result =
            "No mounted storage found.";
    }

    storageLabel->setText(
        result
    );
}


// =============================================================
// Update everything
// =============================================================

void ActivityMonitor::updateStats()
{
    updateCpu();
    updateMemory();
    updateSystem();
    updateGpu();
    updateStorage();
}