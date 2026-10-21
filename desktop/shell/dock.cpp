#include "dock.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QSize>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QStringList>

Dock::Dock(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("bullfrogDock");

    // =========================================================
    // Floating BullFrog dock
    // =========================================================

    setFixedSize(420, 80);

    setStyleSheet(R"(
        QWidget#bullfrogDock {
            background-color: rgba(28, 29, 36, 245);
            border: 1px solid #555762;
            border-radius: 18px;
        }

        QPushButton {
            background-color: transparent;
            color: white;
            border: none;
            border-radius: 12px;
        }

        QPushButton:hover {
            background-color: rgba(255, 255, 255, 30);
        }

        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 50);
        }
    )");

    // =========================================================
    // Main vertical layout
    // =========================================================

    auto *mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    mainLayout->setSpacing(0);

    // =========================================================
    // Button content area
    // =========================================================

    auto *content = new QWidget(this);

    content->setObjectName("dockContent");

    content->setStyleSheet(R"(
        QWidget#dockContent {
            background: transparent;
            border: none;
        }
    )");

    auto *layout = new QHBoxLayout(content);

    layout->setContentsMargins(
        12,
        7,
        12,
        7
    );

    layout->setSpacing(8);

    // =========================================================
    // BullFrog launcher / Garry the Frog
    // =========================================================

    auto *launcher = createAppButton(
        QString(),
        "Open BullFrog launcher"
    );

    const QStringList mascotCandidates = {
        QDir::current().filePath(
            "assets/download.png"
        ),

        QDir::current().filePath(
            "../assets/download.png"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "assets/download.png"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "../assets/download.png"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "../../assets/download.png"
        )
    };

    bool mascotLoaded = false;

    for (const QString &path : mascotCandidates) {

        if (!QFileInfo::exists(path))
            continue;

        QPixmap mascot(path);

        if (mascot.isNull())
            continue;

        launcher->setIcon(
            QIcon(mascot)
        );

        launcher->setIconSize(
            QSize(42, 42)
        );

        launcher->setText("");

        mascotLoaded = true;

        break;
    }

    if (!mascotLoaded) {

        launcher->setText("🐸");

        launcher->setStyleSheet(R"(
            QPushButton {
                background-color: transparent;
                color: white;
                border: none;
                border-radius: 12px;
                font-size: 27px;
            }

            QPushButton:hover {
                background-color: rgba(255, 255, 255, 30);
            }

            QPushButton:pressed {
                background-color: rgba(255, 255, 255, 50);
            }
        )");
    }

    layout->addWidget(launcher);

    connect(
        launcher,
        &QPushButton::clicked,
        this,
        &Dock::launcherRequested
    );

    // =========================================================
    // Files
    // =========================================================

    auto *files = createAppButton(
        QString(),
        "Files"
    );

    const QString filesIcon =
        QDir::current().filePath(
            "assets/icons/File exploror.svg"
        );

    const QStringList filesIconCandidates = {
        filesIcon,

        QDir::current().filePath(
            "../assets/icons/File exploror.svg"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "assets/icons/File exploror.svg"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "../assets/icons/File exploror.svg"
        )
    };

    for (const QString &path : filesIconCandidates) {

        if (!QFileInfo::exists(path))
            continue;

        QIcon icon(path);

        if (icon.isNull())
            continue;

        files->setIcon(icon);
        files->setIconSize(QSize(32, 32));

        break;
    }

    layout->addWidget(files);

    connect(
        files,
        &QPushButton::clicked,
        this,
        &Dock::filesRequested
    );

    // =========================================================
    // Terminal
    // =========================================================

    auto *terminal = createAppButton(
        QString(),
        "Terminal"
    );

    terminal->setText(">_");

    terminal->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            color: white;
            border: none;
            border-radius: 12px;
            font-size: 19px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: rgba(255, 255, 255, 30);
        }

        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 50);
        }
    )");

    layout->addWidget(terminal);

    connect(
        terminal,
        &QPushButton::clicked,
        this,
        &Dock::terminalRequested
    );

    // =========================================================
    // Browser
    // =========================================================

    auto *browser = createAppButton(
        QString(),
        "BullFrog Browser"
    );

    const QStringList browserIconCandidates = {
        QDir::current().filePath(
            "assets/icons/Browser icon.svg"
        ),

        QDir::current().filePath(
            "../assets/icons/Browser icon.svg"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "assets/icons/Browser icon.svg"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "../assets/icons/Browser icon.svg"
        )
    };

    for (const QString &path : browserIconCandidates) {

        if (!QFileInfo::exists(path))
            continue;

        QIcon icon(path);

        if (icon.isNull())
            continue;

        browser->setIcon(icon);
        browser->setIconSize(QSize(32, 32));

        break;
    }

    layout->addWidget(browser);

    connect(
        browser,
        &QPushButton::clicked,
        this,
        &Dock::browserRequested
    );

    // =========================================================
    // Settings
    // =========================================================

    auto *settings = createAppButton(
        QString(),
        "Settings"
    );

    const QStringList settingsIconCandidates = {
        QDir::current().filePath(
            "assets/icons/setting icon.svg"
        ),

        QDir::current().filePath(
            "../assets/icons/setting icon.svg"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "assets/icons/setting icon.svg"
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "../assets/icons/setting icon.svg"
        )
    };

    for (const QString &path : settingsIconCandidates) {

        if (!QFileInfo::exists(path))
            continue;

        QIcon icon(path);

        if (icon.isNull())
            continue;

        settings->setIcon(icon);
        settings->setIconSize(QSize(32, 32));

        break;
    }

    layout->addWidget(settings);

    connect(
        settings,
        &QPushButton::clicked,
        this,
        &Dock::settingsRequested
    );

    // =========================================================
    // Make all buttons behave evenly
    // =========================================================

    for (auto *button :
         content->findChildren<QPushButton *>()) {

        button->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding
        );
    }

    // =========================================================
    // Add content to dock
    // =========================================================

    mainLayout->addWidget(content, 1);

    // =========================================================
    // REAL BLACK BOTTOM BORDER
    // =========================================================
    //
    // This is deliberately a separate widget instead of relying
    // on border-bottom in the stylesheet.
    //

    auto *bottomBorder = new QWidget(this);

    bottomBorder->setObjectName(
        "dockBottomBorder"
    );

    bottomBorder->setFixedHeight(4);

    bottomBorder->setStyleSheet(R"(
        QWidget#dockBottomBorder {
            background-color: #000000;
            border: none;
        }
    )");

    mainLayout->addWidget(bottomBorder);
}

// =============================================================
// Create dock button
// =============================================================

QPushButton *Dock::createAppButton(
    const QString &text,
    const QString &tooltip)
{
    auto *button =
        new QPushButton(text, this);

    button->setFixedSize(
        60,
        56
    );

    button->setToolTip(
        tooltip
    );

    button->setCursor(
        Qt::PointingHandCursor
    );

    button->setFocusPolicy(
        Qt::StrongFocus
    );

    return button;
}