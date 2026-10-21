#ifndef BULLFROG_SETTINGS_H
#define BULLFROG_SETTINGS_H

#include <QWidget>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class SettingsApp : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsApp(
        QWidget *parent = nullptr
    );

private:
    void buildInterface();

    void showPage(
        QListWidgetItem *current
    );

    void applySettingsToDesktop();

    QListWidget *sidebar = nullptr;

    QStackedWidget *pages = nullptr;
};

#endif