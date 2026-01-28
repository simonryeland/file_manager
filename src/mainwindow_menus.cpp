// src/mainwindow_menus.cpp

#include "mainwindow.h"
#include <QApplication> // Для qApp
#include <QMenuBar>
#include <QIcon>

void MainWindow::createMenus ()
{
    // 1. Создаем действия (Actions) с иконками
    newWindowAction = new QAction (QIcon (":/icons/app_icon.png"), tr ("&New Window"), this);
    newWindowAction->setShortcut (QKeySequence (Qt::CTRL | Qt::Key_N));
    newWindowAction->setStatusTip (tr ("Открыть новое окно файлового менеджера"));
    connect (newWindowAction, &QAction::triggered, this, &MainWindow::newWindow);

    createFolderAction = new QAction (QIcon (":/icons/new_folder.png"), tr ("&New Folder..."), this);
    createFolderAction->setShortcut (QKeySequence (Qt::CTRL | Qt::SHIFT | Qt::Key_N));
    createFolderAction->setStatusTip (tr ("Создать новый каталог"));
    connect (createFolderAction, &QAction::triggered, this, &MainWindow::createNewFolder);

    createDocumentAction = new QAction (QIcon (":/icons/new_doc.png"), tr ("New &Empty Document"), this);
    createDocumentAction->setStatusTip (tr ("Создать новый пустой документ"));
    connect (createDocumentAction, &QAction::triggered, this, &MainWindow::createNewEmptyDocument);

    exitAction = new QAction (QIcon (":/icons/exit.png"), tr ("&Exit"), this);
    exitAction->setShortcut (QKeySequence (Qt::CTRL | Qt::Key_Q));
    exitAction->setStatusTip (tr ("Закрытие приложения"));
    connect (exitAction, &QAction::triggered, qApp, &QApplication::quit);  // Подключение к глобальному qApp

    // Добавляем иконки к действиям меню Правка
    deleteAction = new QAction (QIcon (":/icons/delete.png"), tr ("&Delete"), this);
    deleteAction->setShortcut (QKeySequence (Qt::Key_Delete));
    connect (deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedItems);

    renameAction = new QAction (QIcon (":/icons/rename.png"), tr ("&Rename..."), this);
    renameAction->setShortcut (QKeySequence (Qt::Key_F2));
    connect (renameAction, &QAction::triggered, this, &MainWindow::renameSelectedItem);

    copyAction = new QAction (QIcon (":/icons/copy.png"), tr ("&Copy"), this);
    copyAction->setShortcut (QKeySequence (Qt::CTRL | Qt::Key_C));
    connect (copyAction, &QAction::triggered, this, &MainWindow::copySelectedItem);

    pasteAction = new QAction (QIcon (":/icons/paste.png"), tr ("&Paste"), this);
    pasteAction->setShortcut (QKeySequence (Qt::CTRL | Qt::Key_V));
    connect (pasteAction, &QAction::triggered, this, &MainWindow::pasteItems);

    propertiesAction = new QAction (QIcon (":/icons/properties.png"), tr ("&Properties"), this);
    connect (propertiesAction, &QAction::triggered, this, &MainWindow::showProperties);

    openWithAction = new QAction (tr ("Открыть с помощью..."), this);
    connect (openWithAction, &QAction::triggered, this, &MainWindow::openWithDialog);

    helpAction = new QAction (QIcon (":/icons/help.png"), tr ("&Help"), this);
    helpAction->setShortcut (QKeySequence (Qt::Key_F1));
    connect (helpAction, &QAction::triggered, this, &MainWindow::helpItem);

    aboutAction = new QAction (QIcon (":/icons/about.png"), tr ("&About"), this);
    connect (aboutAction, &QAction::triggered, this, &MainWindow::aboutItem);

    aboutQTAction = new QAction (QIcon (":/icons/about.png"), tr ("&About QT"), this);
    connect (aboutQTAction, &QAction::triggered, this, &MainWindow::aboutQTItem);

    // Menus
    fileMenu = menuBar ()->addMenu (tr ("&File"));
    fileMenu->addAction (newWindowAction);
    fileMenu->addSeparator ();
    fileMenu->addAction (createFolderAction);
    fileMenu->addAction (createDocumentAction);
    fileMenu->addSeparator ();
    fileMenu->addAction (exitAction);

    editMenu = menuBar ()->addMenu (tr ("&Edit"));
    // ДОБАВЛЕНИЕ ДЕЙСТВИЙ В МЕНЮ ПРАВКА
    editMenu->addAction (copyAction);
    editMenu->addAction (pasteAction);
    editMenu->addSeparator ();
    editMenu->addAction (renameAction);
    editMenu->addAction (deleteAction);
    editMenu->addSeparator ();
    editMenu->addAction (openWithAction);
    editMenu->addAction (propertiesAction);

    helpMenu = menuBar ()->addMenu (tr ("&Help"));
    helpMenu->addAction (helpAction);
    helpMenu->addAction (aboutAction);
    helpMenu->addSeparator ();
    helpMenu->addAction (aboutQTAction);
}
