
#include "terminal.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProcess>
#include <QVBoxLayout>
#include <QDir>
#include <QScrollBar>
#include <QKeyEvent>

TerminalApp::TerminalApp(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("BullFrog Terminal");
    resize(850, 520);

    buildInterface();
    startShell();
}

TerminalApp::~TerminalApp()
{
    if (shellProcess && shellProcess->state() != QProcess::NotRunning) {
        shellProcess->terminate();

        if (!shellProcess->waitForFinished(1000)) {
            shellProcess->kill();
            shellProcess->waitForFinished();
        }
    }
}

void TerminalApp::buildInterface()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #202127;
            color: #eeeeee;
            font-family: "DejaVu Sans Mono", monospace;
        }

        QPlainTextEdit {
            background-color: #17181e;
            color: #e6e6ec;
            border: 1px solid #393a45;
            border-radius: 6px;
            padding: 10px;
            selection-background-color: #426b55;
        }

        QLineEdit {
            background-color: #292b35;
            color: white;
            border: 1px solid #484a57;
            border-radius: 6px;
            padding: 9px;
        }

        QLabel {
            color: #b9bac6;
        }
    )");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(10, 10, 10, 10);
    root->setSpacing(8);

    auto *heading = new QLabel("BullFrog Terminal", this);
    heading->setStyleSheet(
        "font-size: 16px; font-weight: bold;"
    );

    outputView = new QPlainTextEdit(this);
    outputView->setReadOnly(true);
    outputView->setMaximumBlockCount(5000);

    commandInput = new QLineEdit(this);
    commandInput->setPlaceholderText("Enter a shell command...");
    commandInput->setClearButtonEnabled(false);

    statusLabel = new QLabel("Starting shell...", this);

    root->addWidget(heading);
    root->addWidget(outputView, 1);
    root->addWidget(commandInput);
    root->addWidget(statusLabel);

    connect(commandInput, &QLineEdit::returnPressed,
            this, &TerminalApp::executeCommand);
}

void TerminalApp::startShell()
{
    shellProcess = new QProcess(this);

    shellProcess->setWorkingDirectory(QDir::homePath());

    connect(shellProcess, &QProcess::readyReadStandardOutput,
            this, [this]() {
        appendOutput(
            QString::fromLocal8Bit(
                shellProcess->readAllStandardOutput()
            )
        );
    });

    connect(shellProcess, &QProcess::readyReadStandardError,
            this, [this]() {
        appendOutput(
            QString::fromLocal8Bit(
                shellProcess->readAllStandardError()
            )
        );
    });

    connect(shellProcess, &QProcess::errorOccurred,
            this, [this](QProcess::ProcessError) {
        statusLabel->setText(
            "Shell process error: " + shellProcess->errorString()
        );
    });

    connect(shellProcess,
            qOverload<int, QProcess::ExitStatus>(
                &QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus) {
        handleProcessFinished(exitCode);
    });

    shellProcess->start("/bin/bash", {"--noprofile", "--norc", "-i"});

    if (!shellProcess->waitForStarted(2000)) {
        statusLabel->setText(
            "Could not start /bin/bash: " +
            shellProcess->errorString()
        );
        return;
    }

    statusLabel->setText("Shell running");
    appendOutput("BullFrog Terminal\n");
    appendOutput("Type commands below. Use exit to close the shell.\n\n");
}

void TerminalApp::executeCommand()
{
    const QString command = commandInput->text();

    if (command.trimmed().isEmpty())
        return;

    if (!shellProcess ||
        shellProcess->state() != QProcess::Running) {
        statusLabel->setText("Shell is not running.");
        return;
    }

    appendOutput("$ " + command + "\n");

    shellProcess->write(command.toLocal8Bit());
    shellProcess->write("\n");

    commandInput->clear();
}

void TerminalApp::appendOutput(const QString &text)
{
    if (!outputView)
        return;

    outputView->moveCursor(QTextCursor::End);
    outputView->insertPlainText(text);
    outputView->verticalScrollBar()->setValue(
        outputView->verticalScrollBar()->maximum()
    );
}

void TerminalApp::handleProcessFinished(int exitCode)
{
    statusLabel->setText(
        QString("Shell exited with code %1").arg(exitCode)
    );

    appendOutput(
        QString("\n[Shell exited: code %1]\n").arg(exitCode)
    );
}
