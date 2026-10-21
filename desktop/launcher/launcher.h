#ifndef BULLFROG_LAUNCHER_H
#define BULLFROG_LAUNCHER_H

#include <QWidget>
#include <QString>

class QLineEdit;
class QKeyEvent;

class Launcher : public QWidget
{
    Q_OBJECT

public:
    explicit Launcher(QWidget *parent = nullptr);

    void toggleLauncher();
    bool isLauncherVisible() const;

signals:
    void filesRequested();
    void terminalRequested();
    void settingsRequested();
    void browserRequested();
    void activityMonitorRequested();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void buildInterface();
    void filterApplications(const QString &text);

    QLineEdit *searchBox = nullptr;
    QWidget *appGrid = nullptr;

    bool launcherVisible = false;
};

#endif