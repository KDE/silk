#include <qapplication.h>
#include <qboxlayout.h>
#include <qtoolbar.h>
#include <qwidget.h>

#include "view.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    QWidget *w = new QWidget();
    QToolBar *bar = new QToolBar(w);
    bar->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    View tl(w);
    QAction *action;
    foreach( action, tl.actions() ) {
        bar->addAction( action );
    }

    QVBoxLayout *box = new QVBoxLayout(w);
    box->addWidget(bar);
    box->addWidget(&tl);

    w->show();
    app.exec();
}
