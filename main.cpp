#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

        a.setOrganizationName("HOUSE");
        a.setOrganizationDomain("");
        a.setApplicationName("GiveTR");
        a.setApplicationVersion("v 1.00 r .01");     //  Need to update this when neceassary.

    w.show();
    return a.exec();
}
