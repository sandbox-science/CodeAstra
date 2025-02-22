#include "../include/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Chris Dedman");
    app.setApplicationName("Code Astra");

    MainWindow window;
    window.show();
    return app.exec();
}
