#include <qapplication.h>
#include <qurl.h>
#include "slicewidget.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    SliceWidget slice;
    slice.setUrl( QUrl(QString("http://dot.kde.org/")) );
    slice.setElement( QString("#block-user-0") );
    slice.show();

    return app.exec();
}
