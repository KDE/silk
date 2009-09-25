/*
 * WebAppEditor.cpp
 *
 * Copyright (C) 2008 Sebastian Kuegler <sebas@kde.org>
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

WebAppEditor::WebAppEditor(QWidget *widget, const QString &name )
    : KPageWidgetItem(widget, name)
{
    // accept dnd
    //setAcceptDrops(true);

    //setupActions();

    // add a status bar
    //statusBar()->show();

    // a call to KXmlGuiWindow::setupGUI() populates the GUI
    // with actions, using KXMLGUI.
    // It also applies the saved mainwindow settings, if any, and ask the
    // mainwindow to automatically save settings if changed: window size,
    // toolbar position, icon size, etc.
    //setupGUI();
    setupMainWidget();
}

WebAppEditor::~WebAppEditor()
{
}

void WebAppEditor::setupMainWidget()
{


    webAppUi.setupUi(widget());

    connect(webAppUi.saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(webAppUi.allowedBasesLine, SIGNAL(returnPressed()), this, SLOT(addAllowedBase()));

    connect(webAppUi.allowedBases, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));

    // FIXME: temporary ...
    m_actionFile = "/home/sebas/kdesvn/src/project-silk/selkie/services/test/silk-webapp-test.desktop";
    showActionFile();
    kDebug() << "actionFile" << m_actionFile;
}

void WebAppEditor::openActionFile()
{
    m_actionFile = KFileDialog::getOpenFileName(KUrl("file:///home/sebas/kdesvn/src/project-silk/selkie/services/silk"), QString("*.desktop"));
    showActionFile();
}

void WebAppEditor::showActionFile()
{
    //webAppUi.title->setText(i18nc("title widget", "WebApp Action Editor (%1)", KUrl(m_actionFile).fileName()));
    kDebug() << m_actionFile;
    m_desktopFile = new KDesktopFile(m_actionFile);
    KConfigGroup group = m_desktopFile->group("Desktop Entry");

    webAppUi.label->setText(group.readEntry("Name", QString()));
    webAppUi.pluginName->setText(group.readEntry("X-KDE-PluginInfo-Name", QString()));
    webAppUi.icon->setIcon(group.readEntry("Icon", QString()));
    webAppUi.author->setText(group.readEntry("Author", QString()));
    setItems(webAppUi.allowedBases, group.readEntry("X-Silk-AllowedBases", QStringList()));
    setIcon(KIcon(webAppUi.icon->icon()));
    webAppUi.saveButton->setIcon(KIcon("document-save"));

    dump();
}

void WebAppEditor::save()
{
    // ...
    kDebug() << "---> Saving here ...";
    KConfigGroup group = m_desktopFile->group("Desktop Entry");
    group.writeEntry("Icon", webAppUi.icon->icon());
    group.writeEntry("Author", webAppUi.author->text());
    group.writeEntry("Name", webAppUi.label->text());
    group.writeEntry("X-KDE-PluginInfo-Name", webAppUi.pluginName->text());
    m_desktopFile->sync();
    //webAppUi.title->setPixmap(KIcon(webAppUi.icon->icon()));
    //*/
    dump();
}

void WebAppEditor::dump()
{
    kDebug() << "------------------------------";
    kDebug() << "Name" << m_desktopFile->readName() << webAppUi.label->text();
    kDebug() << "Icon" << m_desktopFile->readIcon() << webAppUi.icon->icon();
    kDebug() << "Type" << m_desktopFile->readType();
    kDebug() << "Author" << m_desktopFile->group("Desktop Entry").readEntry("Author", "empty author");
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
