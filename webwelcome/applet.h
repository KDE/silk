/***************************************************************************
 *   Copyright 2010 by Sebastian Kügler <sebas@kde.org>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef WEBWELCOMEAPPLET_H
#define WEBWELCOMEAPPLET_H

//Plasma
#include <Plasma/Applet>
//#include <Plasma/ToolTipManager>

// own
#include "dialog.h"

namespace Plasma
{
    class IconWidget;
}

namespace SilkWebWelcome
{
  class Applet : public Plasma::Applet
  {
    Q_OBJECT

    public:
        Applet(QObject *parent, const QVariantList &args);
        ~Applet();
        void init();
        QGraphicsWidget *graphicsWidget();

    private Q_SLOTS:
        void handleAddApplet(const QString &applet);

    private:
        Dialog *m_dialog;
  };
};

#endif
