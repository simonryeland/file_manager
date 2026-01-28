// include/application_chooser_dialog.h

#ifndef APPLICATIONCHOOSERDIALOG_H
#define APPLICATIONCHOOSERDIALOG_H

#include "application_finder.h"
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

class ApplicationChooserDialog : public QDialog {
    Q_OBJECT

public:
    ApplicationChooserDialog(const QList<ApplicationInfo>& apps, const QString& currentApp, QWidget* parent = nullptr);
    QString getSelectedExecutable () const;

private slots:
    void onItemSelected (QListWidgetItem* item);

private:
    QListWidget* listWidget;
    QPushButton* okButton;
    QString selectedExecutable;
    QPushButton* defaultAppButton;
    QLabel* currentAppLabel;
};

#endif // APPLICATIONCHOOSERDIALOG_H
