#include "settings.h"

#include "../settings/settings_manager.h"
#include "../shell/desktop.h"

#include <QApplication>
#include <QComboBox>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSlider>
#include <QStackedWidget>
#include <QStringList>
#include <QVBoxLayout>


// =============================================================
// Constructor
// =============================================================

SettingsApp::SettingsApp(
    QWidget *parent
)
    : QWidget(parent)
{
    setWindowTitle(
        "BullFrog Settings"
    );

    setMinimumSize(
        900,
        600
    );

    resize(
        950,
        620
    );

    buildInterface();
}


// =============================================================
// Apply settings to BullFrog Desktop
// =============================================================

void SettingsApp::applySettingsToDesktop()
{
    const auto windows =
        QApplication::topLevelWidgets();


    for (QWidget *widget : windows)
    {
        auto *desktop =
            qobject_cast<
                BullFrogDesktop *
            >(widget);


        if (!desktop)
            continue;


        desktop->reloadAppearance();

        break;
    }
}


// =============================================================
// Build interface
// =============================================================

void SettingsApp::buildInterface()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #20231f;
            color: #f2f5f3;
            font-family: "Ubuntu", "DejaVu Sans";
        }

        QListWidget {
            background-color: #171a17;
            border: none;
            padding: 14px 8px;
            outline: none;
        }

        QListWidget::item {
            padding: 13px 16px;
            margin: 3px 4px;
            border-radius: 9px;
            color: #cfd8d2;
        }

        QListWidget::item:hover {
            background-color: #28352d;
        }

        QListWidget::item:selected {
            background-color: #315941;
            color: white;
        }

        QStackedWidget {
            background-color: #20231f;
        }

        QLabel#pageTitle {
            font-size: 28px;
            font-weight: bold;
            color: #86e8a5;
        }

        QLabel#pageSubtitle {
            font-size: 14px;
            color: #aebbb3;
        }

        QFrame#card {
            background-color: #292e2a;
            border: 1px solid #3c463f;
            border-radius: 14px;
        }

        QLabel#cardTitle {
            font-size: 16px;
            font-weight: bold;
            color: white;
        }

        QLabel#cardText {
            color: #aebbb3;
            font-size: 13px;
        }

        QComboBox {
            background-color: #303832;
            border: 1px solid #48564c;
            border-radius: 8px;
            padding: 8px 12px;
            min-width: 180px;
        }

        QComboBox:hover {
            border-color: #66cf8c;
        }

        QComboBox QAbstractItemView {
            background-color: #292e2a;
            color: white;
            selection-background-color: #315941;
            border: 1px solid #48564c;
        }

        QSlider::groove:horizontal {
            height: 5px;
            background: #3b453e;
            border-radius: 3px;
        }

        QSlider::handle:horizontal {
            width: 16px;
            margin: -6px 0;
            background: #86e8a5;
            border-radius: 8px;
        }

        QPushButton {
            background-color: #315941;
            color: white;
            border: none;
            border-radius: 9px;
            padding: 9px 16px;
        }

        QPushButton:hover {
            background-color: #3c6e4e;
        }

        QPushButton:pressed {
            background-color: #274b35;
        }
    )");


    // =========================================================
    // Main layout
    // =========================================================

    auto *mainLayout =
        new QHBoxLayout(this);


    mainLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );


    mainLayout->setSpacing(
        0
    );


    // =========================================================
    // Sidebar
    // =========================================================

    sidebar =
        new QListWidget(this);


    sidebar->setFixedWidth(
        250
    );


    sidebar->setIconSize(
        QSize(32, 32)
    );


    const QStringList iconPaths =
    {
        "Moniter.svg",
        "Paint brush.svg",
        "Speakericon.svg",
        "network icon.svg",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "Garry the frog.svg"
    };


    const QStringList categories =
    {
        "Display",
        "Appearance",
        "Sound",
        "Network",
        "Bluetooth",
        "Keyboard",
        "Mouse",
        "Date & Time",
        "Power",
        "Privacy & Security",
        "Storage",
        "About BullFrog"
    };


    auto findIcon =
        [](const QString &filename)
        -> QString
    {
        if (filename.isEmpty())
            return {};


        const QStringList paths =
        {
            QDir::current().filePath(
                "assets/icons/" +
                filename
            ),

            QDir(
                QCoreApplication::
                    applicationDirPath()
            ).filePath(
                "assets/icons/" +
                filename
            ),

            QDir(
                QCoreApplication::
                    applicationDirPath()
            ).filePath(
                "../assets/icons/" +
                filename
            ),

            QDir(
                QCoreApplication::
                    applicationDirPath()
            ).filePath(
                "../../assets/icons/" +
                filename
            )
        };


        for (
            const QString &path :
            paths
        )
        {
            if (
                QFileInfo::exists(path)
            )
            {
                return path;
            }
        }


        return {};
    };


    for (
        int i = 0;
        i < categories.size();
        ++i
    )
    {
        auto *item =
            new QListWidgetItem(
                categories[i]
            );


        if (
            !iconPaths[i].isEmpty()
        )
        {
            const QString iconPath =
                findIcon(
                    iconPaths[i]
                );


            if (
                !iconPath.isEmpty()
            )
            {
                item->setIcon(
                    QIcon(iconPath)
                );
            }
        }


        sidebar->addItem(
            item
        );
    }


    // =========================================================
    // Pages
    // =========================================================

    pages =
        new QStackedWidget(this);


    // =========================================================
    // Display page
    // =========================================================

    auto *displayPage =
        new QWidget();


    auto *displayLayout =
        new QVBoxLayout(
            displayPage
        );


    displayLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    displayLayout->setSpacing(
        20
    );


    auto *displayTitle =
        new QLabel(
            "Display"
        );


    displayTitle->setObjectName(
        "pageTitle"
    );


    auto *displaySubtitle =
        new QLabel(
            "Configure your screen and visual settings."
        );


    displaySubtitle->setObjectName(
        "pageSubtitle"
    );


    displayLayout->addWidget(
        displayTitle
    );


    displayLayout->addWidget(
        displaySubtitle
    );


    // Resolution

    auto *resolutionCard =
        new QFrame();


    resolutionCard->setObjectName(
        "card"
    );


    auto *resolutionLayout =
        new QVBoxLayout(
            resolutionCard
        );


    auto *resolutionTitle =
        new QLabel(
            "Resolution"
        );


    resolutionTitle->setObjectName(
        "cardTitle"
    );


    auto *resolutionText =
        new QLabel(
            "Choose the resolution used by the desktop."
        );


    resolutionText->setObjectName(
        "cardText"
    );


    auto *resolution =
        new QComboBox();


    resolution->addItems(
        {
            "1920 × 1080",
            "1600 × 900",
            "1366 × 768",
            "1280 × 720"
        }
    );


    resolutionLayout->addWidget(
        resolutionTitle
    );


    resolutionLayout->addWidget(
        resolutionText
    );


    resolutionLayout->addWidget(
        resolution
    );


    displayLayout->addWidget(
        resolutionCard
    );


    // Refresh rate

    auto *refreshCard =
        new QFrame();


    refreshCard->setObjectName(
        "card"
    );


    auto *refreshLayout =
        new QVBoxLayout(
            refreshCard
        );


    auto *refreshTitle =
        new QLabel(
            "Refresh Rate"
        );


    refreshTitle->setObjectName(
        "cardTitle"
    );


    auto *refresh =
        new QComboBox();


    refresh->addItems(
        {
            "60 Hz",
            "75 Hz",
            "120 Hz",
            "144 Hz",
            "165 Hz"
        }
    );


    refresh->setCurrentText(
        "144 Hz"
    );


    refreshLayout->addWidget(
        refreshTitle
    );


    refreshLayout->addWidget(
        refresh
    );


    displayLayout->addWidget(
        refreshCard
    );


    // Brightness

    auto *brightnessCard =
        new QFrame();


    brightnessCard->setObjectName(
        "card"
    );


    auto *brightnessLayout =
        new QVBoxLayout(
            brightnessCard
        );


    auto *brightnessTitle =
        new QLabel(
            "Brightness"
        );


    brightnessTitle->setObjectName(
        "cardTitle"
    );


    auto *brightness =
        new QSlider(
            Qt::Horizontal
        );


    brightness->setRange(
        0,
        100
    );


    brightness->setValue(
        80
    );


    brightnessLayout->addWidget(
        brightnessTitle
    );


    brightnessLayout->addWidget(
        brightness
    );


    displayLayout->addWidget(
        brightnessCard
    );


    displayLayout->addStretch();


    pages->addWidget(
        displayPage
    );


    // =========================================================
    // Appearance page
    // =========================================================

    auto *appearancePage =
        new QWidget();


    auto *appearanceLayout =
        new QVBoxLayout(
            appearancePage
        );


    appearanceLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    appearanceLayout->setSpacing(
        20
    );


    auto *appearanceTitle =
        new QLabel(
            "Appearance"
        );


    appearanceTitle->setObjectName(
        "pageTitle"
    );


    auto *appearanceSubtitle =
        new QLabel(
            "Customize the look of BullFrog OS."
        );


    appearanceSubtitle->setObjectName(
        "pageSubtitle"
    );


    appearanceLayout->addWidget(
        appearanceTitle
    );


    appearanceLayout->addWidget(
        appearanceSubtitle
    );


    // =========================================================
    // Wallpaper card
    // =========================================================

    auto *wallpaperCard =
        new QFrame();


    wallpaperCard->setObjectName(
        "card"
    );


    auto *wallpaperLayout =
        new QVBoxLayout(
            wallpaperCard
        );


    auto *wallpaperTitle =
        new QLabel(
            "Wallpaper"
        );


    wallpaperTitle->setObjectName(
        "cardTitle"
    );


    auto *wallpaperText =
        new QLabel(
            "Choose the BullFrog desktop wallpaper."
        );


    wallpaperText->setObjectName(
        "cardText"
    );


    auto *wallpaper =
        new QComboBox();


    wallpaper->addItems(
        {
            "Bullfrog 1",
            "Bullfrog 2",
            "Bullfrog 3"
        }
    );


    wallpaper->setCurrentText(
        SettingsManager::wallpaper()
    );


    wallpaperLayout->addWidget(
        wallpaperTitle
    );


    wallpaperLayout->addWidget(
        wallpaperText
    );


    wallpaperLayout->addWidget(
        wallpaper
    );


    appearanceLayout->addWidget(
        wallpaperCard
    );


    // =========================================================
    // Wallpaper change
    // =========================================================

    connect(
        wallpaper,
        &QComboBox::currentTextChanged,
        this,
        [this](const QString &value)
        {
            SettingsManager::
                setWallpaper(value);

            applySettingsToDesktop();
        }
    );


    // =========================================================
    // Theme card
    // =========================================================

    auto *themeCard =
        new QFrame();


    themeCard->setObjectName(
        "card"
    );


    auto *themeLayout =
        new QVBoxLayout(
            themeCard
        );


    auto *themeTitle =
        new QLabel(
            "Theme"
        );


    themeTitle->setObjectName(
        "cardTitle"
    );


    auto *themeText =
        new QLabel(
            "Choose the BullFrog desktop theme."
        );


    themeText->setObjectName(
        "cardText"
    );


    auto *theme =
        new QComboBox();


    theme->addItems(
        {
            "BullFrog Dark",
            "BullFrog Light",
            "System Default"
        }
    );


    theme->setCurrentText(
        SettingsManager::theme()
    );


    themeLayout->addWidget(
        themeTitle
    );


    themeLayout->addWidget(
        themeText
    );


    themeLayout->addWidget(
        theme
    );


    appearanceLayout->addWidget(
        themeCard
    );


    // =========================================================
    // Theme change
    // =========================================================

    connect(
        theme,
        &QComboBox::currentTextChanged,
        this,
        [this](const QString &value)
        {
            SettingsManager::
                setTheme(value);

            applySettingsToDesktop();
        }
    );


    appearanceLayout->addStretch();


    pages->addWidget(
        appearancePage
    );


    // =========================================================
    // Sound
    // =========================================================

    auto *soundPage =
        new QWidget();


    auto *soundLayout =
        new QVBoxLayout(
            soundPage
        );


    soundLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *soundTitle =
        new QLabel(
            "Sound"
        );


    soundTitle->setObjectName(
        "pageTitle"
    );


    auto *soundSubtitle =
        new QLabel(
            "Configure volume and audio devices."
        );


    soundSubtitle->setObjectName(
        "pageSubtitle"
    );


    soundLayout->addWidget(
        soundTitle
    );


    soundLayout->addWidget(
        soundSubtitle
    );


    auto *volumeCard =
        new QFrame();


    volumeCard->setObjectName(
        "card"
    );


    auto *volumeLayout =
        new QVBoxLayout(
            volumeCard
        );


    auto *volumeTitle =
        new QLabel(
            "Master Volume"
        );


    volumeTitle->setObjectName(
        "cardTitle"
    );


    auto *volume =
        new QSlider(
            Qt::Horizontal
        );


    volume->setRange(
        0,
        100
    );


    volume->setValue(
        70
    );


    volumeLayout->addWidget(
        volumeTitle
    );


    volumeLayout->addWidget(
        volume
    );


    soundLayout->addWidget(
        volumeCard
    );


    soundLayout->addStretch();


    pages->addWidget(
        soundPage
    );


    // =========================================================
    // Network
    // =========================================================

    auto *networkPage =
        new QWidget();


    auto *networkLayout =
        new QVBoxLayout(
            networkPage
        );


    networkLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *networkTitle =
        new QLabel(
            "Network"
        );


    networkTitle->setObjectName(
        "pageTitle"
    );


    auto *networkSubtitle =
        new QLabel(
            "Manage Wi-Fi, Ethernet and network connections."
        );


    networkSubtitle->setObjectName(
        "pageSubtitle"
    );


    networkLayout->addWidget(
        networkTitle
    );


    networkLayout->addWidget(
        networkSubtitle
    );


    auto *wifiCard =
        new QFrame();


    wifiCard->setObjectName(
        "card"
    );


    auto *wifiLayout =
        new QVBoxLayout(
            wifiCard
        );


    auto *wifiTitle =
        new QLabel(
            "Wi-Fi"
        );


    wifiTitle->setObjectName(
        "cardTitle"
    );


    auto *wifiStatus =
        new QLabel(
            "Network management will be connected to Linux."
        );


    wifiStatus->setObjectName(
        "cardText"
    );


    auto *wifiButton =
        new QPushButton(
            "Open Network Settings"
        );


    wifiLayout->addWidget(
        wifiTitle
    );


    wifiLayout->addWidget(
        wifiStatus
    );


    wifiLayout->addWidget(
        wifiButton
    );


    networkLayout->addWidget(
        wifiCard
    );


    networkLayout->addStretch();


    pages->addWidget(
        networkPage
    );


    // =========================================================
    // Bluetooth
    // =========================================================

    auto *bluetoothPage =
        new QWidget();


    auto *bluetoothLayout =
        new QVBoxLayout(
            bluetoothPage
        );


    bluetoothLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *bluetoothTitle =
        new QLabel(
            "Bluetooth"
        );


    bluetoothTitle->setObjectName(
        "pageTitle"
    );


    auto *bluetoothSubtitle =
        new QLabel(
            "Connect and manage Bluetooth devices."
        );


    bluetoothSubtitle->setObjectName(
        "pageSubtitle"
    );


    bluetoothLayout->addWidget(
        bluetoothTitle
    );


    bluetoothLayout->addWidget(
        bluetoothSubtitle
    );


    auto *bluetoothCard =
        new QFrame();


    bluetoothCard->setObjectName(
        "card"
    );


    auto *bluetoothCardLayout =
        new QVBoxLayout(
            bluetoothCard
        );


    auto *bluetoothStatus =
        new QLabel(
            "Bluetooth management will be connected to Linux."
        );


    bluetoothStatus->setObjectName(
        "cardText"
    );


    bluetoothCardLayout->addWidget(
        bluetoothStatus
    );


    bluetoothLayout->addWidget(
        bluetoothCard
    );


    bluetoothLayout->addStretch();


    pages->addWidget(
        bluetoothPage
    );


    // =========================================================
    // Keyboard
    // =========================================================

    auto *keyboardPage =
        new QWidget();


    auto *keyboardLayout =
        new QVBoxLayout(
            keyboardPage
        );


    keyboardLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *keyboardTitle =
        new QLabel(
            "Keyboard"
        );


    keyboardTitle->setObjectName(
        "pageTitle"
    );


    keyboardLayout->addWidget(
        keyboardTitle
    );


    keyboardLayout->addWidget(
        new QLabel(
            "Keyboard layout and shortcut settings."
        )
    );


    keyboardLayout->addStretch();


    pages->addWidget(
        keyboardPage
    );


    // =========================================================
    // Mouse
    // =========================================================

    auto *mousePage =
        new QWidget();


    auto *mouseLayout =
        new QVBoxLayout(
            mousePage
        );


    mouseLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *mouseTitle =
        new QLabel(
            "Mouse & Touchpad"
        );


    mouseTitle->setObjectName(
        "pageTitle"
    );


    mouseLayout->addWidget(
        mouseTitle
    );


    mouseLayout->addWidget(
        new QLabel(
            "Pointer speed and input-device settings."
        )
    );


    mouseLayout->addStretch();


    pages->addWidget(
        mousePage
    );


    // =========================================================
    // Date & Time
    // =========================================================

    auto *timePage =
        new QWidget();


    auto *timeLayout =
        new QVBoxLayout(
            timePage
        );


    timeLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *timeTitle =
        new QLabel(
            "Date & Time"
        );


    timeTitle->setObjectName(
        "pageTitle"
    );


    timeLayout->addWidget(
        timeTitle
    );


    timeLayout->addWidget(
        new QLabel(
            "Configure the system clock and time zone."
        )
    );


    timeLayout->addStretch();


    pages->addWidget(
        timePage
    );


    // =========================================================
    // Power
    // =========================================================

    auto *powerPage =
        new QWidget();


    auto *powerLayout =
        new QVBoxLayout(
            powerPage
        );


    powerLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *powerTitle =
        new QLabel(
            "Power"
        );


    powerTitle->setObjectName(
        "pageTitle"
    );


    powerLayout->addWidget(
        powerTitle
    );


    powerLayout->addWidget(
        new QLabel(
            "Power management, sleep and screen timeout."
        )
    );


    powerLayout->addStretch();


    pages->addWidget(
        powerPage
    );


    // =========================================================
    // Privacy
    // =========================================================

    auto *privacyPage =
        new QWidget();


    auto *privacyLayout =
        new QVBoxLayout(
            privacyPage
        );


    privacyLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *privacyTitle =
        new QLabel(
            "Privacy & Security"
        );


    privacyTitle->setObjectName(
        "pageTitle"
    );


    privacyLayout->addWidget(
        privacyTitle
    );


    privacyLayout->addWidget(
        new QLabel(
            "Security, permissions and privacy controls."
        )
    );


    privacyLayout->addStretch();


    pages->addWidget(
        privacyPage
    );


    // =========================================================
    // Storage
    // =========================================================

    auto *storagePage =
        new QWidget();


    auto *storageLayout =
        new QVBoxLayout(
            storagePage
        );


    storageLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *storageTitle =
        new QLabel(
            "Storage"
        );


    storageTitle->setObjectName(
        "pageTitle"
    );


    storageLayout->addWidget(
        storageTitle
    );


    storageLayout->addWidget(
        new QLabel(
            "View disks, partitions and available storage."
        )
    );


    storageLayout->addStretch();


    pages->addWidget(
        storagePage
    );


    // =========================================================
    // About
    // =========================================================

    auto *aboutPage =
        new QWidget();


    auto *aboutLayout =
        new QVBoxLayout(
            aboutPage
        );


    aboutLayout->setContentsMargins(
        42,
        36,
        42,
        36
    );


    auto *aboutTitle =
        new QLabel(
            "About BullFrog"
        );


    aboutTitle->setObjectName(
        "pageTitle"
    );


    aboutLayout->addWidget(
        aboutTitle
    );


    auto *logo =
        new QLabel(
            "🐸"
        );


    logo->setStyleSheet(
        "font-size: 72px;"
    );


    aboutLayout->addWidget(
        logo
    );


    auto *version =
        new QLabel(
            "BullFrog OS 0.1.0"
        );


    version->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
    );


    aboutLayout->addWidget(
        version
    );


    aboutLayout->addWidget(
        new QLabel(
            "A Linux desktop environment built with Qt."
        )
    );


    aboutLayout->addWidget(
        new QLabel(
            "Mascot: Garry the Frog"
        )
    );


    aboutLayout->addWidget(
        new QLabel(
            "Kernel: Linux"
        )
    );


    aboutLayout->addStretch();


    pages->addWidget(
        aboutPage
    );


    // =========================================================
    // Final layout
    // =========================================================

    mainLayout->addWidget(
        sidebar
    );


    mainLayout->addWidget(
        pages,
        1
    );


    // =========================================================
    // Start on Display
    // =========================================================

    sidebar->setCurrentRow(
        0
    );


    connect(
        sidebar,
        &QListWidget::currentItemChanged,
        this,
        &SettingsApp::showPage
    );
}


// =============================================================
// Show settings page
// =============================================================

void SettingsApp::showPage(
    QListWidgetItem *current
)
{
    if (!current)
        return;


    const int row =
        sidebar->row(
            current
        );


    if (
        row >= 0 &&
        row < pages->count()
    )
    {
        pages->setCurrentIndex(
            row
        );
    }
}