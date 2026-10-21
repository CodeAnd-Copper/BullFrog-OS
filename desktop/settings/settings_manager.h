#ifndef BULLFROG_SETTINGS_MANAGER_H
#define BULLFROG_SETTINGS_MANAGER_H

#include <QString>

class SettingsManager
{
public:
    static QString theme();
    static QString wallpaper();

    static void setTheme(const QString &theme);
    static void setWallpaper(const QString &wallpaper);

private:
    static QString configPath();
};

#endif