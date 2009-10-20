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
    connect(d->slice, SIGNAL(newSize(QRectF)), this, SLOT(sizeChanged(QRectF)));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

void SliceWidget::sizeChanged(QRectF geometry)
{
    qDebug() << "size changed" << geometry;
    setSceneRect(geometry);
    setMinimumSize(geometry.size().toSize());
}

void SliceWidget::resizeEvent ( QResizeEvent * event )
{
}