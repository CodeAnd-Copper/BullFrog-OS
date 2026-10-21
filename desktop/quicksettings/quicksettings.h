
#ifndef BULLFROG_QUICKSETTINGS_H
#define BULLFROG_QUICKSETTINGS_H

#include <QWidget>

class QLabel;
class QSlider;
class QPushButton;

class QuickSettings : public QWidget
{
    Q_OBJECT

public:
    explicit QuickSettings(QWidget *parent = nullptr);

signals:
    void settingsRequested();

private:
    void buildInterface();
    void refreshNetworkStatus();
    void setWifiEnabled(bool enabled);
    void setBluetoothEnabled(bool enabled);
    void setVolume(int value);
    void runCommand(const QString &program,
                    const QStringList &arguments);

    QLabel *wifiStatus = nullptr;
    QLabel *bluetoothStatus = nullptr;
    QLabel *networkStatus = nullptr;
    QLabel *volumeStatus = nullptr;

    QPushButton *wifiButton = nullptr;
    QPushButton *bluetoothButton = nullptr;
};

#endif
