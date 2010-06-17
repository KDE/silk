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
#include <QPixmap>

//KDE
#include <KDebug>
#include <KRun>
#include <KStandardDirs>
//plasma
#include <Plasma/IconWidget>
#include <Plasma/Theme>


//own
#include "servicecontainer.h"

using namespace SilkWebWelcome;

ServiceContainer::ServiceContainer(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
    m_smallWidget(0),
    m_fullWidget(0)
{
    kDebug();
    setup();
    setAcceptsHoverEvents(true);
    //setDrawBackground(true);
    setContentsMargins(8,8,8,8);
    connect(this, SIGNAL(clicked()), this, SLOT(run()));
}

ServiceContainer::~ServiceContainer()
{
}

void ServiceContainer::setup()
{
    kDebug() << "setup(),  FIXME: overload!";
    m_smallText = i18nc("quick intro what this button does", "small text about this service");
    m_fullText = i18nc("the full text explaining what happens here", "the full text explaining what happens, can be longer, point to additional information, and so on.");
    m_pixmap = QPixmap();
}

void ServiceContainer::run()
{
    kDebug() << "Running ...; FIXME: overload!";
    //kDebug() << "Run clicked, starting kcmshell4"  << m_kcmName;
    //QString cmd = QString("kcmshell4 %1").arg(m_kcmName);
    //KRun::runCommand(cmd, 0);
}

QGraphicsWidget* ServiceContainer::smallWidget()
{
    if (!m_smallWidget) {
        kDebug() << "creating small widget";
        // TODO: build widget
        m_smallWidget = new QGraphicsWidget(this);
        QGraphicsGridLayout* layout = new QGraphicsGridLayout();
        m_smallWidget->setLayout(layout);

        Plasma::Label* pixmaplbl = new Plasma::Label(this);
        //pixmaplbl->setText(m_smallText);
        // setImage...
        layout->addItem(pixmaplbl, 0, 1);

        Plasma::Label* toplbl = new Plasma::Label(this);
        toplbl->setText(m_smallText);
        layout->addItem(toplbl, 1, 1);

        //Plasma::Label* lbl = new Plasma::Label(this);
        //lbl->setText(m_smallText);
        //m_pixmapLabel->setScaledContents(true);
        //m_pixmapLabel->setPreferredSize(QSize(140, 60));
    }
    return m_smallWidget;
}

QGraphicsWidget* ServiceContainer::fullWidget()
{
    if (!m_fullWidget) {
        kDebug() << "creating full widget";
        // TODO: build widget
        m_fullWidget = new QGraphicsWidget(this);
        QGraphicsGridLayout* layout = new QGraphicsGridLayout(this);
        m_fullWidget->setLayout(layout);

        Plasma::Label* pixmaplbl = new Plasma::Label(this);
        //pixmaplbl->setText(m_smallText);
        // setImage...
        layout->addItem(pixmaplbl, 0, 1);

        Plasma::Label* toplbl = new Plasma::Label(this);
        toplbl->setText(m_smallText);
        layout->addItem(toplbl, 1, 1);

        Plasma::Label* lbl = new Plasma::Label(this);
        lbl->setText(m_fullText);
        //m_pixmapLabel->setScaledContents(true);
        //m_pixmapLabel->setPreferredSize(QSize(140, 60));
        layout->addItem(lbl, 1, 0, 1, 2);
    }
    return m_fullWidget;
}

void ServiceContainer::setPixmap(const QString &img)
{
    //QString _img = ;
    kDebug() << "Image:" << img;
}

#include "servicecontainer.moc"
