/*
 * Copyright 2009 Richard Moore <rich@kde.org>
 * Copyright 2009 Omat Holding B.V. <info@omat.nl>
 * Copyright 2009 Sebastian Kügler <sebas@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>

// Qt
#include <QBoxLayout>
#include <QWidget>

// KDE
#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>
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
                     KAboutData::License_GPL, ki18n("(C) 2009 Richard Moore"), KLocalizedString(), 0, "rich@kde.org");
                     about.addAuthor( ki18n("Richard Moore"), KLocalizedString(), "rich@kde.org" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[plugin]", ki18n( "Web application to open" ), "gmail");
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    QWidget *w = new QWidget();
    KToolBar *bar = new KToolBar(w);
    //bar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    kDebug() << "ARGS:" << args << args->count();
    if (args->count() == 0)
    {
        //KCmdLineArgs::usage(i18n("No file specified"));
        //void View::loadWebApp(const QString &name, KPluginInfo::List plugins)
        KPluginInfo::List apps = View::listWebApps();
        if ( !apps.size() ) {
            std::cout << "No applications found" << std::endl;
            return 1;
        }

        std::cout << "Usage: sitespecificbrowser [plugin]" << std::endl << std::endl;
        std::cout << "  Available plugins:" << std::endl;
        foreach (const KPluginInfo &info, apps) {
            QString name = info.pluginName();
            QString comment = info.comment();
            QString applet("      - %1 (%2)");

            applet = applet.arg(name).arg(comment);
            std::cout << applet.toLocal8Bit().data() << std::endl;
        }
        return 1;
        //sitespecificbrowser *widget = new sitespecificbrowser;
        //widget->show();
    } else {
        View tl(w);
        //kDebug() << "ARGS:" << args << args->count() << args->getOption("plugin <pluginname>");

        bool ok = tl.loadWebApp( args->arg(0) );
        if (!ok) {
            std::cout << "Could not find plugin: " << args->arg(0).toLocal8Bit().data() << std::endl;
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
}
