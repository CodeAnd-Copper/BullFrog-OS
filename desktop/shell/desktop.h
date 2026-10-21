#ifndef BULLFROG_DESKTOP_H
#define BULLFROG_DESKTOP_H

#include <QWidget>

class QVBoxLayout;
class QResizeEvent;

class Panel;
class Dock;
class Launcher;
class QuickSettings;
class Notifications;

class FilesApp;
class TerminalApp;
class SettingsApp;

class BullFrogDesktop : public QWidget
{
    Q_OBJECT

public:
    explicit BullFrogDesktop(
        QWidget *parent = nullptr
    );

    void reloadAppearance();

protected:
    void resizeEvent(
        QResizeEvent *event
    ) override;

private:
    void buildInterface();

    void openFiles();
    void openTerminal();
    void openSettings();
    void openBrowser();

    void toggleLauncher();
    void toggleQuickSettings();
    void toggleNotifications();

    void positionDock();

    QVBoxLayout *rootLayout = nullptr;

    QWidget *workspace = nullptr;

    Panel *panel = nullptr;
    Dock *dock = nullptr;

    Launcher *launcher = nullptr;
    QuickSettings *quickSettings = nullptr;
    Notifications *notifications = nullptr;

    FilesApp *filesApp = nullptr;
    TerminalApp *terminalApp = nullptr;
    SettingsApp *settingsApp = nullptr;
};

#endif