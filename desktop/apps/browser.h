#ifndef BULLFROG_BROWSER_H
#define BULLFROG_BROWSER_H

#include <QMainWindow>

class QLineEdit;
class QPushButton;
class QWebEngineView;

class BrowserApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrowserApp(QWidget *parent = nullptr);

private:
    void navigateToAddress();
    void updateAddressBar(const QUrl &url);

    QWebEngineView *webView = nullptr;
    QLineEdit *addressBar = nullptr;

    QPushButton *backButton = nullptr;
    QPushButton *forwardButton = nullptr;
    QPushButton *reloadButton = nullptr;
};

#endif