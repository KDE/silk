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
    connect(actionUi.showOnWildcardLine, SIGNAL(returnPressed()), this, SLOT(addShowOnWildcardLine()));
    connect(actionUi.triggerOnWildcardLine, SIGNAL(returnPressed()), this, SLOT(addTriggerOnWildcardLine()));

    connect(actionUi.saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // FIXME: temporary ...
    m_actionFile = "/home/sebas/kdesvn/src/project-silk/selkie/services/test/silk-webapp-test-editor.desktop";
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

void WebAppEditor::addShowOnWildcardLine()
{
    QString text = actionUi.showOnWildcardLine->text();
    actionUi.showOnWildcard->addItem(text);
}

void WebAppEditor::addTriggerOnWildcardLine()
{
    QString text = actionUi.triggerOnWildcardLine->text();
    actionUi.triggerOnWildcard->addItem(text);
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

void WebAppEditor::save()
{
    // ...
    kDebug() << "---> Saving here ...";
    dump();
    KConfigGroup group = m_desktopFile->group("Desktop Entry");
    group.writeEntry("X-Silk-ShowOnUrl", getItems(actionUi.showOnUrl));
    group.writeEntry("X-Silk-TriggerOnUrl", getItems(actionUi.triggerOnUrl));
    group.writeEntry("X-Silk-ShowOnWildcard", getItems(actionUi.showOnWildcard));
    group.writeEntry("X-Silk-TriggerOnWildcard", getItems(actionUi.triggerOnWildcard));
    m_desktopFile->sync();
}

void WebAppEditor::dump()
{
    kDebug() << "------------------------------";
    kDebug() << "Name" << m_desktopFile->readName();
    kDebug() << "Icon" << m_desktopFile->readIcon();
    kDebug() << "Type" << m_desktopFile->readType();
    kDebug() << "shows:" << getItems(actionUi.showOnUrl);
    kDebug() << "triggers:" << getItems(actionUi.triggerOnUrl);
    kDebug() << "shows (wildcards):" << getItems(actionUi.showOnWildcard);
    kDebug() << "triggers (wildcards):" << getItems(actionUi.triggerOnWildcard);
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

    KConfigGroup group = m_desktopFile->group("Desktop Entry");

    setItems(actionUi.showOnUrl, group.readEntry("X-Silk-ShowOnUrl", QStringList()));
    setItems(actionUi.triggerOnUrl, group.readEntry("X-Silk-TriggerOnUrl", QStringList()));
    setItems(actionUi.showOnWildcard, group.readEntry("X-Silk-ShowOnWildcard", QStringList()));
    setItems(actionUi.triggerOnWildcard, group.readEntry("X-Silk-TriggerOnWildcard", QStringList()));

    dump();
}

#include "webappeditor.moc"
