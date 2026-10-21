
#ifndef BULLFROG_TERMINAL_H
#define BULLFROG_TERMINAL_H

#include <QWidget>

class QPlainTextEdit;
class QLineEdit;
class QProcess;
class QLabel;

class TerminalApp : public QWidget
{
    Q_OBJECT

public:
    explicit TerminalApp(QWidget *parent = nullptr);
    ~TerminalApp() override;

private:
    void buildInterface();
    void startShell();
    void executeCommand();
    void appendOutput(const QString &text);
    void handleProcessFinished(int exitCode);

    QPlainTextEdit *outputView = nullptr;
    QLineEdit *commandInput = nullptr;
    QLabel *statusLabel = nullptr;
    QProcess *shellProcess = nullptr;

    QByteArray pendingOutput;
};

#endif
