/***************************************************************************
 *   Copyright 2010 by Sebastian Kügler <sebas@kde.org>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

//own
#include "applet.h"
#include "dialog.h"

//Qt
#include <QClipboard>
#include <QGraphicsLinearLayout>

//KDE
#include <KIcon>
#include <KConfigDialog>
#include <KStandardDirs>
#include <KToolInvocation>

//plasma
#include <Plasma/Containment>
#include <Plasma/Dialog>
#include <Plasma/IconWidget>
#include <Plasma/Theme>



using namespace SilkWebWelcome;

K_EXPORT_PLASMA_APPLET(webwelcome, Applet)

Applet::Applet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
      m_dialog(0)
{
    //setBackgroundHints(StandardBackground);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    //setHasConfigurationInterface(true);
    //setAcceptsHoverEvents(true);

    // initialize the widget
    (void)graphicsWidget();

}

Applet::~Applet()
{
}

void Applet::init()
{
    //setPopupIcon(m_icon->icon());

    //configChanged();
}

void Applet::handleAddApplet(const QString &applet)
{
    kDebug() << "Adding applet ..." << applet;
    containment()->addApplet(applet);
}


QGraphicsWidget* Applet::graphicsWidget()
{
    if (!m_dialog) {
        QGraphicsLinearLayout *lay = new QGraphicsLinearLayout(this);
        setLayout(lay);
        m_dialog = new Dialog(this);
        lay->addItem(m_dialog);
        connect(m_dialog, SIGNAL(addApplet(const QString&)), this, SLOT(handleAddApplet(const QString&)));
    }
    return m_dialog;
}

#include "applet.moc"
