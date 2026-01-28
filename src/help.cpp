// src/help.cpp

#include "mainwindow.h"
#include <QMessageBox>

// РЕАЛИЗАЦИЯ СЛОТА help
void MainWindow::helpItem ()
{
    ;
}

// РЕАЛИЗАЦИЯ СЛОТА about
void MainWindow::aboutItem ()
{
    ;
}

// РЕАЛИЗАЦИЯ СЛОТА aboutQT
void MainWindow::aboutQTItem ()
{
    QMessageBox::aboutQt (this, "Info QT");
}
