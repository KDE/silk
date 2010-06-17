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
#include "servicecontainer.h"

using namespace SilkWebWelcome;

ServiceContainer::ServiceContainer(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
    m_smallWidget(0),
    m_fullWidget(0),
    m_pixmapLabel(0),
    m_smallPixmapLabel(0)
{
    kDebug();
    setup();
    setAcceptsHoverEvents(true);
    setContentsMargins(8,8,8,8);
}

ServiceContainer::~ServiceContainer()
{
}

void ServiceContainer::setup()
{
    kDebug() << "setup(),  FIXME: overload!";
    m_logo = "bird-64.png";
    m_smallText = i18nc("quick intro what this button does", "small text about this service");
    m_fullText = i18nc("the full text explaining what happens here", "the full text explaining what happens, can be longer, point to additional information, and so on.");
    m_buttonText = i18nc("text on the pushbutton", "Trigger");
}

void ServiceContainer::run()
{
    kDebug() << "Running ...; FIXME: overload!";
}

QGraphicsWidget* ServiceContainer::smallWidget()
{
    if (!m_smallWidget) {
        kDebug() << "creating small widget";
        // TODO: build widget
        m_smallWidget = new Plasma::IconWidget(this);
        m_smallLayout = new QGraphicsGridLayout();
        m_smallWidget->setLayout(m_smallLayout);

        QString image_path = KGlobal::dirs()->findResource("data", QString("plasma-applet-webwelcome/%1").arg(m_logo));
        kDebug() << "Image is at:" << image_path;
        m_smallPixmapLabel = new Plasma::Label(m_smallWidget);
        m_smallPixmapLabel->setPreferredSize(QSizeF(72, 72));
        m_smallPixmapLabel->setMinimumSize(QSizeF(72, 72));
        m_smallPixmapLabel->setMaximumSize(QSizeF(72, 72));
        //m_smallPixmapLabel->setPreferredSize(QSizeF(72, 72));
        m_smallPixmapLabel->setImage(image_path);
        m_smallPixmapLabel->setScaledContents(true);
        m_smallLayout->addItem(m_smallPixmapLabel, 0, 0);

        Plasma::Label* toplbl = new Plasma::Label(this);
        toplbl->setText(m_smallText);
        m_smallLayout->addItem(toplbl, 0, 1);
        connect(m_smallWidget, SIGNAL(clicked()), this, SIGNAL(showDetails()));
    }
    return m_smallWidget;
}

QGraphicsWidget* ServiceContainer::fullWidget()
{
    if (!m_fullWidget) {
        kDebug() << "creating full widget";
        // TODO: build widget
        m_fullWidget = new QGraphicsWidget(this);
        m_fullLayout = new QGraphicsGridLayout(m_fullWidget);
        m_fullWidget->setLayout(m_fullLayout);

        QString image_path = KGlobal::dirs()->findResource("data", QString("plasma-applet-webwelcome/%1").arg(m_logo));
        kDebug() << "Image is at:" << image_path;
        m_pixmapLabel = new Plasma::Label(m_fullWidget);
        m_pixmapLabel->setPreferredSize(QSizeF(72, 72));
        m_pixmapLabel->setMinimumSize(QSizeF(72, 72));
        m_pixmapLabel->setMaximumSize(QSizeF(72, 72));
        m_pixmapLabel->setImage(image_path);
        m_pixmapLabel->setScaledContents(true);
        m_fullLayout->addItem(m_pixmapLabel, 0, 0);

        Plasma::Label* toplbl = new Plasma::Label(m_fullWidget);
        toplbl->setText(m_smallText);
        m_fullLayout->addItem(toplbl, 0, 1);

        Plasma::WebView* lbl = new Plasma::WebView(m_fullWidget);
        lbl->setHtml(m_fullText);
        m_fullLayout->addItem(lbl, 1, 0, 1, 2);

        m_pushButton = new Plasma::PushButton(m_fullWidget);
        m_pushButton->setIcon(KIcon("dialog-ok-apply"));
        m_pushButton->setText(m_buttonText);
        connect(m_pushButton, SIGNAL(clicked()), this, SLOT(run()));
        m_fullLayout->addItem(m_pushButton, 2, 1);
    }
    return m_fullWidget;
}

#include "servicecontainer.moc"
