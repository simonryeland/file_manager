// src/toggle_hidden_files.cpp

#include "mainwindow.h"

// СЛОТ ПЕРЕКЛЮЧЕНИЯ СКРЫТЫХ ФАЙЛОВ
void MainWindow::toggleHiddenFiles (int checkState)
{
    QDir::Filters filters = model->filter (); // Получаем текущие фильтры
    QDir::Filters newFilters = filters;

    if (checkState == Qt::Checked) {
        // Если флажок установлен, добавляем флаг QDir::Hidden
        newFilters |= QDir::Hidden;
    } else {
        // Если флажок снят, убираем флаг QDir::Hidden
        newFilters &= ~QDir::Hidden;

        // Проверяем, является ли текущая папка скрытой (начинается с точки в Linux)
        QString currentPath = model->filePath (listView->rootIndex ());
        QFileInfo info (currentPath);

        // Если файл/папка скрыт И мы собираемся его скрыть новым фильтром
        if (info.isHidden () && !(newFilters & QDir::Hidden)) {
             // Если текущая папка станет невидимой, сначала переходим на уровень выше
             navigateUp ();
        }
    }

    model->setFilter (newFilters); // Устанавливаем новые фильтры в модель

    // Опционально: можно обновить текущее представление, хотя модель должна сделать это сама
    // onDirectorySelected (listView->rootIndex ());
}
