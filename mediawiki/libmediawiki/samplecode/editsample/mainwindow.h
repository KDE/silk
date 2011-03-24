#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "edit.h"
#include "mediawiki.h"
#include "login.h"
#include "queryinfo.h"
#include "queryrevision.h"

namespace Ui
{
    class MainWindow;
}

using namespace mediawiki;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private Q_SLOTS:

    void on_plainTextEdit_textChanged();
    void on_mPageEdit_textChanged(QString);
    void on_pushButton1_clicked();
    void on_pushButton2_clicked();
    void loginHandle(KJob* login);
    void revisionHandle(const QList<Revision>& revisions);
    void editError(KJob* job);
    void revisionError(KJob* job);

private:

    Ui::MainWindow* ui;
    MediaWiki       mediawiki;
};

#endif // MAINWINDOW_H
