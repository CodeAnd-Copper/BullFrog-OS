#ifndef BULLFROG_ACTIVITY_MONITOR_H
#define BULLFROG_ACTIVITY_MONITOR_H

#include <QMainWindow>

class QLabel;
class QProgressBar;
class QTimer;

class ActivityMonitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ActivityMonitor(QWidget *parent = nullptr);

private slots:
    void updateStats();

private:
    double readCpuUsage();
    void updateCpu();
    void updateMemory();
    void updateSystem();
    void updateStorage();
    void updateGpu();

    QLabel *cpuLabel = nullptr;
    QLabel *memoryLabel = nullptr;
    QLabel *systemLabel = nullptr;
    QLabel *storageLabel = nullptr;
    QLabel *gpuLabel = nullptr;

    QProgressBar *cpuBar = nullptr;
    QProgressBar *memoryBar = nullptr;

    QTimer *updateTimer = nullptr;

    quint64 previousIdle = 0;
    quint64 previousTotal = 0;
};

#endif