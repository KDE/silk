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
#include <KConfigDialog>

// Plasma


WebSlice::WebSlice(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
    m_slice(0),
    m_url(0),
    m_element(0),
    m_size(192, 192)
{
    setPopupIcon("internet-web-browser");
    setAspectRatioMode(Plasma::KeepAspectRatio );
    setAcceptDrops(true);
    setAcceptsHoverEvents(true);

    QSizeF s1(20, 20), s2(390, 390);
    kDebug() << "sizes:" << s2.height()/s1.height();
    //setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    //setBackgroundHints(NoBackground); // TODO: conditionally, pls.
    setMinimumSize(64, 64);
    //setMaximumSize(INT_MAX, INT_MAX);
    //setPopupIcon(QIcon());
}

void WebSlice::init()
{
    KConfigGroup cg = config();
    m_url = cg.readEntry("url", "http://dot.kde.org/");
    m_element = cg.readEntry("element", "#block-user-0");
    m_size = cg.readEntry("size", QSizeF(192, 192));
    setMinimumSize(m_size);

    kDebug() << "URL/ELEMENT:" << m_url << m_element;
}

WebSlice::~WebSlice ()
{
}

QGraphicsWidget* WebSlice::graphicsWidget()
{
    if (!m_slice) {
        m_slice = new SliceGraphicsWidget;
        connect(m_slice, SIGNAL(newSize(QSizeF)), this, SLOT(sizeChanged(QSizeF)));
        connect(m_slice, SIGNAL(loadFinished()), this, SLOT(loadFinished()));
        setBusy(true);
        m_slice->setUrl(m_url);
        m_slice->setElement( m_element );
        m_slice->hide();
    }
    return m_slice;
}

void WebSlice::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget(parent);
    ui.setupUi(widget);
    parent->addPage(widget, i18n("General"), Applet::icon());
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    ui.urlEdit->setText(m_url.toString());
    ui.elementEdit->setText(m_element);
}

void WebSlice::configAccepted()
{
    if (m_url.toString() != ui.urlEdit->text() || m_element != ui.elementEdit->text()) {
        m_url = QUrl(ui.urlEdit->text());
        m_element = ui.elementEdit->text();

        m_slice->setUrl(m_url);
        m_slice->setElement( m_element );
        m_slice->hide();

        KConfigGroup cg = config();
        cg.writeEntry("url", m_url.toString());
        cg.writeEntry("element", m_element);
        emit configNeedsSaving();
        kDebug() << "config changed" << m_element << m_url;
    }
}


void WebSlice::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & (Plasma::FormFactorConstraint | Plasma::SizeConstraint)) {
        kDebug() << "Constraint changed:" << contentsRect();
        sizeChanged(contentsRect().size());
    }
}

void WebSlice::loadFinished()
{
    kDebug() << "done loading";
    setBusy(false);
    m_slice->show();
    //m_size = m_slice->geometry().size();
    //kDebug() << "SIZECHANGE:" << m_size;
}

void WebSlice::sizeChanged(QSizeF newsize)
{
    kDebug() << "======================= size changed" << newsize;
    if (m_size != newsize) {
        m_size = newsize;
        //setMinimumSize(geo.size());
        //QRectF g = QRectF(0, 0, geo.width(), geo.height());
        QRectF g = QRectF(contentsRect().topLeft(), m_size);
        m_slice->setGeometry(g);
        //kDebug() << "now:" << g;
        KConfigGroup cg = config();
        cg.writeEntry("size", m_size);
        emit configNeedsSaving();
    }
}

#include "webslice.moc"
