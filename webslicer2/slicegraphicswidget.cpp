#include <qdebug.h>
#include <qlabel.h>
#include <qwebgraphicsitem.h>
#include <qwebelement.h>
#include <qwebpage.h>
#include <qwebframe.h>
#include <qboxlayout.h>

#include "slicegraphicswidget.h"

struct SliceGraphicsWidgetPrivate
{
    QWebGraphicsItem *view;
    QString selector;
};

SliceGraphicsWidget::SliceGraphicsWidget( QGraphicsWidget *parent )
    : QGraphicsWidget( parent )
{
    d = new SliceGraphicsWidgetPrivate;
    d->view = new QWebGraphicsItem( this );
    connect( d->view, SIGNAL( loadFinished(bool) ), this, SLOT( createSlice(bool) ) );
    /*
    QLabel *label = new QLabel( this );
    label->setText("loading...");
    label->setAlignment(Qt::AlignCenter);
    addWidget(label);
    */
    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
    frame->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff );

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

void SliceGraphicsWidget::setElement( const QString &selector )
{
    d->selector = selector;
}

void SliceGraphicsWidget::createSlice( bool ok )
{
    if ( !ok )
        return;

    QWebFrame *frame = d->view->page()->mainFrame();
    QWebElement element = frame->findFirstElement( d->selector );
    if ( element.isNull() ) {
        //qobject_cast<QLabel*>(widget(0))->setText("Loading failed. :(");
        return;
    }
    d->view->resize( element.geometry().size() );
    frame->setScrollPosition( element.geometry().topLeft() );
    setGeometry(element.geometry());
    //setCurrentIndex(1);
}

void SliceGraphicsWidget::resizeEvent ( QResizeEvent * event )
{
    QWebFrame *frame = d->view->page()->mainFrame();
    QWebElement element = frame->findFirstElement( d->selector );
    if ( element.isNull() ) {
        return;
    }

    d->view->resize( element.geometry().size() );
    frame->setScrollPosition( element.geometry().topLeft() );
}