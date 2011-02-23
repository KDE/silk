#ifndef WMWINDOW_H
#define WMWINDOW_H
#include <KDialog>
#include <libkipi/interface.h>
namespace KIPI
{
    class Interface;
}

namespace KIPIPlugins
{
    class KPAboutData;
}

namespace KIPIWikiMediaPlugin
{
    class WmWidget;
class WMWindow : public KDialog
{
    Q_OBJECT

public:
   WMWindow(KIPI::Interface *interface, const QString& tmpFolder,
            QWidget *parent);
   void reactivate();
   ~WMWindow();
private:

   QString                   m_tmpDir;
   QString                   m_tmpPath;
   WmWidget                 *m_widget;
   KIPI::Interface          *m_interface;
   KIPIPlugins::KPAboutData *m_about;
};

}
#endif // WMWINDOW_H
