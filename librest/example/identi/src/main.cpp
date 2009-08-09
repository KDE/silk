#include "../../../librest.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>
#include <KDE/KDebug>

static const char description[] =
    I18N_NOOP("A KDE 4 Application");

static const char version[] = "0.1";

class identi: public QObject {
    Q_OBJECT

    public:
        identi() {
          m_lib = new LibRest();
          m_lib->setBaseUri("http://identi.ca/api/");
          m_lib->start("search.atom?q=KDE");
          connect( m_lib, SIGNAL( finished() ), SLOT( slotFinished() ) );
      }
            
    public slots:
        void slotFinished() { 
          kDebug() << m_lib->data();
        }

    private:
        LibRest* m_lib;
};

int main(int argc, char **argv)
{
    KAboutData about("identi", 0, ki18n("identi"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2009 Omat Holding B.V."), 
                     KLocalizedString(), 0, "info@omat.nl");
    about.addAuthor( ki18n("Omat Holding B.V."), KLocalizedString(), "info@omat.nl" );
    KCmdLineArgs::init(argc, argv, &about);

    KApplication app;
    new identi;

    return app.exec();
}

#include "main.moc"
