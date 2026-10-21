
#ifndef BULLFROG_FILES_H
#define BULLFROG_FILES_H

#include <QWidget>
#include <QString>

class QFileSystemModel;
class QTreeView;
class QLineEdit;
class QPushButton;
class QLabel;

class FilesApp : public QWidget
{
    Q_OBJECT

public:
    explicit FilesApp(QWidget *parent = nullptr);

private:
    void buildInterface();
    void navigateToPath(const QString &path);
    void goToHome();
    void goUp();
    void updatePathLabel();

    QFileSystemModel *fileModel = nullptr;
    QTreeView *fileView = nullptr;
    QLineEdit *pathEdit = nullptr;
    QLabel *statusLabel = nullptr;
};

#endif
