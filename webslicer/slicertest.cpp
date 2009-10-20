#include <qdebug.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qurl.h>

#include "slicer.h"
#include "slicertest.h"

SlicerTest::SlicerTest()
    : QWidget()
{
    slicer = new Slicer(this);
    connect( slicer, SIGNAL(finished(bool)), this, SLOT(startTest()) );

    slicer->setUrl( QUrl("http://dot.kde.org/") );

    view = new QLabel(this);
    view->setText("Loading...");
    view->setFixedSize( 400, 300 );

    QVBoxLayout *box = new QVBoxLayout(this);
    box->addWidget(view);
}

void SlicerTest::startTest()
{
    qDebug() << "startTest";
    QPixmap pix = slicer->renderElement( QString("#block-user-0") );
    if ( pix.isNull() ) {
	qDebug() << "nullness";
    }

    //    pix.save("test.png");
    view->setPixmap( pix );

    qDebug() << "done test";
}

