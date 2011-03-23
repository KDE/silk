#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "upload.h"
#include "mediawiki.h"
#include "login.h"
#include "queryinfo.h"
#include "queryrevision.h"


namespace Ui {
    class MainWindow;
}
using namespace mediawiki;

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MediaWiki mediawiki;
    void init();
private slots:
    void on_pushButton_clicked();
    void on_parcourir_clicked();
    void on_lineEdit_textChanged(QString );
    void loginHandle(KJob* login);
    void uploadHandle(KJob* job);
    void processedUploadSize(KJob* job, qulonglong size);
    void TotalUploadSize(KJob* job, qulonglong size);


};

#endif // MAINWINDOW_H
