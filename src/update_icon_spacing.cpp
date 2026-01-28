// src/update_icon_spacing.cpp

#include "mainwindow.h"

// Вспомогательный метод для обновления вида на основе обоих ползунков
void MainWindow::updateIconViewProperties ()
{
    // Получаем текущие значения с ползунков
    iconSizeValue = sizeSlider->value ();
    spacingValue = spacingSlider->value ();

    // Устанавливаем размер иконки (квадратный)
    QSize iconSize (iconSizeValue, iconSizeValue);
    listView->setIconSize (iconSize);

    // Устанавливаем размер сетки (добавляем отступ к размеру иконки)
    // Например: Icon 48px + Spacing 64px = Grid 112x112px
    QSize gridSize (iconSizeValue + spacingValue, iconSizeValue + spacingValue);
    listView->setGridSize (gridSize);
    listView->doItemsLayout ();
}

// СЛОТ ИЗМЕНЕНИЯ РАЗМЕРА ИКОНОК
void MainWindow::changeIconSize ()
{
    // Вместо прямой установки, вызываем функцию синхронизации
    updateIconViewProperties ();
}

// СЛОТ ИЗМЕНЕНИЯ РАССТОЯНИЯ (GRID SPACING)
void MainWindow::changeGridSpacing ()
{
    // Вместо прямой установки, вызываем функцию синхронизации
    updateIconViewProperties ();
}

