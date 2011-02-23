#include "wmwindow.h"
#include <QLayout>
#include <kdebug.h>
#include <KConfig>
#include <KLocale>
#include <KMenu>
#include <KHelpMenu>
#include <KLineEdit>
#include <KComboBox>
#include <KPushButton>
#include <KMessageBox>
#include <KToolInvocation>
#include "kpaboutdata.h"
#include "wmwidget.h"

using namespace KIPIWikiMediaPlugin;

WMWindow::WMWindow(KIPI::Interface* interface, const QString &tmpFolder,
                  QWidget* /*parent*/)
       : KDialog(0)
{
    m_tmpPath.clear();
    m_tmpDir      = tmpFolder;
    m_interface   = interface;
    m_widget      = new WmWidget(this, interface);

    setMainWidget(m_widget);

    setWindowIcon(KIcon("wikimedia"));
    setButtons(Help|User1|Close);
    setDefaultButton(Close);
    setModal(false);

    setWindowTitle(i18n("Export to Wikimedia Web Service"));
    setButtonGuiItem(User1,
                     KGuiItem(i18n("Start Upload"), "network-workgroup",
                              i18n("Start upload to Wikimedia web service")));
    m_widget->setMinimumSize(700, 500);

    m_about = new KIPIPlugins::KPAboutData(ki18n("Wikimedia Export"), 0,
                          KAboutData::License_GPL,
                          ki18n("A Kipi plugin to export image collection "
                                "to Wikimedia web service."),
                          ki18n("(c) 2011, Alexandre Mendes"));

    m_about->addAuthor(ki18n("Alexandre Mendes"), ki18n("Author"),
                           "alex dot mendes1988 at gmail dot com");

    KHelpMenu* helpMenu = new KHelpMenu(this, m_about, false);
    helpMenu->menu()->removeAction(helpMenu->menu()->actions().first());
    QAction *handbook   = new QAction(i18n("Handbook"), this);
    connect(handbook, SIGNAL(triggered(bool)),
            this, SLOT(slotHelp()));
    helpMenu->menu()->insertAction(helpMenu->menu()->actions().first(), handbook);
    button(Help)->setMenu(helpMenu->menu());

}
WMWindow::~WMWindow()
{
    delete m_about;
}
void WMWindow::reactivate()
{
   show();
}
