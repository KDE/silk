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
    setSceneRect(QRectF(QPointF(0, 0), newsize));
    setMinimumSize(newsize.toSize());
}

void SliceWidget::resizeEvent ( QResizeEvent * event )
{
}