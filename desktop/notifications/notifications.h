
#ifndef BULLFROG_NOTIFICATIONS_H
#define BULLFROG_NOTIFICATIONS_H

#include <QWidget>

class QLabel;
class QPushButton;
class QVBoxLayout;

class Notifications : public QWidget
{
    Q_OBJECT

public:
    explicit Notifications(QWidget *parent = nullptr);

    void togglePanel();
    void addNotification(
        const QString &title,
        const QString &message
    );
    void clearNotifications();

signals:
    void notificationsCleared();

private:
    void buildInterface();

    QVBoxLayout *notificationLayout = nullptr;
    QLabel *emptyLabel = nullptr;
    QPushButton *clearButton = nullptr;
    int notificationCount = 0;
};

#endif
