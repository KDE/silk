#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDebug>

#include "mainwindow.h"
#include "googlesearch.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    edit = new QLineEdit( this );
    list = new QListWidget( this );
    layout = new QVBoxLayout( this );
    layout->addWidget( edit );
    layout->addWidget( list );

    connect( edit, SIGNAL(returnPressed()), SLOT(go()) );

    search = new GoogleSearch( this );

    connect( search, SIGNAL(finished(bool)), SLOT(finished(bool)) );
}

MainWindow::~MainWindow()
{
}

void MainWindow::go()
{
    search->search( edit->text() );
}

void MainWindow::finished( bool ok )
{
    list->clear();

    GoogleSearch::Result res;
    foreach( res, search->results() ) {
	//qDebug() << res.title;
	list->addItem( res.title );
    }
}
