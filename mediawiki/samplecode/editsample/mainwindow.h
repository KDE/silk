#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "edit.h"
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
private slots:
    void on_pushButton_clicked();
    void loginHandle(KJob* login);
    void editHandle(KJob* job);


};

#endif // MAINWINDOW_H
