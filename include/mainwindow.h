// include/mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "types.h"
#include <QMainWindow>
#include <QStatusBar>
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QCheckBox>
#include <QSlider>
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow (QWidget *parent = nullptr);
    ~MainWindow ();

private slots:
    void updateStatusBar (const QModelIndex &index);     // СЛОТ ДЛЯ ОБНОВЛЕНИЯ СТРОКИ СОСТОЯНИЯ
    void onDirectorySelected (const QModelIndex &index); // Этот слот теперь управляет историей
    void navigateUp ();
    void navigateBack ();
    void navigateNext (); //  СЛОТ ДЛЯ КНОПКИ ВПЕРЕД
    void launchFile (const QModelIndex &index); // слот для запуска файла

    // СЛОТЫ ДЛЯ МЕНЮ
    void newWindow ();
    void createNewFolder ();
    void createNewEmptyDocument ();
    void createMenus ();
    void deleteSelectedItems ();      // Удаление
    void renameSelectedItem ();       // Переименование
    // Копирование/Вставка требуют реализации буфера обмена, пока добавим базовые слоты
    void copySelectedItem ();         // Копировать
    void pasteItems ();               // Вставить
    void showProperties ();           // Показать свойства
    void onContextMenuRequested (const QPoint &pos);
    void setupContextMenu ();
    void openWithDialog (); // слот: Открыть с помощью...

    void helpItem ();
    void aboutItem ();
    void aboutQTItem ();

    void toggleHiddenFiles (int checkState); // слот для переключения скрытых файлов

    void changeIconSize ();    // Слот для размера иконок
    void changeGridSpacing (); // Слот для расстояния между иконками

protected:
    void closeEvent (QCloseEvent *event) override;

private:
    QFileSystemModel *model; // Модель данных файловой системы
    QTreeView *treeView;     // Представление в виде дерева (для левой панели)
    QListView *listView;     // Представление в виде списка (для правой панели)
    QLineEdit *pathEdit;     // Строка адреса
    QPushButton *upButton;   // Кнопка "Наверх"
    QPushButton *backButton; // Кнопка "Обратно"
    QPushButton *nextButton; // КНОПКА ВПЕРЕД
    QSplitter *splitter;     // указатель на сплиттер

    // ИСТОРИЯ НАВИГАЦИИ
    QList<QModelIndex> history;
    int historyIndex; // Текущая позиция в истории

    // ПОЛЯ ДЛЯ МЕНЮ И ДЕЙСТВИЙ
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QAction *newWindowAction;
    QAction *createFolderAction;
    QAction *createDocumentAction;
    QAction *exitAction;

    QAction *deleteAction;
    QAction *renameAction;
    QAction *copyAction;
    QAction *pasteAction;

    QAction *helpAction;
    QAction *aboutAction;
    QAction *aboutQTAction;

    // ПОЛЕ ДЛЯ БУФЕРА ОБМЕНА ФАЙЛОВ
    QStringList fileClipboard;
    bool cutOperation = false; // Флаг: вырезание (перемещение) или копирование?

    QMenu *propertiesMenu; // меню для свойств
    QAction *propertiesAction; // действие для свойств
    QCheckBox *hiddenFilesCheckBox;

    bool copyDirRecursive (const QString &src, const QString &dst);

    QSlider *sizeSlider;
    QSlider *spacingSlider;
    QToolBar *bottomToolBar;

    int iconSizeValue = 64;
    int spacingValue = iconSizeValue;

    void updateIconViewProperties ();

    QAction *openWithAction;

    sizeInProperties formatSize (long long bytes);

    void saveSettings ();
    void loadSettings ();

    int w = 1024;
    int h = 768;
};

#endif // MAINWINDOW_H
