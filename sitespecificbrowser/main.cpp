#include <qapplication.h>

#include "view.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );
    View tl;
    tl.show();
    app.exec();
};
