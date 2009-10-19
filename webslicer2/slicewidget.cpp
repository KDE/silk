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
    /*
    //d->view = new QGraphicsView( this );
    //connect( d->view, SIGNAL( loadFinished(bool) ), this, SLOT( createSlice(bool) ) );

    QLabel *label = new QLabel( this );
    label->setText("loading...");
    label->setAlignment(Qt::AlignCenter);
    addWidget(label);

    QWebFrame *frame = d->view->page()->mainFrame();
    frame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
    frame->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff );
    */
    //addWidget(d->slice);

    QGraphicsScene *qgs = new QGraphicsScene(this);
    qgs->addItem(d->slice);
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


void SliceWidget::resizeEvent ( QResizeEvent * event )
{
}