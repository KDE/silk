
// Qt
#include <qboxlayout.h>
#include <qtoolbar.h>
#include <qwidget.h>

// KDE
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

// Own
#include "view.h"

static const char description[] =
I18N_NOOP("A KDE 4 Application");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("sitespecificbrowser", 0, ki18n("sitespecificbrowser"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2007 Richard Moore"), KLocalizedString(), 0, "rich@kde.org");
                     about.addAuthor( ki18n("Richard Moore"), KLocalizedString(), "rich@kde.org" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Website to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->count() == 0)
    {
        //sitespecificbrowser *widget = new sitespecificbrowser;
        //widget->show();
    }
    QWidget *w = new QWidget();
    QToolBar *bar = new QToolBar(w);
    bar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    View tl(w);
    QAction *action;
    foreach( action, tl.actions() ) {
        bar->addAction( action );
    }

    QVBoxLayout *box = new QVBoxLayout(w);
    box->addWidget(bar);
    box->addWidget(&tl);

    w->show();

    args->clear();

    return app.exec();
}
