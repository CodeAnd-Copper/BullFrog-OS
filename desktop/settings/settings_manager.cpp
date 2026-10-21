#include "settings_manager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

QString SettingsManager::configPath()
{
    const QString configDirectory =
        QDir::homePath() +
        "/.config/bullfrog";

    QDir().mkpath(configDirectory);

    return configDirectory +
           "/settings.conf";
}

QString SettingsManager::theme()
{
    const QString path =
        configPath();

    QFile file(path);

    if (!file.open(
        QIODevice::ReadOnly |
        QIODevice::Text
    ))
    {
        return "BullFrog Dark";
    }

    QTextStream stream(&file);

    while (!stream.atEnd())
    {
        const QString line =
            stream.readLine().trimmed();

        if (line.startsWith("theme="))
        {
            const QString value =
                line.mid(
                    QString("theme=").length()
                ).trimmed();

            if (!value.isEmpty())
                return value;
        }
    }

    return "BullFrog Dark";
}

QString SettingsManager::wallpaper()
{
    const QString path =
        configPath();

    QFile file(path);

    if (!file.open(
        QIODevice::ReadOnly |
        QIODevice::Text
    ))
    {
        return "Bullfrog 2";
    }

    QTextStream stream(&file);

    while (!stream.atEnd())
    {
        const QString line =
            stream.readLine().trimmed();

        if (line.startsWith("wallpaper="))
        {
            const QString value =
                line.mid(
                    QString("wallpaper=").length()
                ).trimmed();

            if (!value.isEmpty())
                return value;
        }
    }

    return "Bullfrog 2";
}

void SettingsManager::setTheme(
    const QString &newTheme
)
{
    QString currentWallpaper =
        wallpaper();

    QFile file(configPath());

    if (!file.open(
        QIODevice::WriteOnly |
        QIODevice::Text |
        QIODevice::Truncate
    ))
    {
        return;
    }

    QTextStream stream(&file);

    stream
        << "theme="
        << newTheme
        << '\n';

    stream
        << "wallpaper="
        << currentWallpaper
        << '\n';
}

void SettingsManager::setWallpaper(
    const QString &newWallpaper
)
{
    QString currentTheme =
        theme();

    QFile file(configPath());

    if (!file.open(
        QIODevice::WriteOnly |
        QIODevice::Text |
        QIODevice::Truncate
    ))
    {
        return;
    }

    QTextStream stream(&file);

    stream
        << "theme="
        << currentTheme
        << '\n';

    stream
        << "wallpaper="
        << newWallpaper
        << '\n';
}