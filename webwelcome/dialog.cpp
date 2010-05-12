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

using namespace SilkWebWelcome;

Dialog::Dialog(QGraphicsWidget *parent)
    : QGraphicsWidget(parent)
{
    buildDialog();
}

Dialog::~Dialog()
{
}

void Dialog::buildDialog()
{
    QGraphicsGridLayout *gridLayout = new QGraphicsGridLayout(this);

    m_dashboard = new Plasma::WebView(this);
    gridLayout->addItem(m_dashboard, 0, 0, 1, 3); // top cell, spanning 3 columns

    QString img_path = "/home/sebas/kdesvn/src/project-silk/webwelcome/images/";

    Plasma::Label *gmail = new Plasma::Label(this);
    gmail->setImage(img_path + "gmail.png");
    gridLayout->addItem(gmail, 1, 0);

    Plasma::Label *twitter = new Plasma::Label(this);
    gmail->setImage(img_path + "twitter.png");
    gridLayout->addItem(gmail, 1, 1);

    setPreferredSize(400, 400);
    setLayout(gridLayout);
}

#include "dialog.moc"
