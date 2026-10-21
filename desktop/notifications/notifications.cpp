
#include "notifications.h"

#include <QDateTime>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

Notifications::Notifications(QWidget *parent)
    : QWidget(parent, Qt::Tool | Qt::FramelessWindowHint)
{
    setObjectName("bullfrogNotifications");
    setWindowTitle("Notifications");
    setFixedSize(360, 440);

    buildInterface();
    hide();
}

void Notifications::buildInterface()
{
    setStyleSheet(R"(
        QWidget#bullfrogNotifications {
            background-color: #252630;
            color: white;
            border: 1px solid #494a56;
            border-radius: 14px;
        }

        QLabel {
            color: white;
            border: none;
        }

        QPushButton {
            background-color: #393a47;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 8px;
        }

        QPushButton:hover {
            background-color: #50515f;
        }

        QFrame#notificationCard {
            background-color: #353641;
            border: 1px solid #494a56;
            border-radius: 10px;
        }
    )");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(16, 16, 16, 16);
    root->setSpacing(12);

    auto *header = new QHBoxLayout();

    auto *title = new QLabel("Notifications", this);
    title->setStyleSheet(
        "font-size: 20px; font-weight: bold;"
    );

    clearButton = new QPushButton("Clear all", this);

    connect(clearButton, &QPushButton::clicked,
            this, &Notifications::clearNotifications);

    header->addWidget(title);
    header->addStretch();
    header->addWidget(clearButton);

    root->addLayout(header);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; border: none; }"
    );

    auto *content = new QWidget(scrollArea);
    notificationLayout = new QVBoxLayout(content);
    notificationLayout->setContentsMargins(2, 2, 2, 2);
    notificationLayout->setSpacing(10);

    emptyLabel = new QLabel(
        "You're all caught up! 🐸", content
    );

    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setStyleSheet(
        "color: #b9bac6; padding: 30px;"
    );

    notificationLayout->addWidget(emptyLabel);
    notificationLayout->addStretch();

    scrollArea->setWidget(content);
    root->addWidget(scrollArea, 1);
}

void Notifications::togglePanel()
{
    if (isVisible()) {
        hide();
    } else {
        show();
        raise();
        activateWindow();
    }
}

void Notifications::addNotification(
    const QString &title,
    const QString &message)
{
    auto *card = new QFrame(this);
    card->setObjectName("notificationCard");

    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(12, 10, 12, 10);
    layout->setSpacing(5);

    auto *heading = new QLabel(title, card);
    heading->setStyleSheet(
        "font-weight: bold; font-size: 13px;"
    );

    auto *body = new QLabel(message, card);
    body->setWordWrap(true);
    body->setStyleSheet("color: #dedee7;");

    auto *timestamp = new QLabel(
        QDateTime::currentDateTime().toString("hh:mm"), card
    );

    timestamp->setStyleSheet(
        "color: #a9aab8; font-size: 10px;"
    );

    layout->addWidget(heading);
    layout->addWidget(body);
    layout->addWidget(timestamp);

    notificationLayout->insertWidget(
        notificationLayout->count() - 1, card
    );

    ++notificationCount;
    emptyLabel->hide();
}

void Notifications::clearNotifications()
{
    while (notificationLayout->count() > 1) {
        QLayoutItem *item =
            notificationLayout->takeAt(0);

        if (!item)
            break;

        if (QWidget *widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    notificationCount = 0;
    emptyLabel->show();

    emit notificationsCleared();
}
