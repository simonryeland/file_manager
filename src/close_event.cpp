// src/close_event.cpp

#include "mainwindow.h"
#include <QCloseEvent>

// Обработчик закрытия окна
void MainWindow::closeEvent (QCloseEvent *event) {
    saveSettings (); // Собирает перед выходом
    event->accept ();
}
