
#include "quicksettings.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProcess>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

QuickSettings::QuickSettings(QWidget *parent)
    : QWidget(parent, Qt::Tool | Qt::FramelessWindowHint)
{
    setObjectName("bullfrogQuickSettings");
    setWindowTitle("Quick Settings");
    setFixedWidth(340);

    buildInterface();
    refreshNetworkStatus();
}

void QuickSettings::buildInterface()
{
    setStyleSheet(R"(
        QWidget#bullfrogQuickSettings {
            background-color: #252630;
            color: white;
            border: 1px solid #494a56;
            border-radius: 14px;
        }

        QLabel {
            border: none;
        }

        QPushButton {
            background-color: #393a47;
            color: white;
            border: 1px solid #4c4d5b;
            border-radius: 9px;
            padding: 9px;
        }

        QPushButton:hover {
            background-color: #50515f;
        }

        QSlider::groove:horizontal {
            height: 6px;
            background: #4a4b58;
            border-radius: 3px;
        }

        QSlider::handle:horizontal {
            background: #8ee36b;
            width: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }
    )");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(18, 18, 18, 18);
    root->setSpacing(12);

    auto *title = new QLabel("Quick Settings", this);
    title->setStyleSheet(
        "font-size: 21px; font-weight: bold;"
    );

    auto *subtitle = new QLabel(
        "System controls and connection status", this
    );

    subtitle->setStyleSheet("color: #b9bac6;");

    root->addWidget(title);
    root->addWidget(subtitle);

    wifiStatus = new QLabel("Checking...", this);
    bluetoothStatus = new QLabel("Checking...", this);
    networkStatus = new QLabel("Checking...", this);
    volumeStatus = new QLabel("Volume: 50%", this);

    wifiButton = new QPushButton("Toggle Wi-Fi", this);
    bluetoothButton = new QPushButton("Toggle Bluetooth", this);

    connect(wifiButton, &QPushButton::clicked,
            this, [this]() {
        const QString current = wifiStatus->text();

        if (current.startsWith("On")) {
            setWifiEnabled(false);
        } else if (current.startsWith("Off")) {
            setWifiEnabled(true);
        } else {
            wifiStatus->setText("Unavailable");
        }
    });

    connect(bluetoothButton, &QPushButton::clicked,
            this, [this]() {
        const QString current = bluetoothStatus->text();

        if (current.startsWith("On")) {
            setBluetoothEnabled(false);
        } else if (current.startsWith("Off")) {
            setBluetoothEnabled(true);
        } else {
            bluetoothStatus->setText("Unavailable");
        }
    });

    root->addWidget(new QLabel("Wi-Fi", this));
    root->addWidget(wifiStatus);
    root->addWidget(wifiButton);

    root->addWidget(new QLabel("Bluetooth", this));
    root->addWidget(bluetoothStatus);
    root->addWidget(bluetoothButton);

    root->addWidget(new QLabel("Network", this));
    root->addWidget(networkStatus);

    auto *volumeTitle = new QLabel("Volume", this);
    auto *volumeSlider = new QSlider(Qt::Horizontal, this);

    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);

    connect(volumeSlider, &QSlider::valueChanged,
            this, &QuickSettings::setVolume);

    root->addWidget(volumeTitle);
    root->addWidget(volumeSlider);
    root->addWidget(volumeStatus);

    auto *settingsButton = new QPushButton(
        "Open Settings", this
    );

    connect(settingsButton, &QPushButton::clicked,
            this, [this]() {
        emit settingsRequested();
    });

    root->addWidget(settingsButton);
}

void QuickSettings::refreshNetworkStatus()
{
    QProcess process;

    process.start(
        "nmcli",
        {"-t", "-f", "WIFI", "radio", "wifi"}
    );

    if (!process.waitForFinished(1500)) {
        process.kill();
        process.waitForFinished();
        wifiStatus->setText("Unavailable (no nmcli)");
    } else if (process.exitCode() != 0) {
        wifiStatus->setText("Unavailable");
    } else {
        const QString result =
            QString::fromUtf8(
                process.readAllStandardOutput()
            ).trimmed();

        if (result == "enabled")
            wifiStatus->setText("On");
        else if (result == "disabled")
            wifiStatus->setText("Off");
        else
            wifiStatus->setText("Unavailable");
    }

    process.start(
        "bluetoothctl",
        {"show"}
    );

    if (!process.waitForFinished(1500)) {
        process.kill();
        process.waitForFinished();
        bluetoothStatus->setText("Unavailable");
    } else {
        const QString output =
            QString::fromUtf8(
                process.readAllStandardOutput()
            );

        if (process.exitCode() != 0) {
            bluetoothStatus->setText("Unavailable");
        } else if (output.contains("Powered: yes")) {
            bluetoothStatus->setText("On");
        } else if (output.contains("Powered: no")) {
            bluetoothStatus->setText("Off");
        } else {
            bluetoothStatus->setText("Unavailable");
        }
    }

    process.start(
        "nmcli",
        {"-t", "-f", "STATE", "general"}
    );

    if (!process.waitForFinished(1500)) {
        process.kill();
        process.waitForFinished();
        networkStatus->setText("Status unavailable");
    } else if (process.exitCode() != 0) {
        networkStatus->setText("Unavailable");
    } else {
        const QString result =
            QString::fromUtf8(
                process.readAllStandardOutput()
            ).trimmed();

        networkStatus->setText(
            result.isEmpty() ? "Unknown" : result
        );
    }
}

void QuickSettings::setWifiEnabled(bool enabled)
{
    const QString state = enabled ? "on" : "off";

    QProcess process;

    process.start("nmcli", {"radio", "wifi", state});

    if (!process.waitForFinished(3000)) {
        process.kill();
        process.waitForFinished();
        wifiStatus->setText("Command timed out");
    } else if (process.exitCode() == 0) {
        refreshNetworkStatus();
    } else {
        wifiStatus->setText("Could not change Wi-Fi");
    }
}

void QuickSettings::setBluetoothEnabled(bool enabled)
{
    const QString state = enabled ? "yes" : "no";

    QProcess process;

    process.start(
        "bluetoothctl",
        {"power", state}
    );

    if (!process.waitForFinished(3000)) {
        process.kill();
        process.waitForFinished();
        bluetoothStatus->setText("Command timed out");
    } else if (process.exitCode() == 0) {
        refreshNetworkStatus();
    } else {
        bluetoothStatus->setText("Could not change Bluetooth");
    }
}

void QuickSettings::setVolume(int value)
{
    volumeStatus->setText(
        QString("Volume: %1%").arg(value)
    );

    QProcess process;

    process.start(
        "pactl",
        {"set-sink-volume", "@DEFAULT_SINK@",
         QString("%1%").arg(value)}
    );

    if (!process.waitForFinished(1500)) {
        process.kill();
        process.waitForFinished();
        volumeStatus->setText(
            QString("Volume: %1% (unavailable)").arg(value)
        );
    } else if (process.exitCode() != 0) {
        volumeStatus->setText(
            QString("Volume: %1% (unavailable)").arg(value)
        );
    }
}

void QuickSettings::runCommand(
    const QString &program,
    const QStringList &arguments)
{
    QProcess::startDetached(program, arguments);
}
