#ifndef BULLFROG_PANEL_H
#define BULLFROG_PANEL_H

#include <QWidget>

class QLabel;
class QPushButton;
class QTimer;

class Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = nullptr);

    void checkTheme();

signals:
    void launcherRequested();
    void quickSettingsRequested();
    void notificationsRequested();

private:
    void updateClock();

    QLabel *clockLabel = nullptr;
    QLabel *dateLabel = nullptr;

    QPushButton *activitiesButton = nullptr;
    QPushButton *notificationButton = nullptr;
    QPushButton *quickSettingsButton = nullptr;

    QTimer *clockTimer = nullptr;
};

#endif