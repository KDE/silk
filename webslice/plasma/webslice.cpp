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
#include <QGraphicsSceneResizeEvent>
#include <QLabel>
#include <QSlider>

#include <QGraphicsLinearLayout>

// KDE
#include <KConfigDialog>

// Plasma
#include <Plasma/Label>

WebSlice::WebSlice(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
    m_slice(0),
    m_url(0),
    m_element(0),
    m_size(192, 192)
{
    setPopupIcon("internet-web-browser");
    setAspectRatioMode(Plasma::IgnoreAspectRatio );
    setAcceptDrops(true);
    setAcceptsHoverEvents(true);

    setMinimumSize(64, 64);
}

void WebSlice::init()
{
    KConfigGroup cg = config();
    m_url = cg.readEntry("url", "http://dot.kde.org/");
    m_element = cg.readEntry("element", "#block-user-0");

    // for testing geometry
    //m_url = cg.readEntry("url", "http://buienradar.nl/");
    //m_sliceGeometry = cg.readEntry("size", QRectF(258, 102, 550, 511));
    m_sliceGeometry = cg.readEntry("sliceGeometry", QRectF());
    m_size = cg.readEntry("size", m_size);

    //kDebug() << "url/element/slicegeometry:" << m_url << m_element << m_sliceGeometry;
}

WebSlice::~WebSlice ()
{
}

QGraphicsWidget* WebSlice::graphicsWidget()
{
    if (!m_slice) {
        m_widget = new QGraphicsWidget(this);
        QGraphicsLinearLayout *l = new QGraphicsLinearLayout(m_widget);
        m_widget->setLayout(l);


        m_slice = new SliceGraphicsWidget(m_widget);
        connect(m_slice, SIGNAL(sizeChanged(QSizeF)), this, SLOT(sizeChanged(QSizeF)));
        connect(m_slice, SIGNAL(loadFinished()), this, SLOT(loadFinished()));
        setBusy(true);
        m_slice->setUrl(m_url);
        m_slice->setElement( m_element );
        m_slice->setSliceGeometry(m_sliceGeometry);
        m_slice->hide();
        l->addItem(m_slice);
    }
    return m_widget;
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
    if ( m_url.toString() != ui.urlEdit->text() ||
         m_element != ui.elementEdit->text() ||
         ui.geometryEdit->text() != sliceGeometryToString() ) {

        m_url = QUrl(ui.urlEdit->text());
        m_element = ui.elementEdit->text();

        QString geo = ui.geometryEdit->text();
        QStringList gel = geo.split(",");
        qreal x, y, w, h;
        bool ok = true;
        if (gel.length() == 4) {
            bool allOk = true;
            x = (qreal)(gel[0].toDouble(&ok));
            if (!ok) allOk = false;
            y = (qreal)(gel[1].toDouble(&ok));
            if (!ok) allOk = false;
            w = (qreal)(gel[2].toDouble(&ok));
            if (!ok) allOk = false;
            h = (qreal)(gel[3].toDouble(&ok));
            if (!allOk) {
                kWarning() << "a conversion error occured." << gel;
            } else {
                m_sliceGeometry = QRectF(x, y, w, h);
                config().writeEntry("sliceGeometry", m_sliceGeometry);
                kDebug() << "new slice geometry:" << m_sliceGeometry;
            }
        } else {
            if (m_element.isEmpty()) {
                kWarning() << "format error, use x,y,w,h" << gel << gel.length();
            }
        }

        m_slice->setUrl(m_url);
        m_slice->setElement( m_element );
        m_slice->hide();

        KConfigGroup cg = config();
        cg.writeEntry("url", m_url.toString());
        cg.writeEntry("element", m_element);
        if (!m_element.isEmpty()) {
            m_sliceGeometry = QRectF();
        }
        emit configNeedsSaving();
        //kDebug() << "config changed" << m_element << m_url;
    }
}

QString WebSlice::sliceGeometryToString()
{
    QString s = i18n("%1,%2,%3,%4", m_sliceGeometry.x(), m_sliceGeometry.y(), m_sliceGeometry.width(),m_sliceGeometry.height()); // 18n - WTF?
    kDebug() << "geometry:" << s;
    return s;
}


void WebSlice::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & (Plasma::FormFactorConstraint | Plasma::SizeConstraint)) {
        //kDebug() << "Constraint changed:" << mapToScene(contentsRect());
        if (m_slice) {
            //kDebug() << "resizing slice to:" << contentsRect().size();
            m_slice->setMaximumSize(contentsRect().size());
            m_widget->setMinimumSize(64, 64);
            m_slice->refresh();
        }
    }
}

void WebSlice::loadFinished()
{
    setBusy(false);
    m_slice->show();
    m_size = m_slice->geometry().size();

    setAspectRatioMode(Plasma::KeepAspectRatio );

    kDebug() << "done loading, resizing slice to:" << contentsRect().size();
    m_slice->setMaximumSize(contentsRect().size());
}

void WebSlice::sizeChanged(QSizeF newsize)
{
    if (m_slice && m_size != newsize) {
        QSizeF m_size = QSizeF(newsize.width() + 28, newsize.height() + 28);
        m_slice->resize(m_size);

        QRectF g = QRectF(mapToScene(contentsRect().topLeft()), m_size);
        mapFromScene(contentsRect().topLeft());

        m_widget->setMinimumSize(m_size);
        m_slice->setMaximumSize(contentsRect().size());
        setPreferredSize(m_size);
        kDebug() << "size is now:" << m_size;
        KConfigGroup cg = config();
        cg.writeEntry("size", m_size);
        emit configNeedsSaving();
    }
}

#include "webslice.moc"
