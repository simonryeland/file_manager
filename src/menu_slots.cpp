// src/menu_slots.cpp

#include "mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QInputDialog> // Для диалогов ввода имени
#include <QDirIterator> // Для итерации по содержимому директорий
// #include <QClipboard>   // Для системного буфера обмена, если понадобится

void MainWindow::newWindow ()
{
    // Создаем новое окно и показываем его
    MainWindow *newWin = new MainWindow ();
    newWin->show ();
    // Так как у newWin родитель не указан, оно удалится при закрытии автоматически (Qt::WA_DeleteOnClose по умолчанию)
}

void MainWindow::createNewFolder ()
{
    // Определяем текущий путь
    QString currentPath = model->filePath (listView->rootIndex ());
    bool ok;
    // Запрашиваем имя новой папки у пользователя
    QString folderName = QInputDialog::getText (this, tr ("Создать папку"),
                                               tr ("Имя новой папки:"), QLineEdit::Normal,
                                               tr ("Новая папка"), &ok);
    if (ok && !folderName.isEmpty ()) {
        QDir dir (currentPath);
        if (dir.mkdir (folderName)) {
            statusBar ()->showMessage (tr ("Папка успешно создана."), 2000);
        } else {
            QMessageBox::warning (this, tr ("Ошибка"), tr ("Не удалось создать папку."));
        }
    }
}

void MainWindow::createNewEmptyDocument ()
{
    // Определяем текущий путь
    QString currentPath = model->filePath (listView->rootIndex ());
    bool ok;
    // Запрашиваем имя нового файла у пользователя
    QString fileName = QInputDialog::getText(this, tr ("Создать документ"),
                                             tr ("Имя нового файла:"), QLineEdit::Normal,
                                             tr ("Новый файл.txt"), &ok);
    if (ok && !fileName.isEmpty ()) {
        QFile file (QDir (currentPath).filePath (fileName));
        if (file.open (QIODevice::WriteOnly)) {
            file.close ();
            statusBar ()->showMessage (tr ("Файл успешно создан."), 2000);
        } else {
            QMessageBox::warning (this, tr ("Ошибка"), tr ("Не удалось создать файл."));
        }
    }
}

void MainWindow::deleteSelectedItems ()
{
    // Получаем текущий отображаемый путь
    QString currentViewPath = model->filePath (listView->rootIndex ());
    QModelIndexList selection = listView->selectionModel ()->selectedIndexes ();

    if (selection.isEmpty ()) {
        QMessageBox::information (this, tr ("Удаление"), tr ("Пожалуйста, выберите элементы для удаления."));
        return;
    }

    if (QMessageBox::question (this, tr("Подтверждение удаления"),
                              tr ("Вы уверены, что хотите удалить выбранные элементы?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }

    foreach (const QModelIndex &index, selection) {
        QString itemPath = model->filePath (index);

        // !!! КРИТИЧЕСКАЯ ПРОВЕРКА !!!
        // Если удаляемый элемент совпадает с текущим отображаемым путем
        if (itemPath == currentViewPath) {
            // Сначала переходим на уровень выше
            navigateUp ();
            // Это обновит listView->rootIndex () и currentViewPath
        }

        // QFileSystemModel имеет удобный метод для удаления файла/директории
        if (model->remove (index)) {
            statusBar ()->showMessage (tr ("Элемент удален: ") + model->fileName (index), 2000);
        } else {
            QMessageBox::warning (this, tr ("Ошибка"), tr ("Не удалось удалить элемент: ") + model->fileName (index) + tr (" (Возможно, нет прав или папка не пуста)"));
        }
    }

    // После удаления обновляем вид, если нужно (onDirectorySelected делает это)
    // onDirectorySelected (listView->rootIndex ());
}

void MainWindow::renameSelectedItem ()
{
    // Получаем выделенный элемент (работаем только с одним)
    QModelIndex index = listView->selectionModel ()->currentIndex ();
    if (!index.isValid ()) return;

    bool ok;
    QString oldName = model->fileName (index);
    QString newName = QInputDialog::getText (this, tr ("Переименовать"),
                                            tr ("Новое имя:"), QLineEdit::Normal,
                                            oldName, &ok);

    if (ok && !newName.isEmpty () && newName != oldName) {
        // QFileSystemModel::setNameFilter работает, но не переименовывает
        // Нужно использовать QDir::rename
        QDir dir(model->filePath (index));
        // Поднимаемся на уровень выше, чтобы получить базовый путь для rename
        dir.cdUp ();

        if (dir.rename (oldName, newName)) {
            statusBar ()->showMessage (tr ("Элемент переименован."), 2000);
        } else {
            QMessageBox::warning (this, tr ("Ошибка"), tr ("Не удалось переименовать элемент."));
        }
    }
}

void MainWindow::copySelectedItem ()
{
    QModelIndexList selection = listView->selectionModel ()->selectedIndexes ();
    fileClipboard.clear ();
    cutOperation = false;

    foreach (const QModelIndex &index, selection) {
        // Добавляем абсолютные пути выбранных элементов в наш буфер обмена
        fileClipboard.append (model->filePath (index));
    }
    statusBar ()->showMessage (tr ("Элементы скопированы: ") + QString::number (fileClipboard.count ()), 2000);
}

// РЕАЛИЗАЦИЯ ВСПОМОГАТЕЛЬНОГО РЕКУРСИВНОГО МЕТОДА
bool MainWindow::copyDirRecursive (const QString &src, const QString &dst)
{
    QDir srcDir (src);
    if (!srcDir.exists ()) {
        return false;
    }

    // Создаем целевую директорию, если её нет
    if (!QDir (dst).mkpath (dst)) {
        return false;
    }

    // Используем QDirIterator для обхода всех файлов и папок (рекурсивно не нужно, итерируем только текущий уровень)
    QDirIterator it (src, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::NoIteratorFlags);
    bool success = true;

    while (it.hasNext ()) {
        it.next ();
        QString srcItem = it.filePath ();
        QString dstItem = dst + QDir::separator () + it.fileName ();
        QFileInfo info (srcItem);

        if (info.isDir ()) {
            // Если это директория, вызываем функцию рекурсивно
            if (!copyDirRecursive (srcItem, dstItem)) {
                success = false;
            }
        } else if (info.isFile ()) {
            // Если это файл, просто копируем его
            if (!QFile::copy (srcItem, dstItem)) {
                success = false;
            }
        }
    }
    return success;
}

void MainWindow::pasteItems ()
{
    if (fileClipboard.isEmpty ()) return;

    QString destinationPath = model->filePath (listView->rootIndex ());
    bool success = true;

    foreach (const QString &sourcePath, fileClipboard) {
        QFileInfo info (sourcePath);
        QString destFilePath = QDir (destinationPath).filePath (info.fileName ());

        if (cutOperation) {
            // --- Перемещение (Вырезание) ---
            if (info.isDir ()) {
                // Для директорий используем QDir::rename (работает только если целевой путь на том же диске)
                 QDir sourceDir (sourcePath);
                 if (!sourceDir.rename (sourcePath, destFilePath)) {
                    // Если rename не сработал (например, разные диски), пробуем копирование+удаление
                    if (copyDirRecursive (sourcePath, destFilePath)) {
                        QDir ().rmdir (sourcePath); // Удаляем исходную пустую папку
                        // Удаление содержимого старой папки нужно реализовать отдельно через QDirIterator
                    } else {
                         success = false;
                         break;
                    }
                 }
            } else {
                 if (!QFile::rename (sourcePath, destFilePath)) {
                    success = false;
                    break;
                 }
            }
        } else {
            // --- Копирование ---
            if (info.isDir ()) {
                // Для директорий вызываем нашу рекурсивную функцию
                if (!copyDirRecursive (sourcePath, destFilePath)) {
                    success = false;
                    break;
                }
            } else if (info.isFile ()) {
                // Для файлов используем QFile::copy
                if (!QFile::copy (sourcePath, destFilePath)) {
                    success = false;
                    break;
                }
            }
        }
    }

    if (success) {
        statusBar ()->showMessage (tr ("Вставка завершена."), 2000);
        if (cutOperation) {
            fileClipboard.clear ();
            cutOperation = false;
        }
        // Обновляем текущее представление
        onDirectorySelected (listView->rootIndex ());
    } else {
        QMessageBox::warning (this, tr ("Ошибка вставки"), tr ("Не удалось вставить некоторые элементы."));
    }
}
