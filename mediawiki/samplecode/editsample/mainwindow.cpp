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

//Load page
void MainWindow::on_pushButton2_clicked()
{
    QueryRevision * const queryrevision(new QueryRevision(mediawiki));    
    queryrevision->setPageName(this->ui->mPageEdit->text());
    queryrevision->setProp(QueryRevision::Content);
    queryrevision->setExpandTemplates(true);
    queryrevision->setLimit(1);
    connect(queryrevision, SIGNAL(revision(const QList<Revision> &)), this, SLOT(revisionHandle(const QList<Revision> &)));
    connect(queryrevision, SIGNAL(result(KJob* )),this, SLOT(revisionError(KJob*)));
    queryrevision->start();   
}

void MainWindow::revisionHandle(const QList<Revision> & revisions)
{        
    this->ui->plainTextEdit->setPlainText(revisions[0].content());
}

//Send page
void MainWindow::on_pushButton1_clicked()
{
    Login *login = new Login(mediawiki, this->ui->mLoginEdit->text(), this->ui->mMdpEdit->text());
    connect(login, SIGNAL(result(KJob* )),this, SLOT(loginHandle(KJob*)));
    login->start();
}

void MainWindow::loginHandle(KJob* login)
{
    if(login->error() != 0)
    {
        QMessageBox popup;
        popup.setText("Erreur avec identification/Mot de passe");
        popup.exec();
    }

    Edit * job = new Edit( mediawiki,NULL);
    job->setPageName(this->ui->mPageEdit->text());
    job->setText(this->ui->plainTextEdit->toPlainText());
    connect(job, SIGNAL(result(KJob *)),this, SLOT(editError(KJob*)));
    job->start();
}

void MainWindow::editError(KJob* job)
{
    QString errorMessage;
    if(job->error() == 0) errorMessage = "Page wiki modifiee avec succes";
    else errorMessage = "Erreur la page wiki n'a pas ete modifiee";
    QMessageBox popup;
    popup.setText(errorMessage);
    popup.exec();
}

void MainWindow::revisionError(KJob* job)
{
    if(job->error() != 0)
    {
        QMessageBox popup;
        popup.setText(QString("Erreur : La page n'a pas pu etre chargee").toLatin1());
        popup.exec();
    }
}

void MainWindow::on_mPageEdit_textChanged(QString text)
{
    this->ui->pushButton2->setEnabled(!text.isEmpty() && !text.isNull());
}

void MainWindow::on_plainTextEdit_textChanged()
{
    QString text = this->ui->plainTextEdit->toPlainText();
    this->ui->pushButton1->setEnabled(!text.isEmpty() && !text.isNull());
}
