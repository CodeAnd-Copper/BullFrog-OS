#include "desktop.h"

#include "panel.h"
#include "dock.h"

#include "../launcher/launcher.h"
#include "../quicksettings/quicksettings.h"
#include "../notifications/notifications.h"

#include "../apps/files.h"
#include "../apps/terminal.h"
#include "../apps/settings.h"

#include "../settings/settings_manager.h"
#include <QApplication>
#include <QColor>
#include <QCoreApplication>
#include <QDir>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QProcess>
#include <QRect>
#include <QResizeEvent>
#include <QScreen>
#include <QSize>
#include <QStringList>
#include <QVBoxLayout>


// =============================================================
// Wallpaper workspace
// =============================================================

class WallpaperWorkspace : public QWidget
{
public:

    explicit WallpaperWorkspace(
        QWidget *parent = nullptr
    )
        : QWidget(parent)
    {
        setObjectName(
            "bullfrogWorkspace"
        );

        setMinimumSize(
            200,
            200
        );

        loadWallpaper();
    }


    // =========================================================
    // Load wallpaper
    // =========================================================

    void loadWallpaper()
    {
        const QString wallpaperName =
            SettingsManager::wallpaper();


        QString wallpaperFile;


        if (
            wallpaperName ==
            "Bullfrog 1"
        )
        {
            wallpaperFile =
                "Bullfrog 1.jpg";
        }
        else if (
            wallpaperName ==
            "Bullfrog 3"
        )
        {
            wallpaperFile =
                "Bullfrog 3.jpg";
        }
        else
        {
            wallpaperFile =
                "Bullfrog 2.jpg";
        }


        const QStringList paths =
        {
            QDir::current().filePath(
                "assets/wallpapers/" +
                wallpaperFile
            ),

            QDir(
                QCoreApplication::applicationDirPath()
            ).filePath(
                "assets/wallpapers/" +
                wallpaperFile
            ),

            QDir(
                QCoreApplication::applicationDirPath()
            ).filePath(
                "../assets/wallpapers/" +
                wallpaperFile
            ),

            QDir(
                QCoreApplication::applicationDirPath()
            ).filePath(
                "../../assets/wallpapers/" +
                wallpaperFile
            ),

            QDir(
                QCoreApplication::applicationDirPath()
            ).filePath(
                "../../../assets/wallpapers/" +
                wallpaperFile
            )
        };


        wallpaper =
            QPixmap();


        for (
            const QString &path :
            paths
        )
        {
            if (
                wallpaper.load(path)
            )
            {
                break;
            }
        }


        update();
    }


protected:

    // =========================================================
    // Paint wallpaper
    // =========================================================

    void paintEvent(
        QPaintEvent *event
    ) override
    {
        Q_UNUSED(event);


        QPainter painter(this);


        painter.setRenderHint(
            QPainter::SmoothPixmapTransform,
            true
        );


        if (
            wallpaper.isNull()
        )
        {
            painter.fillRect(
                rect(),
                QColor("#373944")
            );


            painter.setPen(
                QColor("#8ee36b")
            );


            painter.drawText(
                rect(),
                Qt::AlignCenter,
                "BullFrog wallpaper not found"
            );


            return;
        }


        const QSize scaledSize =
            wallpaper.size().scaled(
                size(),
                Qt::KeepAspectRatioByExpanding
            );


        const QPixmap scaled =
            wallpaper.scaled(
                scaledSize,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation
            );


        const int x =
            (
                scaled.width() -
                width()
            ) / 2;


        const int y =
            (
                scaled.height() -
                height()
            ) / 2;


        painter.drawPixmap(
            rect(),
            scaled,
            QRect(
                x,
                y,
                width(),
                height()
            )
        );
    }


private:

    QPixmap wallpaper;
};


// =============================================================
// BullFrog Desktop
// =============================================================

BullFrogDesktop::BullFrogDesktop(
    QWidget *parent
)
    : QWidget(parent)
{
    setWindowTitle(
        "BullFrog OS"
    );


    setMinimumSize(
        900,
        600
    );


    buildInterface();
}


// =============================================================
// Build interface
// =============================================================

void BullFrogDesktop::buildInterface()
{
    setObjectName(
        "bullfrogDesktop"
    );


    setStyleSheet(R"(
        QWidget#bullfrogDesktop {
            background-color: #373944;
            color: white;
            font-family: "Ubuntu", "DejaVu Sans";
        }

        QWidget#bullfrogWorkspace {
            background-color: #373944;
        }
    )");


    rootLayout =
        new QVBoxLayout(
            this
        );


    rootLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );


    rootLayout->setSpacing(
        0
    );


    // =========================================================
    // Panel
    // =========================================================

    panel =
        new Panel(
            this
        );


    rootLayout->addWidget(
        panel
    );


    // =========================================================
    // Desktop body
    // =========================================================

    auto *body =
        new QWidget(
            this
        );


    body->setObjectName(
        "bullfrogBody"
    );


    auto *bodyLayout =
        new QHBoxLayout(
            body
        );


    bodyLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );


    bodyLayout->setSpacing(
        0
    );


    // =========================================================
    // Wallpaper
    // =========================================================

    workspace =
        new WallpaperWorkspace(
            body
        );


    bodyLayout->addWidget(
        workspace,
        1
    );


    rootLayout->addWidget(
        body,
        1
    );


    // =========================================================
    // Dock
    // =========================================================

    dock =
        new Dock(
            workspace
        );


    // =========================================================
    // Launcher
    // =========================================================

    launcher =
        new Launcher(
            this
        );


    // =========================================================
    // Quick settings
    // =========================================================

    quickSettings =
        new QuickSettings(
            this
        );


    // =========================================================
    // Notifications
    // =========================================================

    notifications =
        new Notifications(
            this
        );


    // =========================================================
    // Panel connections
    // =========================================================

    connect(
        panel,
        &Panel::launcherRequested,
        this,
        &BullFrogDesktop::toggleLauncher
    );


    connect(
        panel,
        &Panel::quickSettingsRequested,
        this,
        &BullFrogDesktop::toggleQuickSettings
    );


    connect(
        panel,
        &Panel::notificationsRequested,
        this,
        &BullFrogDesktop::toggleNotifications
    );


    // =========================================================
    // Dock connections
    // =========================================================

    connect(
        dock,
        &Dock::launcherRequested,
        this,
        &BullFrogDesktop::toggleLauncher
    );


    connect(
        dock,
        &Dock::filesRequested,
        this,
        &BullFrogDesktop::openFiles
    );


    connect(
        dock,
        &Dock::terminalRequested,
        this,
        &BullFrogDesktop::openTerminal
    );


    connect(
        dock,
        &Dock::settingsRequested,
        this,
        &BullFrogDesktop::openSettings
    );


    connect(
        dock,
        &Dock::browserRequested,
        this,
        &BullFrogDesktop::openBrowser
    );


    // =========================================================
    // Launcher connections
    // =========================================================

    connect(
        launcher,
        &Launcher::filesRequested,
        this,
        &BullFrogDesktop::openFiles
    );


    connect(
        launcher,
        &Launcher::terminalRequested,
        this,
        &BullFrogDesktop::openTerminal
    );


    connect(
        launcher,
        &Launcher::settingsRequested,
        this,
        &BullFrogDesktop::openSettings
    );


    connect(
        launcher,
        &Launcher::browserRequested,
        this,
        &BullFrogDesktop::openBrowser
    );


    // =========================================================
    // Quick settings
    // =========================================================

    connect(
        quickSettings,
        &QuickSettings::settingsRequested,
        this,
        &BullFrogDesktop::openSettings
    );


    // =========================================================
    // Dock position
    // =========================================================

    positionDock();
}


// =============================================================
// Reload appearance
// =============================================================

void BullFrogDesktop::reloadAppearance()
{
    // =========================================================
    // Reload wallpaper
    // =========================================================

    if (workspace)
    {
        auto *wallpaperWorkspace =
            static_cast<WallpaperWorkspace *>(
                workspace
            );


        wallpaperWorkspace->loadWallpaper();
    }


    // =========================================================
    // IMPORTANT:
    // Tell the panel to read the NEW theme
    // =========================================================

    if (panel)
    {
        panel->checkTheme();
    }


    // =========================================================
    // Refresh desktop
    // =========================================================

    update();


    // =========================================================
    // Refresh dock
    // =========================================================

    if (dock)
    {
        dock->update();
        dock->repaint();
    }


    // =========================================================
    // Refresh launcher
    // =========================================================

    if (launcher)
    {
        launcher->update();
        launcher->repaint();
    }


    // =========================================================
    // Refresh quick settings
    // =========================================================

    if (quickSettings)
    {
        quickSettings->update();
        quickSettings->repaint();
    }


    // =========================================================
    // Refresh notifications
    // =========================================================

    if (notifications)
    {
        notifications->update();
        notifications->repaint();
    }


    // =========================================================
    // Force Qt to process the visual update
    // =========================================================

    QApplication::processEvents();
}


// =============================================================
// Position dock
// =============================================================

void BullFrogDesktop::positionDock()
{
    if (
        !dock ||
        !workspace
    )
    {
        return;
    }


    const int x =
        (
            workspace->width() -
            dock->width()
        ) / 2;


    const int y =
        workspace->height() -
        dock->height() -
        18;


    dock->move(
        x,
        y
    );


    dock->raise();
}


// =============================================================
// Resize
// =============================================================

void BullFrogDesktop::resizeEvent(
    QResizeEvent *event
)
{
    QWidget::resizeEvent(
        event
    );


    positionDock();
}


// =============================================================
// Files
// =============================================================

void BullFrogDesktop::openFiles()
{
    if (!filesApp)
    {
        filesApp =
            new FilesApp();
    }


    filesApp->show();
    filesApp->raise();
    filesApp->activateWindow();
}


// =============================================================
// Terminal
// =============================================================

void BullFrogDesktop::openTerminal()
{
    if (!terminalApp)
    {
        terminalApp =
            new TerminalApp();
    }


    terminalApp->show();
    terminalApp->raise();
    terminalApp->activateWindow();
}


// =============================================================
// Settings
// =============================================================

void BullFrogDesktop::openSettings()
{
    if (!settingsApp)
    {
        settingsApp =
            new SettingsApp();
    }


    settingsApp->show();
    settingsApp->raise();
    settingsApp->activateWindow();
}


// =============================================================
// Browser
// =============================================================

void BullFrogDesktop::openBrowser()
{
    if (
        QProcess::startDetached(
            "firefox",
            QStringList{}
        )
    )
    {
        return;
    }


    if (
        QProcess::startDetached(
            "firefox-esr",
            QStringList{}
        )
    )
    {
        return;
    }


    QMessageBox::warning(
        this,
        "Firefox could not start",
        "BullFrog could not launch Firefox.\n\n"
        "Try running 'firefox' in the terminal."
    );
}


// =============================================================
// Launcher
// =============================================================

void BullFrogDesktop::toggleLauncher()
{
    if (!launcher)
    {
        return;
    }


    if (
        launcher->isVisible()
    )
    {
        launcher->hide();
        return;
    }


    const QScreen *screen =
        QGuiApplication::primaryScreen();


    if (screen)
    {
        const QRect area =
            screen->availableGeometry();


        launcher->move(
            area.center().x()
            - launcher->width() / 2,

            area.center().y()
            - launcher->height() / 2
        );
    }


    launcher->toggleLauncher();
}


// =============================================================
// Quick settings
// =============================================================

void BullFrogDesktop::toggleQuickSettings()
{
    if (!quickSettings)
    {
        return;
    }


    const QScreen *screen =
        QGuiApplication::primaryScreen();


    if (screen)
    {
        const QRect area =
            screen->availableGeometry();


        quickSettings->move(
            area.right()
            - quickSettings->width()
            - 16,

            area.top()
            + 42
        );
    }


    quickSettings->setVisible(
        !quickSettings->isVisible()
    );


    if (
        quickSettings->isVisible()
    )
    {
        quickSettings->raise();
        quickSettings->activateWindow();
    }
}


// =============================================================
// Notifications
// =============================================================

void BullFrogDesktop::toggleNotifications()
{
    if (!notifications)
    {
        return;
    }


    const QScreen *screen =
        QGuiApplication::primaryScreen();


    if (screen)
    {
        const QRect area =
            screen->availableGeometry();


        notifications->move(
            area.right()
            - notifications->width()
            - 16,

            area.top()
            + 42
        );
    }


    notifications->togglePanel();
}