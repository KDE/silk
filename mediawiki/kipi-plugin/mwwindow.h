#ifndef MWWINDOW_H
#define MWWINDOW_H
#include <KDialog>
#include <libkipi/interface.h>
namespace KIPIWikiMediaPlugin
{
class MWWindow : public KDialog
{
public:
    MWWindow(KIPI::Interface *interface, const QString& tmpFolder,
             bool import, QWidget *parent);
    void reactivate(){}
};

}
#endif // MWWINDOW_H
