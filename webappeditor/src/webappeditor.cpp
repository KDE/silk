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

#include <KPushButton>

#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>

#include <KDE/KLocale>

WebAppEditor::WebAppEditor()
    : KXmlGuiWindow()
{
    // accept dnd
    setAcceptDrops(true);

    // tell the KXmlGuiWindow that this is indeed the main widget
    //m_widget = new QWidget(this);
    //setCentralWidget(m_widget);

    // then, setup our actions
    setupActions();

    // add a status bar
    statusBar()->show();

    // a call to KXmlGuiWindow::setupGUI() populates the GUI
    // with actions, using KXMLGUI.
    // It also applies the saved mainwindow settings, if any, and ask the
    // mainwindow to automatically save settings if changed: window size,
    // toolbar position, icon size, etc.
    setupGUI();
    setupMainWidget();
}

WebAppEditor::~WebAppEditor()
{
}

void WebAppEditor::setupMainWidget()
{
    m_widget = new QWidget(this);
    actionUi.setupUi(m_widget);

    setCentralWidget(m_widget);

    connect(actionUi.showOnUrlLine, SIGNAL(returnPressed()), this, SLOT(addShowOnUrlLine()));
    connect(actionUi.triggerOnUrlLine, SIGNAL(returnPressed()), this, SLOT(addTriggerOnUrlLine()));

    connect(actionUi.saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // FIXME: temporary ...
    m_actionFile = "/home/sebas/kdesvn/src/project-silk/selkie/services/silk/silk-webapp-silk-urltrigger.desktop";
    showActionFile();
}

void WebAppEditor::addShowOnUrlLine()
{
    QString text = actionUi.showOnUrlLine->text();
    QUrl url(text);
    if (url.isValid()) {
        actionUi.showOnUrl->addItem(text);
    } else {
        kWarning() << "Not a valid URL, won't save it:" << text;
    }
}

void WebAppEditor::addTriggerOnUrlLine()
{
    QString text = actionUi.triggerOnUrlLine->text();
    QUrl url(text);
    if (url.isValid()) {
        actionUi.triggerOnUrl->addItem(text);
    } else {
        kWarning() << "Not a valid URL, won't save it:" << text;
    }
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

void WebAppEditor::save()
{
    // ...
    kDebug() << "---> Saving here ...";
    kDebug() << "shows:" << getItems(actionUi.showOnUrl);
    kDebug() << "triggers:" << getItems(actionUi.triggerOnUrl);
    KConfigGroup group = m_desktopFile->group("Desktop Entry");
    group.writeEntry("X-Silk-ShowOnUrl", getItems(actionUi.showOnUrl));
    group.writeEntry("X-Silk-TriggerOnUrl", getItems(actionUi.triggerOnUrl));
    m_desktopFile->sync();
}

void WebAppEditor::setupActions()
{
    KStandardAction::openNew(this, SLOT(openActionFile()), actionCollection());
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());

    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
}

void WebAppEditor::openActionFile()
{
    m_actionFile = KFileDialog::getOpenFileName(KUrl("file:///home/sebas/kdesvn/src/project-silk/selkie/services/silk"), QString("*.desktop"));
    showActionFile();
}

void WebAppEditor::showActionFile()
{
    //m_fileNameLabel->setText(m_actionFile);
    kDebug() << m_actionFile;
    m_desktopFile = new KDesktopFile(m_actionFile);
    kDebug() << "------------------------------____--";
    kDebug() << "Name" << m_desktopFile->readName();
    kDebug() << "Icon" << m_desktopFile->readIcon();
    kDebug() << "Type" << m_desktopFile->readType();
    KConfigGroup group = m_desktopFile->group("Desktop Entry");

    kDebug() << "showUrl" << group.readEntry("X-Silk-ShowOnUrl");
    QStringList shows = group.readEntry("X-Silk-ShowOnUrl", QStringList());
    foreach (const QString &u, shows) {
        actionUi.showOnUrl->addItem(u);
    }

    kDebug() << "triggerUrl" << group.readEntry("X-Silk-TriggerOnUrl");
    QStringList triggers = group.readEntry("X-Silk-TriggerOnUrl", QStringList());
    foreach (const QString &u, triggers) {
        actionUi.triggerOnUrl->addItem(u);
    }



}

#include "webappeditor.moc"
