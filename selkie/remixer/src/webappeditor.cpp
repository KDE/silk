/*
 * WebAppEditor.cpp
 *
 * Copyright (C) 2008 Sebastian Kügler <sebas@kde.org>
 */
#include "webappeditor.h"
#include "settings.h"

#include <QtGui/QDropEvent>
#include <QtGui/QBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPainter>

#include <kconfigdialog.h>
#include <kstatusbar.h>
#include <KDesktopFile>
#include <KFileDialog>
#include <KPageDialog>
#include <KPushButton>

#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>

#include <KDE/KLocale>

WebAppEditor::WebAppEditor(KDesktopFile *file)
    : KPageWidgetItem(new QWidget(), QString())
{
    setupMainWidget();
    loadDesktopFile(file);
}

WebAppEditor::~WebAppEditor()
{
    delete m_desktopFile;
}

void WebAppEditor::setupMainWidget()
{
    webAppUi.setupUi(widget());

    connect(webAppUi.saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(webAppUi.allowedBasesLine, SIGNAL(returnPressed()), this, SLOT(addAllowedBase()));

    connect(webAppUi.allowedBases, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));
}

void WebAppEditor::openActionFile()
{
    QString filename = KFileDialog::getOpenFileName(KUrl("file:///home/sebas/kdesvn/src/project-silk/selkie/services/silk"), QString("*.desktop"));
    loadDesktopFile(new KDesktopFile(filename));
}

void WebAppEditor::loadDesktopFile(KDesktopFile *file)
{
    m_desktopFile = file;
    KConfigGroup group = m_desktopFile->group("Desktop Entry");

    webAppUi.name->setText(group.readEntry("Name", QString()));
    webAppUi.description->setText(group.readEntry("Comment", QString()));
    webAppUi.author->setText(group.readEntry("X-KDE-PluginInfo-Author", QString()));
    webAppUi.pluginName->setText(group.readEntry("X-KDE-PluginInfo-Name", QString()));
    webAppUi.icon->setIcon(group.readEntry("Icon", QString()));
    webAppUi.startUrl->setText(group.readEntry("X-Silk-StartUrl", QString()));
    setItems(webAppUi.allowedBases, group.readEntry("X-Silk-AllowedBases", QStringList()));
    setIcon(KIcon(webAppUi.icon->icon()));
    setName(webAppUi.name->text());
    webAppUi.saveButton->setIcon(KIcon("document-save"));
    //dump();
}

void WebAppEditor::save()
{
    kDebug() << "---> Saving here ...";
    KConfigGroup group = m_desktopFile->group("Desktop Entry");
    group.writeEntry("Icon", webAppUi.icon->icon());
    group.writeEntry("X-KDE-PluginInfo-Author", webAppUi.author->text());
    //X-KDE-ServiceType=Silk/WebApp
    group.writeEntry("X-KDE-ServiceType", "Silk/WebApp");
    group.writeEntry("Name", webAppUi.name->text());
    group.writeEntry("Comment", webAppUi.description->text());
    group.writeEntry("X-KDE-PluginInfo-Name", webAppUi.pluginName->text());
    group.writeEntry("X-Silk-StartUrl", webAppUi.startUrl->text());
    group.writeEntry("X-Silk-AllowedBases", getItems(webAppUi.allowedBases));

    m_desktopFile->sync();
    //dump();
}

void WebAppEditor::dump()
{
    kDebug() << "------------------------------";
    kDebug() << "Name" << m_desktopFile->readName() << webAppUi.label->text();
    kDebug() << "Icon" << m_desktopFile->readIcon() << webAppUi.icon->icon();
    kDebug() << "Type" << m_desktopFile->readType();
    kDebug() << "Author" << webAppUi.author->text();
}

QStringList WebAppEditor::getItems(QListWidget *listWidget)
{
    QStringList list;
    int rows = listWidget->model()->rowCount();
    for (int i = 0; i < rows; i++) {
        QString text = listWidget->item(i)->text();
        kDebug() << "Row:" << i << text;
        list << text;
    }
    return list;
}

void WebAppEditor::setItems(QListWidget *listWidget, const QStringList &list)
{
    foreach (const QString &u, list) {
        listWidget->addItem(u);
    }
}

void WebAppEditor::removeItem(QListWidgetItem *item)
{
    KListWidget *lw = qobject_cast<KListWidget*>(sender());
    if (lw) {
        webAppUi.allowedBases->removeItemWidget(item);
    }
}

void WebAppEditor::addAllowedBase()
{
    QString text = webAppUi.allowedBasesLine->text();
    QUrl url(text);
    if (url.isValid() && !getItems(webAppUi.allowedBases).contains(text)) {
        webAppUi.allowedBases->addItem(text);
    } else {
        kWarning() << "Not a valid URL, won't save it:" << text;
    }
}

#include "webappeditor.moc"
