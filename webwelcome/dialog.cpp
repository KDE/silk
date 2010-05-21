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
    setMinimumSize(500, 300);
    buildDialog();
}

Dialog::~Dialog()
{
}

void Dialog::buildDialog()
{
    kDebug() << " =========================================== ";
    QGraphicsGridLayout *gridLayout = new QGraphicsGridLayout(this);
    setLayout(gridLayout);

    m_styleSheet = new StyleSheet(QString(), this);

    m_dashboard = new Plasma::WebView(this);
    QString html = QString("<style>\n%1\n</style>\n").arg(m_styleSheet->styleSheet());
    html.append("<h1>Welcome to your Desktop</h1>\n");
    html.append("<p>Please take some minutes to set up your online services on this system.\n");
    html.append("Your passwords will be safely stored in the KWallet secure storage.</p>");
    m_dashboard->setHtml(html);
    kDebug() << html;

    gridLayout->addItem(m_dashboard, 0, 0, 1, 3); // top cell, spanning 3 columns

    //QString img_path = "/home/sebas/kdesvn/src/project-silk/webwelcome/images/";

    //QStringList _b;
    //_b << "gmail.png" << "twitter.png" << "wikipedia.png";

    ServiceButton* b = new ServiceButton(this);
    b->setPixmap("gmail.png");
    m_buttons << b;
    gridLayout->addItem(b, 1, 0);

    ServiceButton* g = new ServiceButton(this);
    g->setPixmap("twitter.png");
    m_buttons << g;
    gridLayout->addItem(g, 1, 1);

    ServiceButton* w = new ServiceButton(this);
    w->setPixmap("wikipedia.png");
    m_buttons << w;
    gridLayout->addItem(w, 2, 0);

    ServiceButton* f = new ServiceButton(this);
    f->setPixmap("facebook.png");
    m_buttons << f;
    gridLayout->addItem(f, 2, 1);
}

#include "dialog.moc"
