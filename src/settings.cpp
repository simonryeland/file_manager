// src/settings.cpp

#include "mainwindow.h"
#include <QSettings>
#include <QLayout>
#include <QDebug>

void MainWindow::saveSettings () {
    //
    QSettings settings ("fm", "proba Qt6 file manager");

    settings.beginGroup ("MainWindow");
    settings.setValue ("size", size ());          // Размер окна
    settings.setValue ("pos", pos ());            // Позиция окна
    settings.setValue ("splitter", splitter->saveState ()); // Состояние сплиттера (ширина панелей)
    settings.setValue ("showHidden", hiddenFilesCheckBox->isChecked ()); // Состояние галочки

    settings.setValue ("sizeIcon", iconSizeValue);
    settings.setValue ("spacing", spacingValue);

    settings.endGroup ();
}

void MainWindow::loadSettings () {
    QSettings settings ("fm", "proba Qt6 file manager");

    settings.beginGroup ("MainWindow");

    // Обновляем размер и позицию (второй параметр — default)
    resize (settings.value ("size", QSize (w, h)).toSize ());
    move (settings.value ("pos", QPoint (200, 200)).toPoint ());

    // Обновляем спрлиттер
    if (settings.contains ("splitter")) {
        splitter->restoreState (settings.value ("splitter").toByteArray ());
    }

    // Обновляем состояние скрытых файлов
    bool showHidden = settings.value ("showHidden", false).toBool ();
    hiddenFilesCheckBox->setChecked (showHidden);
    toggleHiddenFiles (showHidden ? Qt::Checked : Qt::Unchecked);

    int iconSizeV = settings.value ("sizeIcon", 64).toInt ();
    int spacingV = settings.value ("spacing", 128).toInt ();

    settings.endGroup();

    spacingSlider->setValue (spacingV);
    sizeSlider->setValue (iconSizeV);
}
