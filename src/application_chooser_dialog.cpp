// src/application_chooser_dialog.cpp

#include "application_chooser_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileInfo>

ApplicationChooserDialog::ApplicationChooserDialog(const QList<ApplicationInfo>& apps, const QString& currentApp, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle ( tr("Открыть с помощью..."));
    resize (400, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout (this);

    currentAppLabel = new QLabel (this);
    currentAppLabel->setFrameStyle (QFrame::StyledPanel | QFrame::Sunken);
    currentAppLabel->setContentsMargins (5, 5, 5, 5);
    currentAppLabel->setStyleSheet ("padding: 5px;");

    if (currentApp.isEmpty () || currentApp == "xdg-open") {
        currentAppLabel->setText (tr ("<b>Сейчас используется:</b> Системная программа по умолчанию"));
    } else {
        // Ищем имя программы в нашем списке по пути (executable)
        QString appName = QFileInfo (currentApp).fileName ();
        for(const auto& app : apps) {
            if (app.executable == currentApp) { appName = app.name; break; }
        }
        currentAppLabel->setText (tr ("<b>Сейчас используется:</b> %1").arg (appName));
    }
    mainLayout->addWidget (currentAppLabel);

    // Список программ
    listWidget = new QListWidget (this);
    QListWidgetItem* currentItem = nullptr;

    for (const ApplicationInfo& app : apps) {
        QListWidgetItem* item = new QListWidgetItem (app.icon, app.name, listWidget);
        item->setData (Qt::UserRole, app.executable); // Храним исполняемый путь в UserRole

        if (app.executable == currentApp) currentItem = item;
    }
    mainLayout->addWidget (listWidget);

    if (currentItem) {
        listWidget->setCurrentItem (currentItem);
        listWidget->scrollToItem (currentItem);
        selectedExecutable = currentApp;
    }

    okButton = new QPushButton ("Открыть", this);
    okButton->setEnabled (false); // Изначально кнопка неактивна
    QPushButton* cancelButton = new QPushButton ("Отмена", this);

    mainLayout->addWidget (listWidget);
    QHBoxLayout* buttonLayout = new QHBoxLayout ();
    buttonLayout->addWidget (okButton);
    buttonLayout->addWidget (cancelButton);
    mainLayout->addLayout (buttonLayout);

    defaultAppButton = new QPushButton (tr ("Программа по умолчанию (Системная)"), this);
    mainLayout->insertWidget (1, defaultAppButton); // Добавить между списком и кнопками

    connect (defaultAppButton, &QPushButton::clicked, [this]() {
        selectedExecutable = "xdg-open"; // Спец маркер
        accept ();
    });

    connect (listWidget, &QListWidget::itemClicked, this, &ApplicationChooserDialog::onItemSelected);
    connect (listWidget, &QListWidget::itemDoubleClicked, this, &ApplicationChooserDialog::accept); // Двойной клик закрывает диалог
    connect (okButton, &QPushButton::clicked, this, &ApplicationChooserDialog::accept);
    connect (cancelButton, &QPushButton::clicked, this, &ApplicationChooserDialog::reject);
}

void ApplicationChooserDialog::onItemSelected (QListWidgetItem* item) {
    selectedExecutable = item->data (Qt::UserRole).toString ();
    okButton->setEnabled (true);
}

QString ApplicationChooserDialog::getSelectedExecutable () const {
    return selectedExecutable;
}
