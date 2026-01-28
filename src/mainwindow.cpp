// src/mainwindow.cpp

#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QDesktopServices>
#include <QTimer>
#include <QDir> // Добавляем инклуд QDir для подсчета

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
    setWindowTitle ("File Manager (Qt6) with Icons");
    setWindowIcon (QIcon (":/icons/app_icon.png"));
    setMinimumSize (480, 240);
    resize (w, h);

    // Настройка строки состояния (StatusBar)
    // QMainWindow имеет встроенный statusbar(), его не нужно создавать через new
    statusBar ()->showMessage ("Готов.");

    // Инициализация Модели данных Qt
    model = new QFileSystemModel (this);
    model->setRootPath (QDir::homePath ()); // это /home/username/

    historyIndex = -1; // Инициализируем историю

    pathEdit = new QLineEdit (this);
    pathEdit->setToolTip (tr ("Адресная строка"));
    // Настройка Кнопок
    backButton = new QPushButton (QIcon (":/icons/back.png"), "", this); // кнопка Back
    backButton->setToolTip (tr ("Вернуться назад по истории (Alt+Left)"));
    nextButton = new QPushButton (QIcon (":/icons/next.png"), "", this); // кнопка Next
    nextButton->setToolTip (tr ("Перейти вперед по истории (Alt+Right)"));
    upButton = new QPushButton (QIcon (":/icons/up.png"), "", this);     // Кнопка Up
    upButton->setToolTip (tr ("Перейти в родительский каталог (Alt+Up)"));

    // Настройка Представлений (Views)

    treeView = new QTreeView (this);
    treeView->setModel (model);
    // Скрываем ненужные столбцы в левом TreeView, оставляем только имя
    treeView->setHeaderHidden (true);
    treeView->hideColumn (1);
    treeView->hideColumn (2);
    treeView->hideColumn (3);

    // ДОБАВЛЯЕМ ЭТУ НАСТРОЙКУ ДЛЯ ДЕРЕВА
    treeView->setSelectionMode (QAbstractItemView::ExtendedSelection);
    treeView->setSelectionBehavior (QAbstractItemView::SelectItems);
    treeView->setFocusPolicy (Qt::StrongFocus); // Нужно для обработки событий мыши

    listView = new QListView (this);

    listView->setModel (model);
    // Устанавливаем режим иконок для правой панели
    listView->setViewMode (QListView::IconMode);
    listView->setMovement(QListView::Static);
    listView->setIconSize (QSize (iconSizeValue, iconSizeValue)); // Размер иконок
    listView->setGridSize (QSize (spacingValue, spacingValue)); // Расстояние между иконками
    listView->setResizeMode (QListView::Adjust);  // Автоматически пересчитывать расположение при изменении размера
    listView->setWrapping (true); // Разрешить перенос иконок на новую строку
    // listView->setAutoScroll (true);                // Обычно включено по умолчанию

    // Важно для производительности на Linux:
    // Отключаем лишние столбцы, которые ListView не показывает, но модель может обсчитывать
    model->setFilter (QDir::AllEntries | QDir::NoDotAndDotDot);
    // Модель не будет пытаться вычислять тип контента файла глубоко
    model->setResolveSymlinks (false);

    // ДОБАВЛЯЕМ ЭТУ НАСТРОЙКУ ДЛЯ СПИСКА
    listView->setSelectionMode (QAbstractItemView::ExtendedSelection);
    listView->setSelectionBehavior (QAbstractItemView::SelectItems);
    listView->setFocusPolicy (Qt::StrongFocus); // Нужно для обработки событий мыши

    listView->setWordWrap (true); // Разрешить перенос длинных имен файлов
    listView->setTextElideMode (Qt::ElideRight); // Сокращать слишком длинные имена (файл...txt)
    listView->setItemAlignment (Qt::AlignCenter); // Центрировать иконку и текст в ячейке

    // Инициализация CheckBox для скрытых файлов
    hiddenFilesCheckBox = new QCheckBox (tr ("Скрытые файлы"), this);
    // Устанавливаем начальное состояние (по умолчанию скрытые файлы не показываются)
    hiddenFilesCheckBox->setChecked (false);
    hiddenFilesCheckBox->setToolTip (tr ("Включить отображение файлов и папок, начинающихся с точки"));

    // Настройка Макетов (Layouts)

    // Создаем QSplitter и добавляем в него панели
    splitter = new QSplitter (Qt::Horizontal, this);
    splitter->addWidget (treeView);
    splitter->addWidget (listView);

    // ДОБАВЛЯЕМ НИЖНЮЮ ПАНЕЛЬ ИНСТРУМЕНТОВ СО СЛАЙДЕРАМИ
    bottomToolBar = new QToolBar (tr ("Настройки вида"), this);
    bottomToolBar->setToolTip (tr ("Настройки вида"));
    addToolBar (Qt::BottomToolBarArea, bottomToolBar); // Добавляем панель в нижнюю область

    // Добавляем описание к ползункам
    bottomToolBar->addWidget (new QLabel (tr (" Размер иконок: "), this));

    // 1. Ползунок размера иконок
    sizeSlider = new QSlider (Qt::Horizontal, this);
    sizeSlider->setRange (16, 128); // От 16px до 128px
    sizeSlider->setToolTip (tr ("Изменить размер значков от 16 до 128 пикселей"));
    sizeSlider->setValue (iconSizeValue);
    bottomToolBar->addWidget (sizeSlider);

    bottomToolBar->addWidget (new QLabel (tr (" Расстояние: "), this));

    // 2. Ползунок расстояния между иконками (Grid Size)
    spacingSlider = new QSlider (Qt::Horizontal, this);
    spacingSlider->setRange (20, 256);
    spacingSlider->setToolTip (tr ("Изменить расстояние между значками от 20 до 256 пикселей"));
    spacingSlider->setValue (spacingValue);
    bottomToolBar->addWidget (spacingSlider);

    // Устанавливаем пропорции разделения по умолчанию (например, 1 доля левой панели, 3 правой)
    QList<int> sizes;
    sizes << 230 << 600;
    splitter->setSizes (sizes);

    QHBoxLayout *headerLayout = new QHBoxLayout ();
    headerLayout->addWidget (backButton);
    headerLayout->addWidget (nextButton); // Добавляем кнопку Next
    headerLayout->addWidget (upButton);   // Кнопка Up
    headerLayout->addWidget (pathEdit);
    headerLayout->addWidget (hiddenFilesCheckBox); // Добавляем флажок в верхний макет

    // Настройка центрального виджета и компоновки
    // QMainWindow требует установки центрального виджета для размещения основных элементов
    QWidget *centralWidget = new QWidget (this);

    // Создаем главный вертикальный макет окна
    QVBoxLayout *mainVerticalLayout = new QVBoxLayout ();
    mainVerticalLayout->addLayout (headerLayout);
    // Добавляем сплиттер вместо старого горизонтального макета
    mainVerticalLayout->addWidget (splitter);

    // Устанавливаем макет для центрального виджета QWidget
    centralWidget->setLayout (mainVerticalLayout);
    // Устанавливаем QWidget как центральный виджет QMainWindow
    setCentralWidget (centralWidget);

    // СОЗДАНИЕ МЕНЮ
    createMenus ();

    // ВЫЗЫВАЕМ НАСТРОЙКУ КОНТЕКСТНОГО МЕНЮ
    setupContextMenu ();

    // Подключения сигналов и слотов
    connect (treeView, &QTreeView::clicked, this, &MainWindow::updateStatusBar);
    connect (listView, &QListView::clicked, this, &MainWindow::updateStatusBar);

    // Сигнал одиночного клика для навигации по левой панели (обновляет историю)
    connect (treeView, &QTreeView::clicked, this, &MainWindow::onDirectorySelected);

    // Сигнал двойного клика в правой панели вызывает launchFile
    connect (listView, &QListView::doubleClicked, this, &MainWindow::launchFile);
    // Двойной клик в левой панели вызывает launchFile (который обрабатывает и файлы, и папки)
    connect (treeView, &QTreeView::doubleClicked, this, &MainWindow::launchFile);

    connect (upButton, &QPushButton::clicked, this, &MainWindow::navigateUp);
    connect (backButton, &QPushButton::clicked, this, &MainWindow::navigateBack);
    connect (nextButton, &QPushButton::clicked, this, &MainWindow::navigateNext);

    // Используем условную компиляцию на основе версии Qt
    #if (QT_VERSION >= QT_VERSION_CHECK (6, 7, 0))
        // Этот код будет скомпилирован, если версия Qt >= 6.7.0
        connect (hiddenFilesCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::toggleHiddenFiles);
    #else
        // Этот код будет скомпилирован, если версия Qt < 6.7.0 (например, Qt5)
        connect (hiddenFilesCheckBox, &QCheckBox::stateChanged, this, &MainWindow::toggleHiddenFiles);
    #endif

    connect (sizeSlider, &QSlider::valueChanged, this, &MainWindow::changeIconSize);
    connect (spacingSlider, &QSlider::valueChanged, this, &MainWindow::changeGridSpacing);

    connect (pathEdit, &QLineEdit::returnPressed, this, [=]() {
        QString path = pathEdit->text ();
        QDir dir (path);

        if (dir.exists ()) {
            // Получаем индекс пути в модели и переключаем отображение
            QModelIndex index = model->index (path);
            listView->setRootIndex (index);
        }
    });

    // Устанавливаем начальный каталог и добавляем его в историю
    QModelIndex rootIndex = model->index (QDir::homePath ()); // Начинаем из домашней папки
    onDirectorySelected (rootIndex);

    QTimer::singleShot (0, this, [=]() {
        loadSettings (); // Выполнится сразу после того, как конструктор завершит работу
    });
}

MainWindow::~MainWindow () {}

