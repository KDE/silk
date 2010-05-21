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
//Qt
#include <QGraphicsGridLayout>
#include <QLabel>
//#include <QVBoxLayout>
//#include <QTimer>
//#include <QWebFrame>

//KDE
#include <KDebug>
#include <KIcon>
#include <KIconLoader>
#include <KRun>
#include <KServiceTypeTrader>
#include <KStandardDirs>

//plasma
#include <Plasma/Label>
#include <Plasma/Dialog>
#include <Plasma/Theme>


//own
#include "dialog.h"
//#include "servicebutton.h"
#include "stylesheet.h"

using namespace SilkWebWelcome;

Dialog::Dialog(QGraphicsWidget *parent)
    : QGraphicsWidget(parent)
{
    setMinimumSize(400, 300);
    buildDialog();
}

Dialog::~Dialog()
{
}

void Dialog::buildDialog()
{
    m_gridLayout = new QGraphicsGridLayout(this);
    setLayout(m_gridLayout);

    m_styleSheet = new StyleSheet(QString(), this);

    m_dashboard = new Plasma::WebView(this);
    QString html = QString("<style>\n%1\n</style>\n").arg(m_styleSheet->styleSheet());
    html.append("<h1>Welcome to your Desktop</h1>\n");
    html.append("<p>Please take some minutes to set up your online services on this system.\n");
    html.append("Your passwords will be safely stored in the KWallet secure storage.</p>");
    m_dashboard->setHtml(html);
    m_dashboard->setMinimumHeight(145);
    m_dashboard->setMaximumHeight(145);
    m_gridLayout->addItem(m_dashboard, 0, 0, 1, 2); // top cell, spanning 2 columns

    loadServices();
}

void Dialog::loadServices()
{
    kDebug() << "Looking for webservice plugins...";
    KService::List offers = KServiceTypeTrader::self()->query("Silk/WebService");

    int row = 1;
    int col = 0;

    foreach (const KSharedPtr<KService> s, offers) {
        // Create plugininfo, feed that to the button to load it up
        KPluginInfo* info = new KPluginInfo(s);
        ServiceButton* button = new ServiceButton(this);
        button->load(info);

        // And sort our button into the layout, into two columns and as many rows as we need
        m_gridLayout->addItem(button, row, col);
        kDebug() << "Added:" << info->pluginName() << row << col;
        if (col == 1) {
            row++;
            col = 0;
        } else {
            col++;
        }
    }
    kDebug() << "Done, found" << offers.count();
}

#include "dialog.moc"
