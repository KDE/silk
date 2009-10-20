#include <qapplication.h>
#include "slicertest.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    SlicerTest slicertest;
    slicertest.show();

    return app.exec();
}
