#ifndef WMWINDOW_H
#define WMWINDOW_H
#include <KDialog>
#include <libkipi/interface.h>
namespace KIPIWikiMediaPlugin
{
class WMWindow : public KDialog
{
public:
    WMWindow(KIPI::Interface *interface, const QString& tmpFolder,
             bool import, QWidget *parent);
    void reactivate();
};

}
#endif // WMWINDOW_H
