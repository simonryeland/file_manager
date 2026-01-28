// src/launch_file.cpp

#include "mainwindow.h"
#include <QSettings>
#include <QProcess>
#include <QDesktopServices>

// РЕАЛИЗАЦИЯ СЛОТА launchFile
void MainWindow::launchFile (const QModelIndex &index)
{
    QFileInfo info = model->fileInfo (index);

    if (info.isDir ()) {
        // Если это директория, просто переходим в нее (как при одиночном клике на левой панели)
        onDirectorySelected (index);
        return;
    }

    QString extension = info.suffix ().toLower ();
    QSettings settings ("fm", "proba Qt6 file manager");

    settings.beginGroup ("FileAssociations");
    QString customApp = settings.value (extension, "").toString();
    settings.endGroup ();

    if (!customApp.isEmpty () && customApp != "xdg-open") {
        QProcess::startDetached (customApp, QStringList () << info.absoluteFilePath ());
    } else {
        QDesktopServices::openUrl (QUrl::fromLocalFile (info.absoluteFilePath ()));
    }
    if (QDesktopServices::openUrl (QUrl::fromLocalFile (info.absoluteFilePath ()))) {
            statusBar ()->showMessage (QString ("Запущен файл: %1").arg (info.fileName ()), 5000);
    } else {
            statusBar ()->showMessage (QString ("Ошибка запуска файла: %1").arg (info.fileName ()), 7000);
    }
}

