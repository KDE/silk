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

#include <QGraphicsLinearLayout>

#include <Plasma/DataEngineManager>

#include "plasma-geoweb-hello.h"


Hello::Hello(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    // this will get us the standard applet background, for free!
    setBackgroundHints(DefaultBackground);
    resize(200, 80);
}

Hello::~Hello()
{
    if (!hasFailedToLaunch())
        Plasma::DataEngineManager::self()->unloadEngine("geoweb");
}

void Hello::init()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, this);

    // Hello! text
    key = new Plasma::Label(this);
    key->setAlignment(Qt::AlignHCenter);
    layout->addItem(key);

    // the rest
    text = new Plasma::Label(this);
    text->setAlignment(Qt::AlignHCenter);
    layout->addItem(text);

    connectEngine();
}

void Hello::connectEngine()
{
    Plasma::DataEngine *geowebEngine = Plasma::DataEngineManager::self()->loadEngine("geoweb");
    if (!geowebEngine)
        setFailedToLaunch(true, I18N_NOOP("Problem loading geoweb dataengine!"));

    geowebEngine->connectSource("helloPlugin", this);
}

void Hello::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    if (source != "helloPlugin")
        return;

    Plasma::DataEngine::Data::const_iterator i = data.constBegin();
    while (i != data.constEnd())
    {
        if (i.key() != "status")
        {
            key->setText(i.key());
            text->setText(i.value().toString());
        }
        i++;
    }
}

#include "plasma-geoweb-hello.moc"
