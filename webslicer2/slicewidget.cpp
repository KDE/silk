#include <qwebview.h>
#include <qwebelement.h>
#include <qwebpage.h>
#include <qwebframe.h>
#include <qboxlayout.h>

#include "slicewidget.h"

struct SliceWidgetPrivate
{
    QWebView *view;
    QString selector;
};

SliceWidget::SliceWidget( QWidget *parent )
    : QWidget( parent )
{
    d = new SliceWidgetPrivate;
    d->view = new QWebView( this );
    connect( d->view, SIGNAL( loadFinished(bool) ), this, SLOT( createSlice(bool) ) );

    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
    frame->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff );

    QVBoxLayout *box = new QVBoxLayout(this);
    box->addWidget(d->view);
}

SliceWidget::~SliceWidget()
{
    delete d;
}

void SliceWidget::setUrl( const QUrl &url )
{
    d->view->load( url );
}

void SliceWidget::setElement( const QString &selector )
{
    d->selector = selector;
}

void SliceWidget::createSlice( bool ok )
{
    if ( !ok )
	return;

    QWebFrame *frame = d->view->page()->mainFrame();
    QWebElement element = frame->findFirstElement( d->selector );
    if ( element.isNull() )
	return;

    d->view->resize( element.geometry().size() );
    frame->setScrollPosition( element.geometry().topLeft() );
}
