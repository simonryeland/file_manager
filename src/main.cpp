// src/main.cpp

#include <QApplication>
#include "mainwindow.h"

int main (int argc, char **argv)
{
    QApplication a (argc, argv);
    a.setWindowIcon (QIcon::fromTheme ("system-file-manager"));
    MainWindow w;
    w.show ();
    return a.exec ();
}
