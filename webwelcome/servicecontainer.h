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

#ifndef SERVICECONTAINER_H
#define SERVICECONTAINER_H

//Qt
#include <QGraphicsWidget>

class QGraphicsGridLayout;

class StyleSheet;
// Plasma
namespace Plasma
{
    class IconWidget;
    class PushButton;
    class Label;
}

namespace SilkWebWelcome
{
  class ServiceContainer : public QObject
  {
  Q_OBJECT

    public:
        ServiceContainer(QGraphicsWidget *parent = 0);
        virtual ~ServiceContainer();
        virtual void setup();

        QGraphicsWidget* smallWidget();
        QGraphicsWidget* fullWidget();

  Q_SIGNALS:
        void showDetails();
        void addApplet(const QString&);
        void back();

    public Q_SLOTS:
        virtual void run();
        virtual void linkClicked(const QUrl&);
        void widgetDestroyed();

    protected:
        Plasma::IconWidget* m_smallWidget;
        QGraphicsWidget* m_fullWidget;
        QGraphicsGridLayout* m_smallLayout;
        QGraphicsGridLayout* m_fullLayout;
        Plasma::PushButton* m_backButton;
        Plasma::PushButton* m_pushButton;
        Plasma::Label *m_pixmapLabel;
        Plasma::Label *m_smallPixmapLabel;
        Plasma::WebView* m_mainView;

        QString m_style;
        QString m_logo;
        QString m_smallText;
        QString m_fullText;
        QString m_buttonText;

        StyleSheet* m_styleSheet;
};
}

#endif

