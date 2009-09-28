/*
 * webappactioneditor.cpp
 *
 * Copyright (C) 2009 Sebastian Kuegler <sebas@kde.org>
 */
#include "webappactioneditor.h"
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
#include <KService>
#include <KServiceTypeTrader>
#include <KPushButton>
#include <KTextEditor/ConfigInterface>
#include <KTextEditor/Document>
#include <KTextEditor/View>

#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>

#include <KDE/KLocale>

WebAppActionEditor::WebAppActionEditor(KDesktopFile *file)
    : KPageWidgetItem(new QWidget(), QString())
{
    setupMainWidget();
    loadDesktopFile(file);
}

WebAppActionEditor::~WebAppActionEditor()
{
}

void WebAppActionEditor::setupMainWidget()
{
    actionUi.setupUi(widget());


    KService::List offers = KServiceTypeTrader::self()->query("KTextEditor/Document");
    QWidget *w = new QWidget();
    foreach (const KService::Ptr service, offers) {
        m_editorPart = service->createInstance<KTextEditor::Document>(w);
        if (m_editorPart) {
            m_editorPart->setHighlightingMode("JavaScript");

            KTextEditor::View * view = m_editorPart->createView(w);
            view->setContextMenu(view->defaultContextMenu());

            KTextEditor::ConfigInterface *config = qobject_cast<KTextEditor::ConfigInterface*>(view);
            if (config) {
                config->setConfigValue("line-numbers", true);
                config->setConfigValue("dynamic-word-wrap", true);
            }

            actionUi.scriptsTab->layout()->addWidget(view);
            connect(m_editorPart, SIGNAL(textChanged(KTextEditor::Document*)),
                    this, SLOT(scriptTextChanged()));
            break;
        }
    }

    connect(actionUi.showOnUrlLine, SIGNAL(returnPressed()), this, SLOT(addShowOnUrlLine()));
    connect(actionUi.triggerOnUrlLine, SIGNAL(returnPressed()), this, SLOT(addTriggerOnUrlLine()));
    connect(actionUi.showOnWildcardLine, SIGNAL(returnPressed()), this, SLOT(addShowOnWildcardLine()));
    connect(actionUi.triggerOnWildcardLine, SIGNAL(returnPressed()), this, SLOT(addTriggerOnWildcardLine()));

    connect(actionUi.showOnUrl, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));
    connect(actionUi.triggerOnUrl, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));
    connect(actionUi.showOnWildcard, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));
    connect(actionUi.triggerOnWildcard, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItem(QListWidgetItem*)));

    connect(actionUi.saveButton, SIGNAL(clicked()), this, SLOT(save()));

}

void WebAppActionEditor::removeItem(QListWidgetItem *item)
{
    KListWidget *lw = qobject_cast<KListWidget*>(sender());
    if (lw) {
        actionUi.showOnUrl->removeItemWidget(item);
    }
}

void WebAppActionEditor::addShowOnUrlLine()
{
    QString text = actionUi.showOnUrlLine->text();
    QUrl url(text);
    if (url.isValid() && !getItems(actionUi.showOnUrl).contains(text)) {
        actionUi.showOnUrl->addItem(text);
    } else {
        kWarning() << "Not a valid URL, won't save it:" << text;
    }
}

void WebAppActionEditor::addTriggerOnUrlLine()
{
    QString text = actionUi.triggerOnUrlLine->text();
    QUrl url(text);
    if (url.isValid() && !getItems(actionUi.triggerOnUrl).contains(text)) {
        actionUi.triggerOnUrl->addItem(text);
    } else {
        kWarning() << "Not a valid URL, won't save it:" << text;
    }
}

void WebAppActionEditor::addShowOnWildcardLine()
{
    QString text = actionUi.showOnWildcardLine->text();
    if (!text.isEmpty() && !getItems(actionUi.showOnWildcard).contains(text)) {
        actionUi.showOnWildcard->addItem(text);
    }
}

void WebAppActionEditor::addTriggerOnWildcardLine()
{
    QString text = actionUi.triggerOnWildcardLine->text();
    if (!text.isEmpty() && !getItems(actionUi.triggerOnWildcard).contains(text)) {
        actionUi.triggerOnWildcard->addItem(text);
    }
}


QStringList WebAppActionEditor::getItems(QListWidget *listWidget)
{
    QStringList list;
    int rows = listWidget->model()->rowCount();
    for (int i = 0; i < rows; i++) {
        QString text = listWidget->item(i)->text();
        //kDebug() << "Row:" << i << text;
        list << text;
    }
    return list;
}

void WebAppActionEditor::setItems(QListWidget *listWidget, const QStringList &list)
{
    foreach (const QString &u, list) {
        listWidget->addItem(u);
    }
}

/*
void WebAppActionEditor::setupActions()
{
    KStandardAction::open(this, SLOT(openActionFile()), actionCollection());
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());
}
*/
void WebAppActionEditor::openActionFile()
{
    QString filename = KFileDialog::getOpenFileName(KUrl("file:///home/sebas/kdesvn/src/project-silk/selkie/services/silk"), QString("*.desktop"));
    loadDesktopFile(new KDesktopFile(filename));
}

void WebAppActionEditor::loadDesktopFile(KDesktopFile *file)
{
    m_desktopFile = file;
    KConfigGroup group = m_desktopFile->group("Desktop Entry");

    actionUi.label->setText(group.readEntry("Name", QString()));
    actionUi.pluginName->setText(group.readEntry("X-KDE-PluginInfo-Name", QString()));
    actionUi.icon->setIcon(group.readEntry("Icon", QString()));
    actionUi.author->setText(group.readEntry("X-KDE-PluginInfo-Author", QString()));
    setItems(actionUi.showOnUrl, group.readEntry("X-Silk-ShowOnUrl", QStringList()));
    setItems(actionUi.triggerOnUrl, group.readEntry("X-Silk-TriggerOnUrl", QStringList()));
    setItems(actionUi.showOnWildcard, group.readEntry("X-Silk-ShowOnWildcard", QStringList()));
    setItems(actionUi.triggerOnWildcard, group.readEntry("X-Silk-TriggerOnWildcard", QStringList()));
    actionUi.saveButton->setIcon(KIcon("document-save"));

    setIcon(KIcon(actionUi.icon->icon()));
    setName(actionUi.label->text());

    //dump();
}

void WebAppActionEditor::save()
{
    // ...
    kDebug() << "---> Saving here ...";
    KConfigGroup group = m_desktopFile->group("Desktop Entry");
    group.writeEntry("Icon", actionUi.icon->icon());
    group.writeEntry("Name", actionUi.label->text());
    group.writeEntry("X-KDE-PluginInfo-Author", actionUi.author->text());
    group.writeEntry("X-KDE-PluginInfo-Name", actionUi.pluginName->text());
    group.writeEntry("X-Silk-ShowOnUrl", getItems(actionUi.showOnUrl));
    group.writeEntry("X-Silk-TriggerOnUrl", getItems(actionUi.triggerOnUrl));
    group.writeEntry("X-Silk-ShowOnWildcard", getItems(actionUi.showOnWildcard));
    group.writeEntry("X-Silk-TriggerOnWildcard", getItems(actionUi.triggerOnWildcard));
    m_desktopFile->sync();
    setIcon(KIcon(actionUi.icon->icon()));
    //dump();
}

void WebAppActionEditor::dump()
{
    kDebug() << "------------------------------";
    kDebug() << "Name" << m_desktopFile->readName() << actionUi.label->text();
    kDebug() << "Icon" << m_desktopFile->readIcon() << actionUi.icon->icon();
    kDebug() << "Type" << m_desktopFile->readType();
    kDebug() << "Author" << actionUi.author->text();
    kDebug() << "shows:" << getItems(actionUi.showOnUrl);
    kDebug() << "triggers:" << getItems(actionUi.triggerOnUrl);
    kDebug() << "shows (wildcards):" << getItems(actionUi.showOnWildcard);
    kDebug() << "triggers (wildcards):" << getItems(actionUi.triggerOnWildcard);
}


#include "webappactioneditor.moc"
