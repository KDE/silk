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
#include <QGraphicsLinearLayout>
#include <QPixmap>

//KDE
#include <KDebug>
#include <KRun>
#include <KStandardDirs>

//plasma
#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/PushButton>
#include <Plasma/Theme>
#include <Plasma/WebView>

//own
#include "news.h"

using namespace SilkWebWelcome;

News::News(QGraphicsWidget *parent)
    : ServiceContainer(parent)
{
    kDebug() << "Adding News";
    setup();
}

News::~News()
{
}

void News::setup()
{
    kDebug() << "setup of News";
    m_logo = "news-64.png";
    m_smallText = i18nc("quick intro what this button does", "<h2>Follow news feeds</h2>");
    m_fullText = i18nc("the full text explaining what happens here", "<p>add a News widget to your desktop to keep tabs on what's going on in the world.</p> " \
    "<p>By pointing a News widget at multiple feeds, you get a timeline view of different sources. You can also add multiple News widgets to your workspace pointing to separate feeds, or collections of feeds.</p>");
    m_buttonText = i18nc("text on the pushbutton", "Add News Widget");
}

void News::run()
{
    QString js_path = KGlobal::dirs()->findResource("data", QString("plasma-applet-webwelcome/news-add.js"));
    kDebug() << "Script found at:" << js_path;
    emit addApplet("news");
    
    kDebug() << "Adding News Widget!";
}


#include "news.moc"
