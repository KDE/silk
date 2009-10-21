/***************************************************************************
 *   Copyright (C) 2009 by Sebastian Kügler <sebas@kde.org>                *
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
    m_slice(0)
{
    setPopupIcon("internet-web-browser");
    setAspectRatioMode(Plasma::Square );
    //setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    //setBackgroundHints(NoBackground); // TODO: conditionally, pls.
    setMinimumSize(8, 8);
    //setMaximumSize(INT_MAX, INT_MAX);
    //setPopupIcon(QIcon());
    resize(22, 22);
}

void WebSlice::init()
{
    // TODO
}

WebSlice::~WebSlice ()
{
}

QGraphicsWidget* WebSlice::graphicsWidget()
{
    if (!m_slice) {
        m_slice = new SliceGraphicsWidget;
        connect(m_slice, SIGNAL(newSize(QRectF)), this, SLOT(sizeChanged(QRectF)));

        m_slice->setUrl( QUrl("http://dot.kde.org/") );
        m_slice->setElement( QString("#block-user-0") );
    }
    return m_slice;
}

void WebSlice::sizeChanged(QRectF geometry)
{
    kDebug() << "size changed" << geometry;
    setMinimumSize(geometry.size());
    QRectF g = QRectF(0, 0, geometry.width(), geometry.height());
    m_slice->setGeometry(g);
    kDebug() << "now:" << g;
}


#include "webslice.moc"
