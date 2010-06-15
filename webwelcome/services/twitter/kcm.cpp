/*
    Copyright 2010 by Sebastian Kügler <sebas@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/


#include <QGraphicsView>
#include <QLabel>

#include "kcm.h"

#include <kaboutdata.h>
#include <kglobal.h>
#include <klocale.h>
#include <kdialog.h>

#include <stdio.h>
#include <KDebug>
#include <KPluginFactory>
#include <KPluginLoader>
#include <KWallet/Wallet>

#include <QVBoxLayout>

K_PLUGIN_FACTORY(KcmTwitterFactory,
        registerPlugin<KcmTwitter>();
        )
K_EXPORT_PLUGIN(KcmTwitterFactory("kcmTwitter", "silk_webservices.po"))


KcmTwitter::KcmTwitter(QWidget *parent, const QVariantList &)
  : KCModule(KcmTwitterFactory::componentData(), parent)
{
    //setButtons(Help);
    setAboutData(new KAboutData("kcmtwitter", 0, ki18n("Twitter Setup"), "0.1",
                                ki18n("Configure your system for use with Twitter"),
                                KAboutData::License_GPL, ki18n("Copyright 2010, Sebastian Kügler"),
                                KLocalizedString(), "sebas@kde.org"));

    setQuickHelp( i18n("<h1>Twitter Setup</h1>"
                "This module sets up Twitter on your machine, as a desktop widget (the microblog widget) and full app (choqok).<br />"
                ));
    m_twitterConfig.setupUi(this);
    m_walletFolder = QString("Plasma-Microblog");

    connect(m_twitterConfig.username, SIGNAL(textChanged(const QString&)), this, SLOT(emitChanged()));
    connect(m_twitterConfig.password, SIGNAL(textChanged(const QString&)), this, SLOT(emitChanged()));
}

//---------------------------------------------------------------------------------------------

void KcmTwitter::emitChanged()
{
    emit changed(true);
}

void KcmTwitter::load()
{
    m_username = "sebasje";
    m_wallet = KWallet::Wallet::openWallet(KWallet::Wallet::NetworkWallet(),
                                           winId(), KWallet::Wallet::Asynchronous);
    connect(m_wallet, SIGNAL(walletOpened(bool)), SLOT(readWallet()));
}

void KcmTwitter::loadPlasmoid()
{
    // Load a microblog widget onto the desktop
    // qdbus org.kde.plasma-desktop /MainApplication loadScriptInInteractiveConsole /path/to/file
    kDebug() << "Loading plasmoid....";
}

void KcmTwitter::readWallet()
{
    QString pwd;
    if (!m_wallet->setFolder(m_walletFolder)) {
        kDebug() << "error setting wallet to folder" << m_walletFolder;
    }
    if (m_wallet->readPassword(m_username, pwd) == 0) {
        kDebug() << "got the password from the wallet";
    }
        m_password = pwd;
    //}
    if (m_password.isEmpty()) {
        kDebug() << "password failed reading / empty";
    }
    //kDebug() << "password:" << m_password;
    m_twitterConfig.username->setText(m_username);
    m_twitterConfig.password->setText(m_password);
    delete m_wallet;
    m_wallet = 0;
}


void KcmTwitter::writeWallet()
{
    m_wallet->createFolder(m_walletFolder);
    if (!m_wallet->setFolder(m_walletFolder)) {
        kDebug() << "error setting wallet to folder" << m_walletFolder;
    }
    if ((m_wallet->writePassword(m_username, m_password) == 0)) {
        kDebug() << "successfully wrote password to wallet";
    } else {
        kDebug() << "failed to store password in wallet";
    }
    delete m_wallet;
    m_wallet = 0;

    loadPlasmoid();
}

void KcmTwitter::save()
{
    m_username = m_twitterConfig.username->text();
    m_password = m_twitterConfig.password->text();
    kDebug() << "Saving ..." << m_username << m_password;
    // Write password into the wallet
    m_wallet = KWallet::Wallet::openWallet(KWallet::Wallet::NetworkWallet(),
                                           winId(), KWallet::Wallet::Asynchronous);
    connect(m_wallet, SIGNAL(walletOpened(bool)), SLOT(writeWallet()));
}

void KcmTwitter::defaults()
{
  emit changed(true);
}

//---------------------------------------------------------------------------------------------

#include "kcm.moc"
