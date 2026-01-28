TARGET = fm
CONFIG += console widgets
QT = widgets core gui

HEADERS += include/mainwindow.h \
           include/application_finder.h \
           include/application_chooser_dialog.h \
           include/types.h

SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/menu_slots.cpp \
           src/update_status_bar.cpp \
           src/toggle_hidden_files.cpp \
           src/close_event.cpp \
           src/mainwindow_menus.cpp \
           src/context_menu.cpp \
           src/application_finder.cpp \
           src/application_chooser_dialog.cpp \
           src/help.cpp \
           src/launch_file.cpp \
           src/open_with_dialog.cpp \
           src/navigate.cpp \
           src/properties.cpp \
           src/settings.cpp \
           src/update_icon_spacing.cpp

INCLUDEPATH += include/

RESOURCES += rc/icons.qrc

MOC_DIR = build_tmp/moc
OBJECTS_DIR = build_tmp/obj
DESTDIR = .
