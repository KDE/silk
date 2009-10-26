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
};

SliceGraphicsWidget::SliceGraphicsWidget( QGraphicsWidget *parent )
    : QGraphicsWidget( parent )
{
    d = new SliceGraphicsWidgetPrivate;
    d->view = new QGraphicsWebView( this );
    connect( d->view, SIGNAL( loadFinished() ), this, SIGNAL( loadFinished() ) );
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
    qDebug() << "createSLice";
    QRectF geo = sliceGeometry();
    d->view->resize( geo.size() );
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollPosition( geo.topLeft().toPoint() );
    setPreferredSize(geo.size());
    updateGeometry();
    emit sizeChanged(geo.size());
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
    d->view->resize( geo.size() );
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollPosition( geo.topLeft().toPoint() );
    setMinimumSize(geo.size());
    setMaximumSize(geo.size());
    setPreferredSize(geo.size());
    resize(geo.size());
    updateGeometry();
    //emit sizeHintChanged();

}

void SliceGraphicsWidget::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    QSizeF o = event->oldSize();
    o = d->view->geometry().size();
    QSizeF n = event->newSize();
    qreal f;
    qDebug() << "old:" << o << "new:" << n;
    if (!(o.height() && n.height() && o.width() && n.width())) {
        qDebug() << "chickening out";
        refresh();
        return;
    }
    /*
    if (o.height() < n.height() || o.width() < n.width()) {
        // made bigger
        qreal f = qMin((o.height() / n.height()), (o.width() / n.width()));
        qDebug() << "Growing" << f << (o.height() / n.height()) << (o.width() / n.width());
    } else {
        qreal f = qMin((o.height() / n.height()), (o.width() / n.width()));
        // smaller
        qDebug() << "Shrinking" << f << (o.height() / n.height()) << (o.width() / n.width());
    }
    //d->view->setZoomFactor(f);

    qDebug() << " -----------> resizeevent" << f << o.height() << n.height();
    */
    refresh();
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
