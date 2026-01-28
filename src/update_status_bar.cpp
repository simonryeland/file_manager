// src/update_status_bar.cpp

#include "mainwindow.h"

void MainWindow::updateStatusBar (const QModelIndex &index)
{
    if (!index.isValid ()) {
        statusBar ()->showMessage ("Готов.");
        return;
    }

    // Получаем информацию о файле/директории по индексу из модели
    QFileInfo info = model->fileInfo (index);
    QString statusMessage;

    if (info.isDir ()) {
        // Используем QDir для прямого подсчета
        QDir dir (info.absoluteFilePath ());
        // Фильтруем, чтобы не считать "." и ".."
        int count = dir.entryList (QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs).count ();

        statusMessage = QString("Директория: %1 | Всего элементов: %2")
                            .arg (info.fileName ())
                            .arg (count);
    } else {
        // Если это файл: имя, размер, дата изменения
        statusMessage = QString ("Файл: %1 | Размер: %2 КБ | Изменен: %3")
                            .arg (info.fileName ())
                            .arg (info.size () / 1024)
                            .arg (info.lastModified ().toString ("dd.MM.yyyy hh:mm"));
    }

    // Обновляем текст в строке состояния
    statusBar ()->showMessage (statusMessage, 0); // Показываем сообщение
}
