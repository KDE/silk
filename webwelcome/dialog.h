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

#ifndef SWWDIALOG_H
#define SWWDIALOG_H

//Qt
#include <QGraphicsGridLayout>
#include <QLabel>
#include <QStringList>

// KDE
#include <KDirLister>
#include <KFileItem>
#include <KIO/ListJob>
#include <kio/jobclasses.h>

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/LineEdit>
#include <Plasma/TabBar>
#include <Plasma/WebView>

// Own
#include "servicecontainer.h"

class StyleSheet;
//class Applet;

//desktop view
namespace Plasma
{
    class Icon;
    class Dialog;
}

namespace SilkWebWelcome
{
  class Dialog : public QGraphicsWidget
  {
  Q_OBJECT

    public:
        Dialog(QGraphicsWidget *parent);
        virtual ~Dialog();

    Q_SIGNALS:
        void addApplet(const QString&);

    private Q_SLOTS:
        void widgetDetails();

    private :
        void buildDialog();
        void loadServices();
        void addService(ServiceContainer* container, int row);

        QGraphicsGridLayout *m_gridLayout;
        QGraphicsWidget *m_homeWidget;
        Plasma::TabBar* m_tabBar;
        Plasma::WebView *m_dashboard;
        QHash<QString, QGraphicsWidget*> m_serviceButtons;

        StyleSheet *m_styleSheet;

        QList<ServiceContainer*> m_containers;
  };
}

#endif

