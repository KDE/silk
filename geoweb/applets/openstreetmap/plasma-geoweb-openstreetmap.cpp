/*
 *   Copyright 2010 Peter Schiffer <peter.schiffer@eyeos.cz>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QTreeView>
#include <QHeaderView>
#include <QGraphicsLinearLayout>
#include <QDesktopServices>
#include <QUrl>

#include <KDebug>
#include <Plasma/DataEngineManager>

#include "plasma-geoweb-openstreetmap.h"


OSM::OSM(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    // this will get us the standard applet background, for free!
    setBackgroundHints(DefaultBackground);
    resize(400, 250);

    error = false;
}

OSM::~OSM()
{
    if (Plasma::DataEngineManager::self()->engine("geoweb")->isValid())
        Plasma::DataEngineManager::self()->unloadEngine("geoweb");
}

void OSM::init()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);

    // main tree
    tree = new Plasma::TreeView(this);
    tree->nativeWidget()->header()->setStretchLastSection(false);
    tree->nativeWidget()->header()->setResizeMode(QHeaderView::ResizeToContents);
    tree->nativeWidget()->header()->hide();
    tree->setStyleSheet("QTreeView {background-color:rgba(255, 255, 255, 0);}");
    model = new QStandardItemModel(0, 1, tree);
    parentItem = model->invisibleRootItem();
    tree->setModel(model);
    layout->addItem(tree);

    connect(this, SIGNAL(updateData()), this, SLOT(updateDataSlot()));
    connect(tree->nativeWidget(), SIGNAL(clicked(QModelIndex)), this, SLOT(clickedSlot(QModelIndex)));

    connectEngine();
}

void OSM::connectEngine()
{
    Plasma::DataEngine *geowebEngine = Plasma::DataEngineManager::self()->loadEngine("geoweb");
    if (!geowebEngine)
        setFailedToLaunch(true, I18N_NOOP("Problem loading geoweb dataengine!"));

    geowebEngine->connectSource("openstreetmap", this);
    geowebEngine->connectSource("location", this, 5000);
}

void OSM::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    if (source != "openstreetmap")
    {
        if (error)
        {
            m_data = Plasma::DataEngineManager::self()->engine("geoweb")->query("openstreetmap");
            emit updateData();
        }
        return;
    }

    m_data = data;

    emit updateData();
}

void OSM::updateDataSlot()
{
    // brush for every second item
    QBrush brush = QPalette().light();
    QColor color = brush.color();
    color.setAlpha(50);
    brush.setColor(color);

    QString osm_id = "";
    QString osm_type = "";

    QStandardItem *item;
    QStandardItem *child;

    bool even = false;

    // reset model
    model->removeRows(0, model->rowCount());

    if (m_data["status"] == "error")
    {
        item = new QStandardItem(m_data["error"].toString());
        item->setFlags(Qt::ItemIsEnabled);
        parentItem->appendRow(item);
        error = true;
        return;
    }
    else if (m_data["status"] == "loading")
    {
        item = new QStandardItem("Loading..");
        item->setFlags(Qt::ItemIsEnabled);
        parentItem->appendRow(item);
        error = true;
        return;
    }
    else if (m_data["status"] == "n/a")
    {
        item = new QStandardItem("Service unavailable.");
        item->setFlags(Qt::ItemIsEnabled);
        parentItem->appendRow(item);
        error = true;
        return;
    }

    error = false;
    Plasma::DataEngine::Data::const_iterator i = m_data.constBegin();
    while (i != m_data.constEnd())
    {
        if (parseKey(i.key(), osm_type, osm_id) != 1 || osm_type != "osm")
        {
            i++;
            continue;
        }

        item = new QStandardItem(i.value().toString());
        item->setFlags(Qt::ItemIsEnabled);

        if (even)
        {
            item->setBackground(brush);
            even = false;
        }
        else
            even = true;

        parentItem->appendRow(item);

        // distance and type
        if (m_data.contains(QString("dist_%1").arg(osm_id)) && m_data.contains(QString("type_%1").arg(osm_id)))
        {
            QString val = m_data[QString("type_%1").arg(osm_id)].toString().replace("_", " ");

            child = new QStandardItem(QString("%1m, %2").arg(m_data[QString("dist_%1").arg(osm_id)].toString(), val));
            child->setFlags(Qt::ItemIsEnabled);

            QFont font = child->font();
            font.setCapitalization(QFont::Capitalize);
            child->setFont(font);

            item->appendRow(child);
        }

        // cuisine
        if (m_data.contains(QString("cuisine_%1").arg(osm_id)))
        {
            child = new QStandardItem(QString("Cuisine: %1").arg(m_data[QString("cuisine_%1").arg(osm_id)].toString()));
            child->setFlags(Qt::ItemIsEnabled);
            item->appendRow(child);
        }

        // opening hours
        if (m_data.contains(QString("openingh_%1").arg(osm_id)))
        {
            child = new QStandardItem(QString("Opening hours: %1").arg(m_data[QString("openingh_%1").arg(osm_id)].toString()));
            child->setFlags(Qt::ItemIsEnabled);
            item->appendRow(child);
        }

        // website
        if (m_data.contains(QString("web_%1").arg(osm_id)))
        {
            child = new QStandardItem(QString("Website"));
            child->setFlags(Qt::ItemIsEnabled);
            child->setToolTip(m_data[QString("web_%1").arg(osm_id)].toString());

            QFont font = child->font();
            font.setUnderline(true);
            child->setFont(font);

            item->appendRow(child);
        }

        // place on open street map
        if (m_data.contains(QString("url_%1").arg(osm_id)))
        {
            child = new QStandardItem(QString("Show on map"));
            child->setFlags(Qt::ItemIsEnabled);
            child->setToolTip(m_data[QString("url_%1").arg(osm_id)].toString());

            QFont font = child->font();
            font.setUnderline(true);
            child->setFont(font);

            item->appendRow(child);
        }

        i++;
    }
}

int OSM::parseKey(const QString &key, QString &type, QString &osm_id)
{
    QStringList list = key.split("_", QString::SkipEmptyParts);
    if (list.isEmpty())
        return -1;

    osm_id = list.last();
    type = list.first();

    return 1;
}

void OSM::clickedSlot(const QModelIndex &index)
{
    QString text = index.data(Qt::ToolTipRole).toString();

    if (text.startsWith("http://"))
        QDesktopServices::openUrl(QUrl(text));
}

#include "plasma-geoweb-openstreetmap.moc"
