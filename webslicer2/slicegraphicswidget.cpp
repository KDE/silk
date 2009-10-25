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
    /*
    QLabel *label = new QLabel( this );
    label->setText("loading...");
    label->setAlignment(Qt::AlignCenter);
    addWidget(label);
    */
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
    frame->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff );
    frame->setHtml("<h1>Loading ...</h1>");

    //addWidget(d->view);
    //setCurrentIndex(0);
    //setMinimumSize(20,20);
    //setPreferedSize(20,80);
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
    QWebFrame *frame = d->view->page()->mainFrame();
    QRectF geo = QRectF();
    if (!d->sliceGeometry.isValid() && !d->selector.isEmpty()) {
        QWebElement element = frame->findFirstElement( d->selector );
        if ( element.isNull() ) {
            d->view->setHtml("Loading failed. :(");
            return;
        }
        geo = element.geometry();
    } else if (d->sliceGeometry.isValid()) {

    } else {
        qWarning() << "invalid size" << d->sliceGeometry;
    }
    d->view->resize( geo.size() );
    frame->setScrollPosition( geo.topLeft().toPoint() );
    setGeometry(geo);
    qDebug() << "SliceGraphicsWidget::loading finished" << geo;
    emit newSize(geo.size());
    //setCurrentIndex(1);
}

void SliceGraphicsWidget::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    QWebFrame *frame = d->view->page()->mainFrame();
    QRectF geo = QRectF();
    if (!d->sliceGeometry.isValid() && !d->selector.isEmpty()) {
        QWebElement element = frame->findFirstElement( d->selector );
        if ( element.isNull() ) {
            d->view->setHtml("Loading failed. :(");
            return;
        }
        geo = element.geometry();
    } else if (d->sliceGeometry.isValid()) {
        geo = d->sliceGeometry;
    } else {
        qWarning() << "invalid size" << d->sliceGeometry;
    }
    d->view->resize( geo.size() );
    frame->setScrollPosition( geo.topLeft().toPoint() );
}