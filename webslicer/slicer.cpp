#include <qdebug.h>
#include <qpainter.h>
#include <qurl.h>
#include <qwebpage.h>
#include <qwebframe.h>
#include <QWebElement>


#include "slicer.h"

class SlicerPrivate
{
public:
    QWebPage *page;
};

Slicer::Slicer( QObject *parent )
    : QObject(parent)
{
    d = new SlicerPrivate;
    d->page = new QWebPage( this );
    connect( d->page, SIGNAL( loadFinished(bool) ), this, SIGNAL( finished(bool) ) );
}

Slicer::~Slicer()
{
    delete d;
}

void Slicer::setUrl( const QUrl &url )
{
    d->page->mainFrame()->load( url );
}

QPixmap Slicer::renderElement( const QString &selector )
{
    QWebFrame *frame = d->page->mainFrame();
    d->page->setViewportSize( frame->contentsSize() );

    QWebElement element = frame->findFirstElement( selector );
    if ( element.isNull() )
	return QPixmap();

    qDebug() << "Found element";

    QRect rect = element.geometry();
    QPixmap result = QPixmap( rect.size() );
    result.fill( Qt::white );

    QPainter painter( &result );
    painter.translate( -rect.x(), -rect.y() );
    frame->render( &painter, QRegion(rect) );

    return result;
}

