/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2011-02-11
 * Description : a kipi plugin to export images to WikiMedia web service
 *
 * Copyright (C) 2011 by Alexandre Mendes <alex dot mendes1988 at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

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
private Q_SLOTS:
   void slotHelp();
private:

   QString                   m_tmpDir;
   QString                   m_tmpPath;
   WmWidget                 *m_widget;
   KIPI::Interface          *m_interface;
   KIPIPlugins::KPAboutData *m_about;
};

}
#endif // WMWINDOW_H
