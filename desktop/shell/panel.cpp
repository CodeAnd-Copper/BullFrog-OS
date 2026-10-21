#include "panel.h"

#include "../settings/settings_manager.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QStyle>
Panel::Panel(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(36);

    setObjectName(
        "topPanel"
    );

    // =========================================================
    // Activities button
    // =========================================================

    activitiesButton =
        new QPushButton(
            "🐸 Activities",
            this
        );

    activitiesButton->setToolTip(
        "Open the BullFrog application launcher"
    );

    connect(
        activitiesButton,
        &QPushButton::clicked,
        this,
        &Panel::launcherRequested
    );


    // =========================================================
    // Date
    // =========================================================

    dateLabel =
        new QLabel(
            this
        );


    // =========================================================
    // Clock
    // =========================================================

    clockLabel =
        new QLabel(
            this
        );


    // =========================================================
    // Notifications
    // =========================================================

    notificationButton =
        new QPushButton(
            "🔔",
            this
        );

    notificationButton->setToolTip(
        "Notifications"
    );

    connect(
        notificationButton,
        &QPushButton::clicked,
        this,
        &Panel::notificationsRequested
    );


    // =========================================================
    // Quick settings
    // =========================================================

    quickSettingsButton =
        new QPushButton(
            "⚙",
            this
        );

    quickSettingsButton->setToolTip(
        "Quick settings"
    );

    connect(
        quickSettingsButton,
        &QPushButton::clicked,
        this,
        &Panel::quickSettingsRequested
    );


    // =========================================================
    // Layout
    // =========================================================

    auto *layout =
        new QHBoxLayout(
            this
        );

    layout->setContentsMargins(
        12,
        2,
        12,
        2
    );

    layout->setSpacing(
        8
    );


    auto *spacer =
        new QWidget(
            this
        );

    spacer->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Preferred
    );


    layout->addWidget(
        activitiesButton
    );

    layout->addWidget(
        spacer
    );

    layout->addWidget(
        dateLabel
    );

    layout->addWidget(
        clockLabel
    );

    layout->addWidget(
        notificationButton
    );

    layout->addWidget(
        quickSettingsButton
    );


    // =========================================================
    // Clock timer
    // =========================================================

    clockTimer =
        new QTimer(
            this
        );

    connect(
        clockTimer,
        &QTimer::timeout,
        this,
        &Panel::updateClock
    );

    clockTimer->start(
        1000
    );


    updateClock();


    // =========================================================
    // Apply current theme
    // =========================================================

    checkTheme();
}


// =============================================================
// Theme
// =============================================================

void Panel::checkTheme()
{
    const QString theme =
        SettingsManager::theme();


    const bool lightTheme =
        theme.compare(
            "Light",
            Qt::CaseInsensitive
        ) == 0;


    // =========================================================
    // Light theme
    // =========================================================

    if (lightTheme)
    {
        setStyleSheet(R"(
            QWidget#topPanel {
                background-color: #f5f5f5;
                color: #202124;
                border-bottom: 1px solid #d8d8d8;
            }

            QLabel {
                color: #202124;
                background: transparent;
                font-size: 12px;
            }

            QPushButton {
                background-color: transparent;
                color: #202124;
                border: none;
                border-radius: 6px;
                padding: 5px 9px;
            }

            QPushButton:hover {
                background-color: #dedede;
            }

            QPushButton:pressed {
                background-color: #cecece;
            }
        )");
    }

    // =========================================================
    // Dark theme
    // =========================================================

    else
    {
        setStyleSheet(R"(
            QWidget#topPanel {
                background-color: #202127;
                color: #ffffff;
                border-bottom: 1px solid #303139;
            }

            QLabel {
                color: #ffffff;
                background: transparent;
                font-size: 12px;
            }

            QPushButton {
                background-color: transparent;
                color: #ffffff;
                border: none;
                border-radius: 6px;
                padding: 5px 9px;
            }

            QPushButton:hover {
                background-color: #454650;
            }

            QPushButton:pressed {
                background-color: #565762;
            }
        )");
    }


    // =========================================================
    // Force Qt to recalculate the stylesheet
    // =========================================================

    style()->unpolish(
        this
    );

    style()->polish(
        this
    );

    update();
    repaint();
}


// =============================================================
// Clock
// =============================================================

void Panel::updateClock()
{
    const QDateTime now =
        QDateTime::currentDateTime();


    if (dateLabel)
    {
        dateLabel->setText(
            now.toString(
                "ddd d MMM"
            )
        );
    }


    if (clockLabel)
    {
        clockLabel->setText(
            now.toString(
                "hh:mm"
            )
        );
    }
}