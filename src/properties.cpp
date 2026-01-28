// src/properties.cpp

#include "mainwindow.h"
#include <string>
#include <cmath>
#include <QMessageBox>
#include <QFileInfo>

sizeInProperties MainWindow::formatSize (long long bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    int unitIndex = 0;
    double doubleSize = static_cast<double>(bytes);

    while (doubleSize >= 1024 && unitIndex < 5) {
        doubleSize /= 1024.0;
        unitIndex++;
    }

    sizeInProperties result;
    result.size = doubleSize;
    result.type = units[unitIndex];
    return result;
}

void MainWindow::showProperties ()
{
    // Определяем текущий выделенный элемент в том представлении, откуда вызвано меню (через отправителя сигнала sender())
    // В контекстном меню проще получить текущий выделенный индекс напрямую
    QModelIndex index;
    // Определяем текущую выделенную ячейку в фокусном представлении
    // currentSelectionModel() вернет модель выделения для того виджета, который сейчас активен
    QItemSelectionModel *selectionModel = nullptr;
    if (listView->hasFocus ()) {
        selectionModel = listView->selectionModel ();
    } else if (treeView->hasFocus ()) {
        selectionModel = treeView->selectionModel ();
    }

    if (selectionModel && selectionModel->currentIndex ().isValid ()) {
         index = selectionModel->currentIndex ();
    } else {
        // Если фокус не определен (например, меню вызвано без выделения), выходим
        return;
    }

    QFileInfo info = model->fileInfo (index);
    sizeInProperties propSize = formatSize (info.size ());
    QFile::Permissions p = info.permissions ();
    QString details;
    details += (p & QFile::ReadUser ? "r" : "-");
    details += (p & QFile::WriteUser ? "w" : "-");
    details += (p & QFile::ExeUser ? "x" : "-");
    QString propertiesText = QString (
        "Имя: %1\nРазмер: %2 %3\nТип: %4\nПрава: %5\nИзменен: %6\nПуть: %7"
    ).arg (info.fileName ())
     .arg (propSize.size, 0, 'f', 2)
     .arg (QString::fromStdString (propSize.type))
     .arg (info.isDir () ? "Директория" : "Файл")
     .arg (details)
     .arg (info.lastModified ().toString ("dd.MM.yyyy hh:mm"))
     .arg (info.absoluteFilePath ());

    QMessageBox::information (this, tr ("Свойства элемента"), propertiesText);
}
