/*
 *   Copyright 2009 by Sebastian Kügler <sebas@kde.org>
 *   Copyright 2009 by Richard Moore <rich@kde.org>

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <qdebug.h>

#include "slicewidget.h"
#include "slicegraphicswidget.h"

struct SliceWidgetPrivate
{
    SliceGraphicsWidget *slice;
    QString selector;
};

SliceWidget::SliceWidget( QWidget *parent )
    : QGraphicsView( parent )
{
    d = new SliceWidgetPrivate;
    d->slice = new SliceGraphicsWidget;
    connect(d->slice, SIGNAL(sizeChanged(QSizeF)), this, SLOT(sizeChanged(QSizeF)));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGraphicsScene *qgs = new QGraphicsScene(this);
    qgs->addItem(d->slice);
    qgs->setActiveWindow(d->slice);
    setScene(qgs);

    setMinimumSize(20,20);
    //setPreferedSize(20,80);
}

SliceWidget::~SliceWidget()
{
    delete d;
}

void SliceWidget::setUrl( const QUrl &url )
{
    d->slice->setUrl( url );
}

void SliceWidget::setElement( const QString &selector )
{
    d->slice->setElement(selector);
}

void SliceWidget::sizeChanged(QSizeF newsize)
{
    qDebug() << "size changed" << newsize;
    QRectF newgeometry = QRectF(QPointF(0, 0), newsize);
    setSceneRect(newgeometry);
    resize(newgeometry.toRect().size());
}

QSize SliceWidget::sizeHint () const
{
    return sceneRect().size().toSize();
}

void SliceWidget::resizeEvent ( QResizeEvent * event )
{
    QRectF newgeometry = QRectF(QPointF(0, 0), event->size());
    qDebug() << "SliceWidget::resizeEvent" << newgeometry << "(" << event->oldSize() << ")";
    setSceneRect(newgeometry);
    d->slice->setGeometry(newgeometry);
}
