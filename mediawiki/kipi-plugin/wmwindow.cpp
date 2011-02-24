/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2011-02-11
 * Description : a kipi plugin to export images to WikiMedia web service
 *
 * Copyright (C) 2011 by Alexandre Mendes <alex dot mendes1988 at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "wmwindow.h"
#include <QLayout>
#include <kdebug.h>
#include <KConfig>
#include <KLocale>
#include <KMenu>
#include <KHelpMenu>
#include <KLineEdit>
#include <KComboBox>
#include <KPushButton>
#include <KMessageBox>
#include <KToolInvocation>
#include "kpaboutdata.h"
#include "wmwidget.h"
#include "wmlogin.h"
#include "wikimediajob.h"
#include "imageslist.h"

using namespace KIPIWikiMediaPlugin;

WMWindow::WMWindow(KIPI::Interface* interface, const QString &tmpFolder,
                  QWidget* /*parent*/)
       : KDialog(0)
{
    m_tmpPath.clear();
    m_tmpDir      = tmpFolder;
    m_interface   = interface;
    m_widget      = new WmWidget(this, interface);

    setMainWidget(m_widget);

    setWindowIcon(KIcon("wikimedia"));
    setButtons(Help|User1|Close);
    setDefaultButton(Close);
    setModal(false);

    setWindowTitle(i18n("Export to Wikimedia Web Service"));
    setButtonGuiItem(User1,
                     KGuiItem(i18n("Start Upload"), "network-workgroup",
                              i18n("Start upload to Wikimedia web service")));
    m_widget->setMinimumSize(700, 500);

    m_about = new KIPIPlugins::KPAboutData(ki18n("Wikimedia Export"), 0,
                          KAboutData::License_GPL,
                          ki18n("A Kipi plugin to export image collection "
                                "to Wikimedia web service."),
                          ki18n("(c) 2011, Alexandre Mendes"));

    m_about->addAuthor(ki18n("Alexandre Mendes"), ki18n("Author"),
                           "alex dot mendes1988 at gmail dot com");

    KHelpMenu* helpMenu = new KHelpMenu(this, m_about, false);
    helpMenu->menu()->removeAction(helpMenu->menu()->actions().first());
    QAction *handbook   = new QAction(i18n("Handbook"), this);
    connect(handbook, SIGNAL(triggered(bool)),
            this, SLOT(slotHelp()));
    helpMenu->menu()->insertAction(helpMenu->menu()->actions().first(), handbook);
    button(Help)->setMenu(helpMenu->menu());

    disconnect(this,SIGNAL(user1Clicked()),this,SLOT(slotStartTransfer()));
    connect(this,SIGNAL(user1Clicked()),this,SLOT(slotStartTransfer()));
    connect(m_widget,SIGNAL(signalChangeUserRequest()), this, SLOT(slotChangeUserClicked()));

    this->reactivate();
}

WMWindow::~WMWindow()
{
    delete m_about;
}

void WMWindow::reactivate()
{
    if (!(m_login.isEmpty() && runLWindow()))
    {
        m_widget->imagesList()->loadImagesFromCurrentSelection();
        show();
    }
}

int WMWindow::runLWindow()
{
    m_dlgLoginExport = new KIPIWikiMediaPlugin::WmLogin(this, i18n("Login"), QString(), QString());

    if (!m_dlgLoginExport)
    {
        kDebug() << " Out of memory error " ;
    }

    if (m_dlgLoginExport->exec() == QDialog::Accepted)
    {
        m_login = m_dlgLoginExport->username();
        m_pass  = m_dlgLoginExport->password();
        m_wiki = m_dlgLoginExport->wiki();
        delete m_dlgLoginExport;
    }
    else
    {
        delete m_dlgLoginExport;
        //Return something which say authentication needed.
        return -1;
    }
    m_mediawiki = new mediawiki::MediaWiki(m_wiki);
    mediawiki::Login login(*m_mediawiki, m_login, m_pass);
    login.exec();
    m_uploadJob = new KIPIWikiMediaPlugin::WikiMediaJob(m_interface,m_login,m_mediawiki,this);
    qDebug()<< login.error();
    if(login.error()){
        m_login.clear();
        m_pass.clear();
        m_uploadJob = NULL;
    }else{
        m_widget->updateLabels(m_login,m_wiki.toString());
    }
    return login.error();
}

void WMWindow::slotHelp()
{
    KToolInvocation::invokeHelp("wikimedia", "kipi-plugins");
}

void WMWindow::slotStartTransfer()
{
    this->m_uploadJob->begin();
}

void WMWindow::slotChangeUserClicked()
{
    if(runLWindow())
        hide();
        //login error
}
