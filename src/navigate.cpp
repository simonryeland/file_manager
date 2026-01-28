// src/navigate.cpp

#include "mainwindow.h"

// Вспомогательная функция, которая обновляет вид и историю
void MainWindow::onDirectorySelected (const QModelIndex &index)
{
    // Если индекс не валиден (клик в пустом месте)
    if (!index.isValid ()) {
        // Просто снимаем выделение и выходим.
        // updateStatusBar() уже вызван другим connect'ом и обновил статусбар до "Готов."
        if (sender () == listView) {
            listView->clearSelection ();
        } else if (sender () == treeView) {
            treeView->clearSelection ();
        }
        return;
    }

    // Проверяем, что мы выбрали каталог (на всякий случай)
    if (!model->isDir (index)) return;

    // Если мы не в конце истории, "обрезаем" историю вперед,
    // чтобы начать новую ветку навигации
    while (history.count () > historyIndex + 1) {
        history.removeLast ();
    }

    // Добавляем новый каталог в историю
    history.append (index);
    historyIndex++;

    // Обновляем отображение
    listView->setRootIndex (index);
    // Синхронизируем строку адреса и левую панель
    pathEdit->setText (model->filePath (index));
    treeView->setCurrentIndex (index);

    // Опционально: обновляем состояние кнопок (активны/неактивны)
    backButton->setEnabled (historyIndex > 0);
    nextButton->setEnabled (historyIndex < history.count () - 1);

}

// Кнопка ВВЕРХ
void MainWindow::navigateUp ()
{
    // Получаем индекс родителя текущей отображаемой папки
    QModelIndex currentIndex = listView->rootIndex (); // Берем текущую папку
    QModelIndex parentIndex = model->parent (currentIndex); // Ищем родителя

    if (parentIndex.isValid ()) {
         // Переходим в родительскую папку, используя onDirectorySelected,
         // чтобы добавить этот шаг в историю автоматически
        onDirectorySelected (parentIndex);
    }
}

// Кнопка НАЗАД
void MainWindow::navigateBack ()
{
    // Проверяем, есть ли предыдущий элемент в истории (historyIndex > 0)
    if (historyIndex > 0) {
        historyIndex--; // Двигаемся на шаг назад в истории
        QModelIndex prevIndex = history.at (historyIndex);

        // Обновляем представления без добавления в историю заново
        listView->setRootIndex (prevIndex);
        pathEdit->setText (model->filePath (prevIndex));
        treeView->setCurrentIndex (prevIndex);

        // Обновляем состояние кнопок
        backButton->setEnabled (historyIndex > 0);
        nextButton->setEnabled (historyIndex < history.count () - 1);
    }

    // Если это файл, ничего не происходит (в будущем здесь можно добавить открытие файла)
}

// Кнопка ВПЕРЕД
void MainWindow::navigateNext ()
{
    if (historyIndex < history.count() - 1) {
        historyIndex++; // Двигаемся на шаг вперед в истории
        QModelIndex nextIndex = history.at (historyIndex);

        // Обновляем представления без добавления в историю заново
        listView->setRootIndex (nextIndex);
        pathEdit->setText (model->filePath (nextIndex));
        treeView->setCurrentIndex (nextIndex);

        // Обновляем состояние кнопок
        backButton->setEnabled (historyIndex > 0);
        nextButton->setEnabled (historyIndex < history.count () - 1);
    }
}

