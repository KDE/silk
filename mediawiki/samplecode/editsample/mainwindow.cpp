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

void MainWindow::on_pushButton_clicked()
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
    connect(job, SIGNAL(result(KJob *)),this, SLOT(editHandle(KJob*)));
    job->start();
}
void MainWindow::editHandle(KJob* job)
{
    qDebug() << "Edit : " << job->error();
}
