/***************************************************************************
 *   Copyright (C) 2009 by Sebastian K?gler <sebas@kde.org>                *
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

#include "webslice.h"

#include <slicegraphicswidget.h>
#include <limits.h>

// Qt
#include <QLabel>
#include <QSlider>

#include <QGraphicsLinearLayout>

// KDE
//#include <KGlobalSettings>

// Plasma


WebSlice::WebSlice(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
    m_url(0),
    m_selector(0),
    m_slice(0)
{
    setPopupIcon("internet-web-browser");
    setAspectRatioMode(Plasma::KeepAspectRatio );
    setAcceptDrops(true);
    setAcceptsHoverEvents(true);

    //setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    //setBackgroundHints(NoBackground); // TODO: conditionally, pls.
    setMinimumSize(64, 64);
    //setMaximumSize(INT_MAX, INT_MAX);
    //setPopupIcon(QIcon());
    resize(192, 192);
}

void WebSlice::init()
{
    // TODO
    KConfigGroup cg = config();
    m_url = cg.readEntry("url", "http://dot.kde.org/");
    m_selector = cg.readEntry("selector", "#block-user-0");

}

WebSlice::~WebSlice ()
{
}

QGraphicsWidget* WebSlice::graphicsWidget()
{
    if (!m_slice) {
        m_slice = new SliceGraphicsWidget;
        connect(m_slice, SIGNAL(newSize(QRectF)), this, SLOT(sizeChanged(QRectF)));
        connect(m_slice, SIGNAL(loadFinished()), this, SLOT(loadFinished()));

        m_slice->setUrl( QUrl("http://dot.kde.org/") );
        m_slice->setElement( QString("#block-user-0") );
        m_slice->hide();
    }
    return m_slice;
}

void WebSlice::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & (Plasma::FormFactorConstraint | Plasma::SizeConstraint)) {
        kDebug() << "Constraint changed:" << contentsRect();
        sizeChanged(contentsRect());
    }
}

void WebSlice::loadFinished()
{
    kDebug() << "done loading";
    setBusy(false);
    m_slice->show();
}

void WebSlice::sizeChanged(QRectF geo)
{
    kDebug() << "======================= size changed" << geometry() << geo;
    setMinimumSize(geo.size());
    //QRectF g = QRectF(0, 0, geo.width(), geo.height());
    QRectF g = QRectF(contentsRect().topLeft(), geo.size());
    //m_slice->setMinimumSize(geo.size());
    //kDebug() << "now:" << g;
}


#include "webslice.moc"
