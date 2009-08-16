#include <qdebug.h>

#include "../greasemonkeyscript.h"

int main( int argc, char **argv )
{
    GreaseMonkeyScript script;
    QString filename = argv[1];
    if ( !script.load( filename ) ) {
	qDebug() << "Load failed";
	return 1;
    }

    script.dump();
    return 0;
}
