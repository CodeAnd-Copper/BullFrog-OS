#ifndef BULLFROG_DOCK_H
#define BULLFROG_DOCK_H

#include <QWidget>
#include <QString>

class QPushButton;

class Dock : public QWidget
{
    Q_OBJECT

public:
    explicit Dock(QWidget *parent = nullptr);

signals:
    void launcherRequested();
    void filesRequested();
    void terminalRequested();
    void browserRequested();
    void settingsRequested();

private:
    QPushButton *createAppButton(
        const QString &text,
        const QString &tooltip
    );
};

#endif