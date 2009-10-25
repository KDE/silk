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

void SliceGraphicsWidget::createSlice()
{
    QWebFrame *frame = d->view->page()->mainFrame();
    QWebElement element = frame->findFirstElement( d->selector );
    if ( element.isNull() ) {
        //qobject_cast<QLabel*>(widget(0))->setText("Loading failed. :(");
        return;
    }
    d->view->resize( element.geometry().size() );
    frame->setScrollPosition( element.geometry().topLeft() );
    setGeometry(element.geometry());
    qDebug() << "SliceGraphicsWidget::loading finished" << element.geometry();
    emit newSize(element.geometry());
    //setCurrentIndex(1);
}

void SliceGraphicsWidget::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
    qDebug() << "SliceGraphicsWidget::resizing ... " << event->oldSize() << " -> " << event->newSize();
    QGraphicsWidget::resizeEvent(event);
    QWebFrame *frame = d->view->page()->mainFrame();
    QWebElement element = frame->findFirstElement( d->selector );
    if ( element.isNull() ) {
        return;
    }

    d->view->resize( element.geometry().size() );
    frame->setScrollPosition( element.geometry().topLeft() );
}