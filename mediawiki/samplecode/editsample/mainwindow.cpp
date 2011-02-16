#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mediawiki(QUrl("http://test.wikipedia.org/w/api.php"))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//bouton charger page
void MainWindow::on_pushButton2_clicked()
{
    QueryRevision * const queryrevision(new QueryRevision(mediawiki));
    qDebug() << "page : " << this->ui->mPageEdit->text();
    queryrevision->setPageName(this->ui->mPageEdit->text());
    queryrevision->setProp(QueryRevision::Content);
    queryrevision->setExpandTemplates(true);
    queryrevision->setLimit(1);
    connect(queryrevision, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));
    connect(queryrevision, SIGNAL(result(KJob* )),this, SLOT(revisionError(KJob*)));
}

void MainWindow::revisionHandle(const QList<Revision> & revisions)
{    
    this->ui->plainTextEdit->setPlainText(revisions[0].content());
}

//bouton login et envoi
void MainWindow::on_pushButton1_clicked()
{
    Login *login = new Login(mediawiki, this->ui->mLoginEdit->text(), this->ui->mMdpEdit->text());
    connect(login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
    login->start();
}

void MainWindow::loginHandle(KJob* login)
{
    qDebug() << "Login : " << login->error();
    Edit * job = new Edit( mediawiki,NULL);
    job->setPageName(this->ui->mPageEdit->text());
    job->setText(this->ui->plainTextEdit->toPlainText());
    connect(job, SIGNAL(result(KJob *)),this, SLOT(editError(KJob*)));
    job->start();
}

void MainWindow::editError(KJob* job)
{
    qDebug() << "Edit : " << job->error();
}

void MainWindow::revisionError(KJob* job)
{
    qDebug() << "Revision : " << job->error();
}
