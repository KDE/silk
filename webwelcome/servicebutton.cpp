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

//plasma
#include <Plasma/IconWidget>
#include <Plasma/Theme>


//own
#include "servicebutton.h"

using namespace SilkWebWelcome;

ServiceButton::ServiceButton(QGraphicsWidget *parent)
    : Plasma::IconWidget(parent)
{
    m_imgPath = "/home/sebas/kdesvn/src/project-silk/webwelcome/images/"; // FIXME: guess what!?!
    setupButton();
    setAcceptsHoverEvents(true);
    setDrawBackground(true);
    setContentsMargins(8,8,8,8);
}

ServiceButton::~ServiceButton()
{
}

void ServiceButton::setupButton()
{
    m_layout = new QGraphicsLinearLayout(this);
    setLayout(m_layout);

    m_pixmapLabel = new Plasma::Label(this);
    //m_pixmapLabel->setScaledContents(true);
    //m_pixmapLabel->setPreferredSize(QSize(140, 60));
    m_layout->addItem(m_pixmapLabel);
}

void ServiceButton::setPixmap(const QString &img)
{
    m_pixmapLabel->setImage(m_imgPath + img);
    m_pixmapLabel->setMinimumHeight(68);
    m_pixmapLabel->setMaximumHeight(68);
    m_pixmapLabel->setMinimumWidth(128);
}

#include "servicebutton.moc"
