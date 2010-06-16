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
#include <KConfig>
#include <kglobal.h>
#include <klocale.h>
#include <kdialog.h>

#include <stdio.h>
#include <KDebug>
#include <KPluginFactory>
#include <KPluginLoader>
#include <KWallet/Wallet>

#include <QVBoxLayout>

K_PLUGIN_FACTORY(KcmWikiFactory,
        registerPlugin<KcmWiki>();
        )
K_EXPORT_PLUGIN(KcmWikiFactory("kcmwikipedia", "silk_webservices.po"))


KcmWiki::KcmWiki(QWidget *parent, const QVariantList &)
  : KCModule(KcmWikiFactory::componentData(), parent)
{
    //setButtons(Help);
    setAboutData(new KAboutData("kcmwiki", 0, ki18n("Wikipedia Setup"), "0.1",
                                ki18n("Configure your system for use with Wikipedia"),
                                KAboutData::License_GPL, ki18n("Copyright 2010, Sebastian Kügler"),
                                KLocalizedString(), "sebas@kde.org"));

    setQuickHelp( i18n("<h1>Wikipedia Setup</h1>"
                "This module sets up Wikipedia on your system.<br />"
                ));
    m_wikiConfig.setupUi(this);
    //connect(m_wikiConfig.username, SIGNAL(textChanged(const QString&)), this, SLOT(emitChanged()));
}

//---------------------------------------------------------------------------------------------

void KcmWiki::emitChanged()
{
    emit changed(true);
}

void KcmWiki::load()
{

    bool m_wikipediaEnabled = false;
    KConfig* wpcfg = new KConfig("krunnerrc");
    KConfigGroup plugins = wpcfg->group("Plugins");
    m_wikipediaEnabled = plugins.readEntry("wikipediaEnabled", true);
    if (!wpcfg->isConfigWritable(false)) {
        kDebug() << "Config immutable, bailing out.";
        return;
    } else {
        kDebug() << "Wiki Enabled?" << m_wikipediaEnabled;
    }
    kDebug() << wpcfg->groupList();
}

void KcmWiki::enableWiki()
{
    kDebug() << "Enabling Wikipedia....";
}

void KcmWiki::save()
{
    load();
}

void KcmWiki::defaults()
{
    emit changed(true);
}

//---------------------------------------------------------------------------------------------

#include "kcm.moc"
