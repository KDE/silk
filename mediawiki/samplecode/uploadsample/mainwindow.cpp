#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
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
    Upload * e1 = new Upload( mediawiki );
    QFile file(this->ui->lineEdit->text());
    file.open(QIODevice::ReadOnly);
    e1->setFile(&file);
    e1->setFilename(this->ui->lineEdit_2->text());

    QString text("== {{int:filedesc}} == {{Information |Description=");
    text.append(this->ui->descriptionEdit->text());
    text.append(" |Source=").append(this->ui->sourceEdit->text());
    text.append(" |Date=").append(this->ui->dateEdit->text());
    text.append(" |Author=").append(this->ui->authorEdit->text());
    text.append(" |Permission=").append(this->ui->permissionEdit->text());
    text.append(" |other_versions=").append(this->ui->versionsEdit->text());
    text.append("}} == {{int:license}} ==");
    e1->setText(text);
    connect(e1, SIGNAL(result(KJob* )),this, SLOT(uploadHandle(KJob*)));
    e1->exec();
}
void MainWindow::uploadHandle(KJob* job)
{
    qDebug() << "Upload : " << job->error();
}

void MainWindow::on_parcourir_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"), "~", tr("Image Files (*.png *.jpg *.bmp *.jpeg *.gif)"));
    this->ui->lineEdit->setText(fileName);
}
