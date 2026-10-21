#include "launcher.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QStringList>


// =============================================================
// Find launcher assets
// =============================================================

static QString launcherAsset(
    const QString &name
)
{
    const QStringList paths = {

        QDir::current().filePath(
            "assets/icons/" + name
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "assets/icons/" + name
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "../assets/icons/" + name
        ),

        QDir(
            QCoreApplication::applicationDirPath()
        ).filePath(
            "../../assets/icons/" + name
        )
    };


    for (const QString &path : paths)
    {
        if (QFileInfo::exists(path))
        {
            return path;
        }
    }


    return {};
}


// =============================================================
// Constructor
// =============================================================

Launcher::Launcher(
    QWidget *parent
)
    : QWidget(parent)
{
    setWindowFlags(
        Qt::Popup |
        Qt::FramelessWindowHint
    );


    setFixedSize(
        470,
        440
    );


    setStyleSheet(R"(
        QWidget {
            color: #f2f5f3;
            font-family: "Ubuntu";
            font-size: 14px;
        }

        Launcher {
            background: #17211e;
            border: 1px solid #40534a;
            border-radius: 18px;
        }

        QLineEdit {
            background: #26352e;
            border: 1px solid #526c5b;
            border-radius: 10px;
            padding: 11px;
            color: white;
        }

        QLineEdit:focus {
            border: 1px solid #66cf8c;
        }

        QPushButton {
            background: #24342c;
            border: 1px solid #394c40;
            border-radius: 12px;
            padding: 8px;
            min-height: 70px;
            color: white;
        }

        QPushButton:hover {
            background: #315941;
            border-color: #66cf8c;
        }

        QPushButton:pressed {
            background: #3b684c;
        }

        QLabel#LauncherTitle {
            color: #86e8a5;
            font-size: 21px;
            font-weight: bold;
        }

        QScrollArea {
            border: none;
            background: transparent;
        }
    )");


    buildInterface();
}


// =============================================================
// Build launcher interface
// =============================================================

void Launcher::buildInterface()
{
    auto *root =
        new QVBoxLayout(this);


    root->setContentsMargins(
        18,
        18,
        18,
        18
    );


    root->setSpacing(
        14
    );


    // ---------------------------------------------------------
    // Title
    // ---------------------------------------------------------

    auto *title =
        new QLabel(
            "🐸  BullFrog Apps",
            this
        );


    title->setObjectName(
        "LauncherTitle"
    );


    root->addWidget(
        title
    );


    // ---------------------------------------------------------
    // Search
    // ---------------------------------------------------------

    searchBox =
        new QLineEdit(this);


    searchBox->setPlaceholderText(
        "Search applications..."
    );


    searchBox->setClearButtonEnabled(
        true
    );


    root->addWidget(
        searchBox
    );


    // ---------------------------------------------------------
    // Scroll area
    // ---------------------------------------------------------

    auto *scroll =
        new QScrollArea(this);


    scroll->setWidgetResizable(
        true
    );


    scroll->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff
    );


    // ---------------------------------------------------------
    // Application grid
    // ---------------------------------------------------------

    appGrid =
        new QWidget();


    auto *grid =
        new QGridLayout(appGrid);


    grid->setContentsMargins(
        4,
        4,
        4,
        4
    );


    grid->setHorizontalSpacing(
        10
    );


    grid->setVerticalSpacing(
        10
    );


    // ---------------------------------------------------------
    // Applications
    // ---------------------------------------------------------

    struct AppEntry
    {
        QString name;
        QString icon;
    };


    const QList<AppEntry> apps = {

        {
            "Files",
            "File exploror.svg"
        },

        {
            "Terminal",
            ""
        },

        {
            "Settings",
            "setting icon.svg"
        },

        {
            "Firefox ESR",
            "Browser icon.svg"
        },

        {
            "Activity Monitor",
            ""
        }
    };


    // ---------------------------------------------------------
    // Create application buttons
    // ---------------------------------------------------------

    for (int i = 0; i < apps.size(); ++i)
    {
        const AppEntry &app =
            apps.at(i);


        auto *button =
            new QPushButton(
                app.name,
                appGrid
            );


        button->setProperty(
            "appName",
            app.name
        );


        button->setMinimumSize(
            180,
            92
        );


        button->setIconSize(
            QSize(
                34,
                34
            )
        );


        // -----------------------------------------------------
        // Load icon
        // -----------------------------------------------------

        if (!app.icon.isEmpty())
        {
            const QString path =
                launcherAsset(
                    app.icon
                );


            if (!path.isEmpty())
            {
                button->setIcon(
                    QIcon(path)
                );
            }
        }


        // -----------------------------------------------------
        // Text fallback icons
        // -----------------------------------------------------

        if (
            button->icon().isNull() &&
            app.name == "Terminal"
        )
        {
            button->setText(
                "⌨\nTerminal"
            );
        }


        if (
            button->icon().isNull() &&
            app.name == "Activity Monitor"
        )
        {
            button->setText(
                "📊\nActivity Monitor"
            );
        }


        // -----------------------------------------------------
        // Grid position
        // -----------------------------------------------------

        const int row =
            i / 2;


        const int column =
            i % 2;


        grid->addWidget(
            button,
            row,
            column
        );


        // -----------------------------------------------------
        // Application click
        // -----------------------------------------------------

        connect(
            button,
            &QPushButton::clicked,
            this,
            [this, name = app.name]()
            {
                hide();


                launcherVisible =
                    false;


                if (name == "Files")
                {
                    emit filesRequested();
                }
                else if (
                    name == "Terminal"
                )
                {
                    emit terminalRequested();
                }
                else if (
                    name == "Settings"
                )
                {
                    emit settingsRequested();
                }
                else if (
                    name == "Firefox ESR"
                )
                {
                    emit browserRequested();
                }
                else if (
                    name == "Activity Monitor"
                )
                {
                    emit activityMonitorRequested();
                }
            }
        );
    }


    // ---------------------------------------------------------
    // Equal column widths
    // ---------------------------------------------------------

    grid->setColumnStretch(
        0,
        1
    );


    grid->setColumnStretch(
        1,
        1
    );


    // ---------------------------------------------------------
    // Put grid inside scroll area
    // ---------------------------------------------------------

    scroll->setWidget(
        appGrid
    );


    root->addWidget(
        scroll
    );


    // ---------------------------------------------------------
    // Search filtering
    // ---------------------------------------------------------

    connect(
        searchBox,
        &QLineEdit::textChanged,
        this,
        &Launcher::filterApplications
    );
}


// =============================================================
// Filter applications
// =============================================================

void Launcher::filterApplications(
    const QString &text
)
{
    const QString query =
        text.trimmed();


    const auto buttons =
        appGrid->findChildren<
            QPushButton *
        >();


    for (auto *button : buttons)
    {
        const QString name =
            button->property(
                "appName"
            ).toString();


        button->setVisible(
            query.isEmpty() ||
            name.contains(
                query,
                Qt::CaseInsensitive
            )
        );
    }
}


// =============================================================
// Toggle launcher
// =============================================================

void Launcher::toggleLauncher()
{
    if (isVisible())
    {
        hide();


        launcherVisible =
            false;


        return;
    }


    show();


    raise();


    activateWindow();


    searchBox->clear();


    searchBox->setFocus();


    launcherVisible =
        true;
}


// =============================================================
// Launcher visibility
// =============================================================

bool Launcher::isLauncherVisible() const
{
    return launcherVisible;
}


// =============================================================
// Keyboard handling
// =============================================================

void Launcher::keyPressEvent(
    QKeyEvent *event
)
{
    if (
        event->key() ==
        Qt::Key_Escape
    )
    {
        hide();


        launcherVisible =
            false;


        event->accept();


        return;
    }


    QWidget::keyPressEvent(
        event
    );
}