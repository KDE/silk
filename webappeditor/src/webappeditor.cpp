/*
 * WebAppEditor.cpp
 *
 * Copyright (C) 2008 Sebastian Kügler <sebas@kde.org>
 */
#include "webappeditor.h"
#include "settings.h"

#include <QtGui/QDropEvent>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPainter>

#include <kconfigdialog.h>
#include <kstatusbar.h>
#include <KDesktopFile>
#include <KFileDialog>

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
    m_widget = new QWidget(this);
    setCentralWidget(m_widget);

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
    m_layout = new QGridLayout(m_widget);

    m_widget->setLayout(m_layout);
    m_fileNameLabel = new QLabel(m_widget);
    m_layout->addWidget(m_fileNameLabel, 0, 0, 1, 3);

    QLabel *showLabel = new QLabel(m_widget);
    showLabel->setText(i18n("Show on URLs"));
    m_showLine = new QLineEdit(m_widget);
    m_showUrls = new QListWidget(m_widget);


    QLabel *triggerLabel = new QLabel(m_widget);
    triggerLabel->setText(i18n("URL Triggers"));
    m_triggerLine = new QLineEdit(m_widget);
    m_triggerUrls = new QListWidget(m_widget);


    m_layout->addWidget(showLabel, 1, 0, 1, 2);
    m_layout->addWidget(m_showUrls, 2, 1);

    m_layout->addWidget(m_showLine, 1, 1);
    m_layout->addWidget(m_triggerLine, 4, 1);

    m_layout->addWidget(triggerLabel, 4, 0);
    m_layout->addWidget(m_triggerUrls, 5, 1, 1, 2);

    connect(m_showLine, SIGNAL(returnPressed()), this, SLOT(addShowLine()));
    connect(m_triggerLine, SIGNAL(returnPressed()), this, SLOT(addTriggerLine()));
}

void WebAppEditor::addShowLine()
{
    QString text = m_showLine->text();
    QUrl url(text);
    if (url.isValid()) {
        kDebug() << "Aaaaaaaalright.";
        m_showUrls->addItem(text);
    } else {
        kWarning() << "url invalid, sorry.";
    }
}

void WebAppEditor::addTriggerLine()
{
    QString text = m_triggerLine->text();
    QUrl url(text);
    if (url.isValid()) {
        kDebug() << "Aaaaaaaalright.";
        m_triggerUrls->addItem(text);
    } else {
        kWarning() << "url invalid, sorry.";
    }
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
    m_fileNameLabel->setText(m_actionFile);
    m_desktopFile = new KDesktopFile(m_actionFile);
    kDebug() << "------------------------------____--";
    kDebug() << "Name" << m_desktopFile->readName();
    kDebug() << "Icon" << m_desktopFile->readIcon();
    kDebug() << "Type" << m_desktopFile->readType();
    KConfigGroup group = m_desktopFile->group("Desktop Entry");

    kDebug() << "showUrl" << group.readEntry("X-Silk-ShowOnUrl");
    QStringList shows = group.readEntry("X-Silk-ShowOnUrl", QStringList());
    foreach (const QString u, shows) {
        m_showUrls->addItem(u);
    }

    kDebug() << "triggerUrl" << group.readEntry("X-Silk-TriggerOnUrl");
    QStringList triggers = group.readEntry("X-Silk-TriggerOnUrl", QStringList());
    foreach (const QString u, triggers) {
        m_triggerUrls->addItem(u);
    }



}

void WebAppEditor::optionsPreferences()
{
    // The preference dialog is derived from prefs_base.ui
    //
    // compare the names of the widgets in the .ui file
    // to the names of the variables in the .kcfg file
    //avoid to have 2 dialogs shown
    if ( KConfigDialog::showDialog( "settings" ) )  {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
    QWidget *generalSettingsDlg = new QWidget;
    ui_prefs_base.setupUi(generalSettingsDlg);
    dialog->addPage(generalSettingsDlg, i18n("General"), "package_setting");
    //connect(dialog, SIGNAL(settingsChanged(QString)), m_view, SLOT(settingsChanged()));
    dialog->setAttribute( Qt::WA_DeleteOnClose );
    dialog->show();
}

#include "webappeditor.moc"
