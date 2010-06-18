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
#include "twitter.h"

using namespace SilkWebWelcome;

Twitter::Twitter(QGraphicsWidget *parent)
    : ServiceContainer(parent)
{
    //ServiceContainer(parent);
    kDebug() << "Adding Twitter";
    //ServiceContainer::setup();
    setup();
}

Twitter::~Twitter()
{
}

void Twitter::setup()
{
    kDebug() << "setup of Twitter";
    m_logo = "twitter-64.png";
    m_smallText = i18nc("quick intro what this button does", "Follow your Friends on Twitter");
    m_fullText = i18nc("the full text explaining what happens here", "<p><em>Add a Twitter widget to see what your friends are thinking.</em></p>\n" \
    "<p>Click on the Twitter Widget's \"Configure...\" button to enter your account information.. </p>\n" \
    "<p><strong>Click on the button below to add a Twitter Widget to your desktop.</strong></p>");
    m_buttonText = i18nc("text on the pushbutton", "Add Twitter Widget");
}

void Twitter::run()
{
    //QString js_path = KGlobal::dirs()->findResource("data", QString("plasma-applet-webwelcome/news-add.js"));
    //kDebug() << "Script found at:" << js_path;
    emit addApplet("twitter");
    kDebug() << "Added Twitter Widget!";

    //kDebug() << ">>>>>>>>>>>>> HTML:\n\n" << m_mainView->html();
}


#include "twitter.moc"
