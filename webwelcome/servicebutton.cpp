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
#include "servicebutton.h"

using namespace SilkWebWelcome;

ServiceButton::ServiceButton(QGraphicsWidget *parent)
    : Plasma::IconWidget(parent),
    m_pixmapLabel(0),
    m_layout(0)
{
    setupButton();
    setAcceptsHoverEvents(true);
    setDrawBackground(true);
    setContentsMargins(8,8,8,8);
    connect(this, SIGNAL(clicked()), this, SLOT(run()));
    
}

ServiceButton::~ServiceButton()
{
}

void ServiceButton::load(const KPluginInfo* info)
{
    kDebug() << "================";
    kDebug() << "Name   :" << info->name();
    kDebug() << "Comment:" << info->comment();
    kDebug() << "Plugin :" << info->pluginName();

    m_name = info->name();
    m_comment = info->comment();
    m_pluginName = info->pluginName();
    m_kcmName = info->property("X-Silk-KcmName").toString();
    //kDebug() << "Name:" << info.name();
    // ...
    // name

    // logo

    // run

    /*
    KDesktopFile desktopFile(m_metadataFile);
    KConfigGroup group = desktopFile.group("Desktop Entry");

    X-KDE-PluginInfo-Author=Sebastian Kügler
    X-KDE-PluginInfo-Email=sebas@kde.org
    X-KDE-PluginInfo-Name=silk
    X-Silk-StartUrl=http://gitorious.org/project-silk/
    m_metadata->pluginName = group.readEntry("X-KDE-PluginInfo-Name", QString());
    m_metadata->name = group.readEntry("Name", QString());
    m_metadata->comment = group.readEntry("Comment", QString());
    */
    setPixmap(KStandardDirs::locate("data", QString("silk/webservices/logo_%1.png").arg(m_pluginName)));
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
    //QString _img = ;
    kDebug() << "Image:" << img;

    m_pixmapLabel->setImage(img);
    m_pixmapLabel->setMinimumHeight(68);
    m_pixmapLabel->setMaximumHeight(68);
    m_pixmapLabel->setMinimumWidth(128);
}

void ServiceButton::run()
{
    //m_kcmName = "mouse";
    kDebug() << "Run clicked, starting kcmshell4"  << m_kcmName;
    QString cmd = QString("kcmshell4 %1").arg(m_kcmName);
    KRun::runCommand(cmd, 0);
}

#include "servicebutton.moc"
