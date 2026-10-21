
#include "shell/desktop.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("BullFrog OS");
    QCoreApplication::setApplicationVersion("0.1.0");
    QCoreApplication::setOrganizationName("BullFrog");

    BullFrogDesktop desktop;
    desktop.showFullScreen();

    return app.exec();
}
