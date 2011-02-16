#include "wmwindow.h"

using namespace KIPIWikiMediaPlugin;
WMWindow::WMWindow(KIPI::Interface* interface, const QString &tmpFolder,
                   bool import, QWidget* /*parent*/)
        : KDialog(0)
{
}
void WMWindow::reactivate()
{
    show();
}
