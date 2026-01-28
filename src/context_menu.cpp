// src/context_menu.cpp

#include "mainwindow.h"

void MainWindow::setupContextMenu ()
{
    // Устанавливаем политику контекстного меню для обоих представлений
    treeView->setContextMenuPolicy (Qt::CustomContextMenu);
    listView->setContextMenuPolicy (Qt::CustomContextMenu);

    // Подключаем сигнал customContextMenuRequested к слоту, который покажет меню
    connect (treeView, &QWidget::customContextMenuRequested, this, &MainWindow::onContextMenuRequested);
    connect (listView, &QWidget::customContextMenuRequested, this, &MainWindow::onContextMenuRequested);
}

void MainWindow::onContextMenuRequested (const QPoint &pos)
{
    // Определяем, какой виджет отправил сигнал, и приводим его к типу QWidget
    QWidget* senderWidget = qobject_cast<QWidget*> (sender ());

    if (!senderWidget) return; // Проверка на всякий случай

    // Преобразуем локальные координаты (pos) отправителя в глобальные (экранные)
    QPoint globalPos = senderWidget->mapToGlobal (pos);

    // Создаем контекстное меню, дублируя действия из главного меню Edit
    QMenu contextMenu (tr ("Контекстное меню"), this);

    contextMenu.addAction (copyAction);
    contextMenu.addAction (pasteAction);
    contextMenu.addSeparator ();
    contextMenu.addAction (renameAction);
    contextMenu.addAction (deleteAction);
    contextMenu.addSeparator ();
    contextMenu.addAction (openWithAction);
    contextMenu.addAction (propertiesAction);

    // Показываем меню в нужной позиции
    contextMenu.exec (globalPos);
}

