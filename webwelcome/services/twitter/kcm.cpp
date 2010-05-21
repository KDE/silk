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


#include <QLabel>

#include "kcm.h"

#include <kaboutdata.h>
#include <kglobal.h>
#include <klocale.h>
#include <kdialog.h>

#include <stdio.h>
#include <KPluginFactory>
#include <KPluginLoader>

#include <QVBoxLayout>

K_PLUGIN_FACTORY(KcmTwitterFactory,
        registerPlugin<KcmTwitter>();
        )
K_EXPORT_PLUGIN(KcmTwitterFactory("kcmTwitter"))


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
  /*
  lbl = new QLabel(this);
  lbl->setText("Twitter config widget loaded. Yay.");

  QVBoxLayout *top = new QVBoxLayout(this);
  top->setMargin(0);
  top->setSpacing(KDialog::spacingHint());
  top->addWidget(lbl);
  */
}

//---------------------------------------------------------------------------------------------

void KcmTwitter::load()
{
  //lbl->init();
}

//---------------------------------------------------------------------------------------------

void KcmTwitter::defaults()
{
  //lbl->resetCalibration();

  emit changed(true);
}

//---------------------------------------------------------------------------------------------

#include "kcm.moc"
