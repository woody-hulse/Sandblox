#include "mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <iostream>
#include <QSettings>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Sandblox");
    QCoreApplication::setOrganizationName("CS 1230");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QSurfaceFormat fmt;
    fmt.setVersion(4, 1);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow w;
    w.initialize();
    w.resize(3000, 3000);
    w.showFullScreen();
    w.show();

    int return_val = a.exec();
    w.finish();
    return return_val;
}
