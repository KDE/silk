
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
//---------------------------------------------------------------------------------------------

K_PLUGIN_FACTORY(KcmWikipediaFactory,
        registerPlugin<KcmWikipedia>();
        )
K_EXPORT_PLUGIN(KcmWikipediaFactory("kcmWikipedia"))


KcmWikipedia::KcmWikipedia(QWidget *parent, const QVariantList &)
  : KCModule(KcmWikipediaFactory::componentData(), parent)
{
  setButtons(Help);
  setAboutData(new KAboutData("kcmjoystick", 0, ki18n("Wikipedia Setup"), "0.1",
                               ki18n("Configure your system for use with Wikipedia"),
                               KAboutData::License_GPL, ki18n("(c) 2010, Sebastian Kügler"),
                               KLocalizedString(), "sebas@kde.org"));

  setQuickHelp( i18n("<h1>Wikipedia</h1>"
              "This module sets up the Wikipedia search in Krunner, amongst other.<br />"
              ));

  lbl = new QLabel(this);
  lbl->setText("this thing loaded now. Congrats sebas!!!!!");

  QVBoxLayout *top = new QVBoxLayout(this);
  top->setMargin(0);
  top->setSpacing(KDialog::spacingHint());
  top->addWidget(lbl);
}

//---------------------------------------------------------------------------------------------

void KcmWikipedia::load()
{
  //lbl->init();
}

//---------------------------------------------------------------------------------------------

void KcmWikipedia::defaults()
{
  //lbl->resetCalibration();

  emit changed(true);
}

//---------------------------------------------------------------------------------------------

#include "kcm.moc"
