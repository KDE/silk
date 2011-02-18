#include <KDialog>
#include "ui_wmwindow.h"
#include <libkipi/interface.h>
#include "wmwindow.h"

using namespace KIPIWikiMediaPlugin;

WMWindow::WMWindow(KIPI::Interface* interface, const QString &tmpFolder,
                  bool import, QWidget* /*parent*/)
       : KDialog(0), ui(new Ui::WMWindow)
{
   ui->setupUi(this);
   m_tmpDir      = tmpFolder;
   m_interface   = interface;
   m_import      = import;
   this->setButtonText(KDialog::Ok, i18n("Upload"));
}
void WMWindow::reactivate()
{
   show();
}

WMWindow::~WMWindow()
{
    delete ui;
}

void WMWindow::changeEvent(QEvent *e)
{
    KDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
