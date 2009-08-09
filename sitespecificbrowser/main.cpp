
#include <iostream>

// Qt
#include <QBoxLayout>
#include <QWidget>

// KDE
#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KDE/KLocale>
#include <KToolBar>

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
    QWidget *w = new QWidget();
    KToolBar *bar = new KToolBar(w);
    //bar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );


#if 0
    View tl(w); 

    // TODO: Load the application. For now use the hard-coded one.
    tl.setupApplication();

    QAction *action;
    foreach( action, tl.actions() ) {
	bar->addAction( action );
    }
    
    QVBoxLayout *box = new QVBoxLayout(w);
    box->addWidget(bar);
    box->addWidget(&tl);

    tl.startApplication();
    
    w->show();

    return app.exec();
#else
    if (args->count() == 0)
    {
        //void View::loadWebApp(const QString &name, KPluginInfo::List plugins)
	KPluginInfo::List apps = View::listWebApps();
	if ( !apps.size() ) {
	    std::cout << "No applications found" << std::endl;
	    return 1;
	}

	foreach (const KPluginInfo &info, apps) {
            QString name = info.pluginName();
            QString comment = info.comment();
            QString applet("%1 - %2");

            applet = applet.arg(name).arg(comment);
            std::cout << applet.toLocal8Bit().data() << std::endl;
        }
	return 1;
        //sitespecificbrowser *widget = new sitespecificbrowser;
        //widget->show();
    } else {
        View tl(w);

	bool ok = tl.loadWebApp( args->arg(0) );
	if (!ok) {
	    std::cout << "Unable to load application" << args->arg(0).toLocal8Bit().data();
	    return 1;
	}

        QAction *action;
        foreach( action, tl.actions() ) {
            bar->addAction( action );
        }

        QVBoxLayout *box = new QVBoxLayout(w);
        box->addWidget(bar);
        box->addWidget(&tl);

	tl.startApplication();

        w->show();

        args->clear();

        return app.exec();
    }
#endif
}
