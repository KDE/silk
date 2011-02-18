#ifndef WMWINDOW_H
#define WMWINDOW_H
#include <KDialog>
#include <libkipi/interface.h>

namespace Ui {
    class WMWindow;
}
namespace KIPIWikiMediaPlugin
{
class WMWindow : public KDialog {
    Q_OBJECT
public:
    WMWindow(KIPI::Interface *interface, const QString& tmpFolder,
                     bool import, QWidget *parent);
    ~WMWindow();
    void reactivate();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::WMWindow *ui;
    bool                      m_import;
    QString                   m_tmpDir;
    KIPI::Interface          *m_interface;
};
}
#endif // WMWINDOW_H
