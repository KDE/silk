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
#include <QGraphicsSceneResizeEvent>
#include <qlabel.h>
#include <qgraphicswebview.h>
#include <qwebelement.h>
#include <qwebpage.h>
#include <qwebframe.h>
#include <qboxlayout.h>

#include "slicegraphicswidget.h"

struct SliceGraphicsWidgetPrivate
{
    QGraphicsWebView *view;
    QString selector;
    QRectF sliceGeometry;
    QRectF originalGeometry;
    qreal currentZoom;
};

SliceGraphicsWidget::SliceGraphicsWidget( QGraphicsWidget *parent )
    : QGraphicsWidget( parent )
{
    d = new SliceGraphicsWidgetPrivate;
    d->currentZoom = 1.0;
    d->originalGeometry = QRectF();
    d->view = new QGraphicsWebView( this );
    //connect( d->view, SIGNAL( loadFinished() ), this, SIGNAL( loadFinished() ) );
    connect( d->view, SIGNAL( loadFinished() ), this, SLOT( createSlice() ) );

    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
    frame->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff );
    frame->setHtml("<h1>Loading ...</h1>");
}

SliceGraphicsWidget::~SliceGraphicsWidget()
{
    delete d;
}

void SliceGraphicsWidget::setUrl( const QUrl &url )
{
    d->view->load( url );
}

QUrl SliceGraphicsWidget::url()
{
    return d->view->url();
}

QString SliceGraphicsWidget::element()
{
    return d->selector;
}

void SliceGraphicsWidget::setElement( const QString &selector )
{
    d->selector = selector;
}

void SliceGraphicsWidget::setSliceGeometry( const QRectF geo )
{
    d->sliceGeometry = geo;
}

void SliceGraphicsWidget::createSlice()
{
    qDebug() << "SliceGraphicsWidget::createSlice()";
    QRectF geo = sliceGeometry();
    if (!geo.isValid()) {
        qDebug() << "Not creating slice, geo invalid" << geo;
        return;
    }
    d->originalGeometry = geo;
    d->view->resize( geo.size() );
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollPosition( geo.topLeft().toPoint() );
    refresh();
    emit sizeChanged(geo.size());
    emit loadFinished();
}

QRectF SliceGraphicsWidget::sliceGeometry()
{
    QWebFrame *frame = d->view->page()->mainFrame();
    QRectF geo = QRectF();
    if (!d->selector.isEmpty()) {
       QWebElement element = frame->findFirstElement( d->selector );
        if ( element.isNull() ) {
            qDebug() << "element is null..." << d->selector;
            return QRectF();
        }
        geo = element.geometry();
        //qDebug() << "element geometry" << geo;
    } else if (d->sliceGeometry.isValid()) {
        geo = d->sliceGeometry;
    } else {
        qWarning() << "invalid element and size" << d->selector << d->sliceGeometry;
    }
    if (!geo.isValid()) {
        qDebug() << "invalid geometry" << geo;
        return QRectF();
    }
    return geo;

}

void SliceGraphicsWidget::refresh()
{
    QRectF geo = sliceGeometry();
    if (!geo.isValid()) {
        qDebug() << "Not creating slice, geo invalid" << geo;
        return;
    }
    d->view->resize( geo.size() );
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollPosition( geo.topLeft().toPoint() );
    qDebug() << "top point" << geo.topLeft().toPoint();
    setPreferredSize(geo.size());
    updateGeometry();
    qDebug() << "refreshed. ... " << geo;
}

void SliceGraphicsWidget::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    qDebug() << "-------------------__";
    qDebug() << "SliceGraphicsWidget::resizeEvent" << event->newSize() << "(" << event->oldSize() << ")";
    QSizeF o = d->originalGeometry.size();
    QSizeF n = event->newSize();
    qDebug() << "Slice :" << o;
    qDebug() << "Widget:" << n;
    // Prevent oopses.
    if (n.width() > 2400 || n.height() > 2400) {
        qDebug() << "What's going on???????" << o.width();
        return;
    }
    qreal f = n.width() / o.width();
    if (f > 0.1 && f < 8) {
        d->view->setZoomFactor(f);
        refresh();
        qDebug() << "Zoom  :" << n.width() << " / " <<  o.width() << " = " << f;
    }
}

QPixmap SliceGraphicsWidget::elementPixmap( const QString &selector )
{
    QRectF rect = sliceGeometry();
    if (!rect.isValid()) {
        return QPixmap();
    }
    QPixmap result = QPixmap( rect.size().toSize() );
    result.fill( Qt::white );

    QPainter painter( &result );
    painter.translate( -rect.x(), -rect.y() );
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->render( &painter, QRegion(rect.toRect()) );

    return result;
}
