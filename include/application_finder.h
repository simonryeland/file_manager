// include/application_finder.h

#ifndef APPLICATIONFINDER_H
#define APPLICATIONFINDER_H

#include <QString>
#include <QList>
#include <QIcon>
#include <QDir>

// Структура для хранения информации о приложении
struct ApplicationInfo {
    QString name;
    QString executable; // Путь к исполняемому файлу
    QIcon icon;
    QString mimeTypes;
};

class ApplicationFinder {
public:
    static QList<ApplicationInfo> findApplications ();
private:
    static ApplicationInfo parseDesktopFile (const QString &filePath);
};

#endif // APPLICATIONFINDER_H
