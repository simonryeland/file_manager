// src/open_with_dialog.cpp

#include "application_chooser_dialog.h"
#include "application_finder.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QProcess>     // Для запуска системных команд
#include <QFileDialog>
#include <QSettings>
#include <QFileInfo>

// РЕАЛИЗАЦИЯ СЛОТА ОТКРЫТЬ С ПОМОЩЬЮ...
void MainWindow::openWithDialog ()
{
    QModelIndex index = listView->selectionModel ()->currentIndex ();
    if (!index.isValid ()) return;

    QFileInfo fileInfo = model->fileInfo (index);
    if (!fileInfo.isFile ()) return;

    QString extension = fileInfo.suffix ().toLower ();
    QSettings settings ("fm", "proba Qt6 file manager");

    settings.beginGroup ("FileAssociations");
    QString lastUsedApp = settings.value (extension, "").toString();
    settings.endGroup ();

    //
    QList<ApplicationInfo> apps = ApplicationFinder::findApplications ();

    ApplicationChooserDialog dialog (apps, lastUsedApp, this);

    if (!lastUsedApp.isEmpty ()) {
        dialog.setWindowTitle (tr ("Открыть в %1 или другой...").arg (QFileInfo (lastUsedApp).fileName ()));
    }

    if (dialog.exec () == QDialog::Accepted) {
        QString programPath = dialog.getSelectedExecutable ();

        if (!programPath.isEmpty ()) {
            settings.beginGroup ("FileAssociations");
            settings.setValue (extension, programPath);
            settings.endGroup ();

            // 3. Запускаем выбранную программу
            QStringList arguments;
            arguments << fileInfo.absoluteFilePath();
            if (QProcess::startDetached (programPath, arguments)) {
                statusBar ()->showMessage (tr ("Запущено через: %1").arg (programPath), 7000);
            } else {
                QMessageBox::warning (this, tr ("Ошибка запуска"), tr ("Не удалось запустить выбранную программу."));
            }
        }
    }
}

