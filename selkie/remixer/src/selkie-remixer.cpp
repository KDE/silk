/******************************************************************************
*   Copyright 2009-2012 by Sebastian Kügler <sebas@kde.org>                   *
*                                                                             *
*   This library is free software; you can redistribute it and/or             *
*   modify it under the terms of the GNU Library General Public               *
*   License as published by the Free Software Foundation; either              *
*   version 2 of the License, or (at your option) any later version.          *
*                                                                             *
*   This library is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
*   Library General Public License for more details.                          *
*                                                                             *
*   You should have received a copy of the GNU Library General Public License *
*   along with this library; see the file COPYING.LIB.  If not, write to      *
*   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
*   Boston, MA 02110-1301, USA.                                               *
*******************************************************************************/

#include "selkieeditor.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("An Editor for Selkie Web Applications");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("webappeditor", 0, ki18n("Selkie Remixer"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("Copyright 2009-2010 Sebastian Kügler"), KLocalizedString(), 0, "sebas@kde.org");
    about.addAuthor( ki18n("Sebastian Kügler"), KLocalizedString(), "sebas@kde.org" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[path]", ki18n( "Selkie package to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    SelkieEditor *widget = 0;
    // see if we are starting with session management
    if (app.isSessionRestored()) {
        //RESTORE(WebAppEditor);
    } else {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0) {
            widget = new SelkieEditor(QString());
            //webappeditor *widget = new webappeditor;
            widget->show();
        } else {
            kDebug() << "Loading:" << argv[0];
            widget = new SelkieEditor(argv[1]);
            widget->show();
            /*
            int i = 0;
            for (; i < args->count(); i++) {
                //webappeditor *widget = new webappeditor;
            }
            */
        }
        args->clear();
    }

    return app.exec();
}
