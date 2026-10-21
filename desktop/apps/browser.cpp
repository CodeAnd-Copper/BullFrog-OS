#include "browser.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QToolBar>
#include <QUrl>
#include <QWebEngineView>
#include <QWebEngineHistory>
#include <QWidget>

BrowserApp::BrowserApp(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("BullFrog Browser");
    resize(1100, 720);

    setStyleSheet(R"(
        QMainWindow {
            background-color: #202128;
        }

        QToolBar {
            background-color: #252630;
            border: none;
            spacing: 6px;
            padding: 7px;
        }

        QPushButton {
            background-color: #343640;
            color: white;
            border: none;
            border-radius: 7px;
            padding: 7px 11px;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #474955;
        }

        QPushButton:pressed {
            background-color: #565866;
        }

        QLineEdit {
            background-color: #30313c;
            color: white;
            border: 1px solid #50515d;
            border-radius: 8px;
            padding: 8px 12px;
            selection-background-color: #6da84f;
        }
    )");

    // ---------------------------------------------------------
    // Browser toolbar
    // ---------------------------------------------------------

    auto *toolbar = new QToolBar(this);

    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setIconSize(QSize(18, 18));

    addToolBar(Qt::TopToolBarArea, toolbar);

    backButton = new QPushButton("←", this);
    backButton->setToolTip("Back");

    forwardButton = new QPushButton("→", this);
    forwardButton->setToolTip("Forward");

    reloadButton = new QPushButton("↻", this);
    reloadButton->setToolTip("Reload");

    addressBar = new QLineEdit(this);
    addressBar->setPlaceholderText(
        "Search or enter website address..."
    );

    toolbar->addWidget(backButton);
    toolbar->addWidget(forwardButton);
    toolbar->addWidget(reloadButton);
    toolbar->addSeparator();

    toolbar->addWidget(addressBar);

    // ---------------------------------------------------------
    // Web engine
    // ---------------------------------------------------------

    webView = new QWebEngineView(this);

    setCentralWidget(webView);

    // ---------------------------------------------------------
    // Navigation
    // ---------------------------------------------------------

    connect(
        backButton,
        &QPushButton::clicked,
        webView,
        &QWebEngineView::back
    );

    connect(
        forwardButton,
        &QPushButton::clicked,
        webView,
        &QWebEngineView::forward
    );

    connect(
        reloadButton,
        &QPushButton::clicked,
        webView,
        &QWebEngineView::reload
    );

    connect(
        addressBar,
        &QLineEdit::returnPressed,
        this,
        &BrowserApp::navigateToAddress
    );

    connect(
        webView,
        &QWebEngineView::urlChanged,
        this,
        &BrowserApp::updateAddressBar
    );

    // ---------------------------------------------------------
    // Start page
    // ---------------------------------------------------------

    webView->setUrl(
        QUrl("https://www.google.com")
    );
}

void BrowserApp::navigateToAddress()
{
    QString text = addressBar->text().trimmed();

    if (text.isEmpty())
        return;

    QUrl url;

    // Direct URL
    if (
        text.startsWith("http://") ||
        text.startsWith("https://")
    ) {
        url = QUrl(text);
    }
    else if (
        text.contains('.') &&
        !text.contains(' ')
    ) {
        url = QUrl(
            "https://" + text
        );
    }
    else {
        // Search
        url = QUrl(
            "https://www.google.com/search?q=" +
            QUrl::toPercentEncoding(text)
        );
    }

    webView->setUrl(url);
}

void BrowserApp::updateAddressBar(const QUrl &url)
{
    addressBar->setText(
        url.toString()
    );

    addressBar->setCursorPosition(0);
}