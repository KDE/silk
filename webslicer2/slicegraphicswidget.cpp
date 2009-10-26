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
    d->view->resize( sliceGeometry().size() );
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollPosition( sliceGeometry().topLeft().toPoint() );
    /*
    QWebFrame *frame = d->view->page()->mainFrame();
    qDebug() << "createSLice1";
    d->view->resize( sliceGeometry().size() );
    qDebug() << "createSLice2";
    frame->setScrollPosition( sliceGeometry().topLeft().toPoint() );
    qDebug() << "createSLice3";
    setGeometry(sliceGeometry());
    qDebug() << "SliceGraphicsWidget::loading finished" << sliceGeometry();
    emit loadFinished();
    emit newSize(sliceGeometry().size());
    */
}

QRectF SliceGraphicsWidget::sliceGeometry()
{
    qDebug() << "get geo";
    QWebFrame *frame = d->view->page()->mainFrame();
    QRectF geo = QRectF();
    if (!d->selector.isEmpty()) {
        qDebug() << "Getting size from element";
        QWebElement element = frame->findFirstElement( d->selector );
        if ( element.isNull() ) {
            //d->view->setHtml("Loading failed. :(");
            qDebug() << "element is null..." << d->selector;
            return QRectF();
        }
        geo = element.geometry();
        qDebug() << "element geometry" << geo;
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

void SliceGraphicsWidget::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    qDebug() << "resizeevent";
    d->view->resize( sliceGeometry().size() );
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollPosition( sliceGeometry().topLeft().toPoint() );
}