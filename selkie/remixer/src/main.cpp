#include "selkieeditor.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("An Editor for Selkie Web Applications");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("webappeditor", 0, ki18n("Selkie Remixer"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2009 Sebastian Kügler"), KLocalizedString(), 0, "sebas@kde.org");
    about.addAuthor( ki18n("Sebastian Kügler"), KLocalizedString(), "sebas@kde.org" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Document to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    SelkieEditor *widget = new SelkieEditor;

    // see if we are starting with session management
    if (app.isSessionRestored())
    {
        //RESTORE(WebAppEditor);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            //webappeditor *widget = new webappeditor;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                //webappeditor *widget = new webappeditor;
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}
