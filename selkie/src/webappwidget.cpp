/*
 * Copyright 2009 Sebastian Kügler <sebas@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */


#include <kdebug.h>

#include "webappwidget.h"
#include "view.h"

struct WebAppWidgetPrivate
{
    View *view;;
};

WebAppWidget::WebAppWidget( KMainWindow *parent )
    : QGraphicsView()
{
    d = new WebAppWidgetPrivate;
    d->view = new View(parent, 0);
    d->view->setGeometry(geometry());

    QGraphicsScene *theScene = new QGraphicsScene(this);
    //theScene->addText("Selkie on QGV...");
    setScene(theScene);
    setMinimumSize(400, 400);
    kDebug() << "qgs" << theScene->sceneRect() << geometry();


    theScene->addItem(d->view);
    QRectF rect = geometry();
    kDebug() << "viewport" << rect;
    theScene->setSceneRect(rect);
    d->view->setGeometry(rect);
    setGeometry(rect.toRect());

    kDebug() << "qgs" << theScene->sceneRect() << geometry();

    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGraphicsScene *qgs = new QGraphicsScene(this);
    qgs->addItem(d->view);
    qgs->setActiveWindow(d->view);
    setScene(qgs);

    setMinimumSize(20,20);
    show();
}

View* WebAppWidget::view()
{
    return d->view;

}

WebAppWidget::~WebAppWidget()
{
    delete d;
}

void WebAppWidget::sizeChanged(QRectF geometry)
{
    qDebug() << "size changed" << geometry;
    setSceneRect(geometry);
    setMinimumSize(geometry.size().toSize());
}

void WebAppWidget::resizeEvent ( QResizeEvent * event )
{
    kDebug() << "resize" << geometry();
    d->view->setGeometry(geometry());

}