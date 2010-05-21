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

#ifndef SERVICEBUTTON_H
#define SERVICEBUTTON_H

//Qt
#include <QGraphicsLinearLayout>

// KDE

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Label>

namespace Plasma
{
    class IconWidget;
}

namespace SilkWebWelcome
{
  class ServiceButton : public Plasma::IconWidget
  {
  Q_OBJECT

    public:
        ServiceButton(QGraphicsWidget *parent = 0);
        virtual ~ServiceButton();
        void setPixmap(const QString &img);
        void load(const KPluginInfo* info);

    public Q_SLOTS:
        void run();

    private :
        void setupButton();

        QString m_imgPath;
        QGraphicsLinearLayout* m_layout;
        Plasma::Label* m_pixmapLabel;

        QString m_name;
        QString m_pluginName;
        QString m_comment;
        QString m_kcmName;
};
}

#endif

