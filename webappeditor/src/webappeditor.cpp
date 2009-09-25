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
    : KPageDialog()
{
    // accept dnd
    setAcceptDrops(true);

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
    m_widget = new QWidget(this);
    actionUi.setupUi(m_widget);

    setCentralWidget(m_widget);

    connect(actionUi.showOnUrlLine, SIGNAL(returnPressed()), this, SLOT(addShowOnUrlLine()));
    connect(actionUi.triggerOnUrlLine, SIGNAL(returnPressed()), this, SLOT(addTriggerOnUrlLine()));
    connect(actionUi.showOnWildcardLine, SIGNAL(returnPressed()), this, SLOT(addShowOnWildcardLine()));
    connect(actionUi.triggerOnWildcardLine, SIGNAL(returnPressed()), this, SLOT(addTriggerOnWildcardLine()));

    connect(actionUi.showOnUrl, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeShowOnUrlItem(QListWidgetItem*)));
    connect(actionUi.triggerOnUrl, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));
    connect(actionUi.showOnWildcard, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));
    connect(actionUi.triggerOnWildcard, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));

    connect(actionUi.saveButton, SIGNAL(clicked()), this, SLOT(save()));

    // FIXME: temporary ...
    m_actionFile = "/home/sebas/kdesvn/src/project-silk/selkie/services/test/silk-webapp-test-editor.desktop";
    showActionFile();
}

void WebAppEditor::removeShowOnUrlItem(QListWidgetItem *item)
{
    kDebug() << "removing item" << item->text();
    actionUi.showOnUrl->removeItemWidget(item);
    actionUi.showOnUrl->removeItemWidget(actionUi.showOnUrl->currentItem());
}

void WebAppEditor::removeItem(QListWidgetItem *item)
{
    KListWidget *lw = qobject_cast<KListWidget*>(sender());
    if (lw) {
        actionUi.showOnUrl->removeItemWidget(item);
    }
}

void WebAppEditor::addShowOnUrlLine()
{
    QString text = actionUi.showOnUrlLine->text();
    QUrl url(text);
    if (url.isValid() && !getItems(actionUi.showOnUrl).contains(text)) {
        actionUi.showOnUrl->addItem(text);
    } else {
        kWarning() << "Not a valid URL, won't save it:" << text;
    }
}

void WebAppEditor::addTriggerOnUrlLine()
{
    QString text = actionUi.triggerOnUrlLine->text();
    QUrl url(text);
    if (url.isValid() && !getItems(actionUi.triggerOnUrl).contains(text)) {
        actionUi.triggerOnUrl->addItem(text);
    } else {
        kWarning() << "Not a valid URL, won't save it:" << text;
    }
}

void WebAppEditor::addShowOnWildcardLine()
{
    QString text = actionUi.showOnWildcardLine->text();
    if (!text.isEmpty() && !getItems(actionUi.showOnWildcard).contains(text)) {
        actionUi.showOnWildcard->addItem(text);
    }
}

void WebAppEditor::addTriggerOnWildcardLine()
{
    QString text = actionUi.triggerOnWildcardLine->text();
    if (!text.isEmpty() && !getItems(actionUi.triggerOnWildcard).contains(text)) {
        actionUi.triggerOnWildcard->addItem(text);
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

void WebAppEditor::setItems(QListWidget *listWidget, const QStringList &list)
{
    foreach (const QString &u, list) {
        listWidget->addItem(u);
    }
}

/*
void WebAppEditor::setupActions()
{
    KStandardAction::open(this, SLOT(openActionFile()), actionCollection());
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());
}
*/
void WebAppEditor::openActionFile()
{
    m_actionFile = KFileDialog::getOpenFileName(KUrl("file:///home/sebas/kdesvn/src/project-silk/selkie/services/silk"), QString("*.desktop"));
    showActionFile();
}

void WebAppEditor::showActionFile()
{
    actionUi.title->setText(i18nc("title widget", "WebApp Action Editor (%1)", KUrl(m_actionFile).fileName()));
    kDebug() << m_actionFile;
    m_desktopFile = new KDesktopFile(m_actionFile);
    KConfigGroup group = m_desktopFile->group("Desktop Entry");

    actionUi.label->setText(group.readEntry("Name", QString()));
    actionUi.pluginName->setText(group.readEntry("X-KDE-PluginInfo-Name", QString()));
    actionUi.icon->setIcon(group.readEntry("Icon", QString()));
    actionUi.author->setText(group.readEntry("Author", QString()));
    setItems(actionUi.showOnUrl, group.readEntry("X-Silk-ShowOnUrl", QStringList()));
    setItems(actionUi.triggerOnUrl, group.readEntry("X-Silk-TriggerOnUrl", QStringList()));
    setItems(actionUi.showOnWildcard, group.readEntry("X-Silk-ShowOnWildcard", QStringList()));
    setItems(actionUi.triggerOnWildcard, group.readEntry("X-Silk-TriggerOnWildcard", QStringList()));
    actionUi.title->setPixmap(KIcon(actionUi.icon->icon()));
    actionUi.saveButton->setIcon(KIcon("document-save"));

    dump();
}

void WebAppEditor::save()
{
    // ...
    kDebug() << "---> Saving here ...";
    KConfigGroup group = m_desktopFile->group("Desktop Entry");
    group.writeEntry("Icon", actionUi.icon->icon());
    group.writeEntry("Author", actionUi.author->text());
    group.writeEntry("Name", actionUi.label->text());
    group.writeEntry("X-KDE-PluginInfo-Name", actionUi.pluginName->text());
    group.writeEntry("X-Silk-ShowOnUrl", getItems(actionUi.showOnUrl));
    group.writeEntry("X-Silk-TriggerOnUrl", getItems(actionUi.triggerOnUrl));
    group.writeEntry("X-Silk-ShowOnWildcard", getItems(actionUi.showOnWildcard));
    group.writeEntry("X-Silk-TriggerOnWildcard", getItems(actionUi.triggerOnWildcard));
    m_desktopFile->sync();
    actionUi.title->setPixmap(KIcon(actionUi.icon->icon()));
    dump();
}

void WebAppEditor::dump()
{
    kDebug() << "------------------------------";
    kDebug() << "Name" << m_desktopFile->readName() << actionUi.label->text();
    kDebug() << "Icon" << m_desktopFile->readIcon() << actionUi.icon->icon();
    kDebug() << "Type" << m_desktopFile->readType();
    kDebug() << "shows:" << getItems(actionUi.showOnUrl);
    kDebug() << "triggers:" << getItems(actionUi.triggerOnUrl);
    kDebug() << "shows (wildcards):" << getItems(actionUi.showOnWildcard);
    kDebug() << "triggers (wildcards):" << getItems(actionUi.triggerOnWildcard);
}


#include "webappeditor.moc"
