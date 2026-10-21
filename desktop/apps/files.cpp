
#include "files.h"

#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QTreeView>
#include <QVBoxLayout>

FilesApp::FilesApp(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("BullFrog Files");
    resize(900, 600);

    buildInterface();
    goToHome();
}

void FilesApp::buildInterface()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #292b35;
            color: white;
            font-family: "Ubuntu", "DejaVu Sans";
        }

        QLineEdit {
            background-color: #383945;
            color: white;
            border: 1px solid #50515e;
            border-radius: 7px;
            padding: 8px;
        }

        QPushButton {
            background-color: #393a47;
            color: white;
            border: none;
            border-radius: 7px;
            padding: 8px 12px;
        }

        QPushButton:hover {
            background-color: #50515f;
        }

        QTreeView {
            background-color: #252630;
            alternate-background-color: #30313c;
            border: 1px solid #454652;
            selection-background-color: #426b55;
            selection-color: white;
            outline: none;
        }

        QHeaderView::section {
            background-color: #353641;
            color: white;
            padding: 7px;
            border: none;
        }
    )");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);
    root->setSpacing(10);

    auto *toolbar = new QHBoxLayout();

    auto *homeButton = new QPushButton("Home", this);
    auto *upButton = new QPushButton("Up", this);

    pathEdit = new QLineEdit(this);
    pathEdit->setPlaceholderText("Enter a folder path...");

    auto *goButton = new QPushButton("Go", this);

    connect(homeButton, &QPushButton::clicked,
            this, &FilesApp::goToHome);

    connect(upButton, &QPushButton::clicked,
            this, &FilesApp::goUp);

    connect(goButton, &QPushButton::clicked,
            this, [this]() {
        navigateToPath(pathEdit->text());
    });

    connect(pathEdit, &QLineEdit::returnPressed,
            this, [this]() {
        navigateToPath(pathEdit->text());
    });

    toolbar->addWidget(homeButton);
    toolbar->addWidget(upButton);
    toolbar->addWidget(pathEdit, 1);
    toolbar->addWidget(goButton);

    root->addLayout(toolbar);

    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::rootPath());
    fileModel->setFilter(
        QDir::AllEntries |
        QDir::NoDotAndDotDot
    );

    fileView = new QTreeView(this);
    fileView->setModel(fileModel);
    fileView->setRootIndex(
        fileModel->index(QDir::homePath())
    );
    fileView->setAlternatingRowColors(true);
    fileView->setSortingEnabled(true);
    fileView->sortByColumn(0, Qt::AscendingOrder);

    fileView->setColumnWidth(0, 350);
    fileView->setColumnWidth(1, 100);
    fileView->setColumnWidth(2, 150);

    connect(fileView, &QTreeView::doubleClicked,
            this, [this](const QModelIndex &index) {
        const QString path = fileModel->filePath(index);
        const QFileInfo info(path);

        if (info.isDir()) {
            navigateToPath(path);
        }
    });

    root->addWidget(fileView, 1);

    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: #b9bac6;");

    root->addWidget(statusLabel);
}

void FilesApp::navigateToPath(const QString &path)
{
    const QString cleanedPath = QDir::cleanPath(
        QDir::fromNativeSeparators(path.trimmed())
    );

    const QFileInfo info(cleanedPath);

    if (!info.exists() || !info.isDir()) {
        statusLabel->setText(
            "That folder does not exist or is inaccessible."
        );
        return;
    }

    const QModelIndex index = fileModel->index(cleanedPath);

    if (!index.isValid()) {
        statusLabel->setText(
            "Could not open that folder."
        );
        return;
    }

    fileView->setRootIndex(index);
    pathEdit->setText(cleanedPath);
    statusLabel->setText(cleanedPath);
}

void FilesApp::goToHome()
{
    navigateToPath(QDir::homePath());
}

void FilesApp::goUp()
{
    const QString currentPath = pathEdit->text();
    const QString parentPath = QFileInfo(currentPath).dir().absolutePath();

    navigateToPath(parentPath);
}

void FilesApp::updatePathLabel()
{
    if (statusLabel && pathEdit)
        statusLabel->setText(pathEdit->text());
}
