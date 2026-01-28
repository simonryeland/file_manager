// src/application_finder.cpp

#include "application_finder.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>

QList<ApplicationInfo> ApplicationFinder::findApplications () {
    QList<ApplicationInfo> apps;
     QStringList paths;
     // Стандартные пути для .desktop файлов в Linux
     paths << "/usr/share/applications/" << "/usr/local/share/applications/";
     // Домашняя директория пользователя
     paths << QDir::homePath () + "/.local/share/applications/";
     foreach (const QString &path, paths) {
         QDir dir (path);
         if (dir.exists()) {
             foreach (const QString &fileName, dir.entryList (QStringList ("*.desktop"), QDir::Files)) {
                 ApplicationInfo appInfo = parseDesktopFile (dir.filePath (fileName));
                 if (!appInfo.name.isEmpty () && !appInfo.executable.isEmpty ()) {
                     apps.append (appInfo);
                 }
             }
         }
     }

    // Если нужно добавить сортировку по имени, можно сделать это здесь:
    std::sort (apps.begin (), apps.end (), [](const ApplicationInfo& a, const ApplicationInfo& b) {
        return a.name < b.name;
    });

    return apps;
}

ApplicationInfo ApplicationFinder::parseDesktopFile (const QString &filePath) {
    ApplicationInfo info;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return info;

    QTextStream in (&file);
    while (!in.atEnd ()) {
        QString line = in.readLine ().trimmed ();
        if (line.startsWith ("Name=")) {
            info.name = line.mid (5);
        } else if (line.startsWith ("Exec=")) {
            // Берем только исполняемый файл, отбрасывая аргументы типа %f или %U
            QString execLine = line.mid (5);
            int spaceIndex = execLine.indexOf (' ');
            info.executable = (spaceIndex != -1) ? execLine.left (spaceIndex) : execLine;
        } else         if (line.startsWith ("Icon=")) {
            QString iconName = line.mid (5);
            // QIcon::fromTheme без QIconLoader
            info.icon = QIcon::fromTheme (iconName);

            if (info.icon.isNull ()) { info.icon = QIcon (":/icons/settings.svg"); }
        } else if (line.startsWith ("MimeType=")) {
            info.mimeTypes = line.mid (9);
        }
    }
    file.close ();
    return info;
}
