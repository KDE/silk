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

#include <QWidget>
#include <QtNetwork>
#include <QMessageBox>
#include <QGraphicsLinearLayout>

#include <Plasma/DataEngineManager>

#include "plasma-geoweb-location.h"

#include "geocodehandler.h"


Location::Location(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    // this will get us the standard applet background, for free!
    setBackgroundHints(DefaultBackground);
    resize(350, 125);
}

Location::~Location()
{
    if (!hasFailedToLaunch())
        Plasma::DataEngineManager::self()->unloadEngine("geoweb");
}

void Location::init()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);

    QString qlabelBold = "QLabel {font-weight: bold;}";

    // Labels on the left side
    labels = new Plasma::Label(this);
    labels->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labels->setStyleSheet(qlabelBold);
    labels->setText(QString("%1 :\n%2 :\n%3 :\n%4 :\n%5 :\n%6 :").arg(I18N_NOOP("Country Code"),
            I18N_NOOP("Country"), I18N_NOOP("City"), I18N_NOOP("Address"), I18N_NOOP("Latitude"), I18N_NOOP("Longitude")));
    layout->addItem(labels);

    // Right side
    dataLabels = new Plasma::Label(this);
    dataLabels->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    dataLabels->setStyleSheet(qlabelBold);
    layout->addItem(dataLabels);

    connectEngine();
}

void Location::connectEngine()
{
    geowebEngine = Plasma::DataEngineManager::self()->loadEngine("geoweb");
    if (!geowebEngine)
        setFailedToLaunch(true, I18N_NOOP("Problem loading geoweb dataengine!"));

    geowebEngine->connectSource("location", this);
}

void Location::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    if (source != "location")
        return;

    dataLabels->setText(QString("%1\n%2\n%3\n%4\n%5\n%6").arg(data["country code"].toString(),
            data["country"].toString(), data["city"].toString(), data["address"].toString(),
            data["latitude"].toString(), data["longitude"].toString()));

    location = data;
}

void Location::createConfigurationInterface(KConfigDialog *parent)
{
    configParent = parent;
    QWidget *locationConfWidget = new QWidget();
    locationConfUi.setupUi(locationConfWidget);
    parent->addPage(locationConfWidget, i18nc("Location configuration page", "Location"), Applet::icon());

    if (Settings::source() == Settings::EnumSource::GeolocationDataengine)
    {
        locationConfUi.geolocationRadioButton->setChecked(true);
        locationConfUi.groupBox->setDisabled(true);
        locationConfUi.checkPushButton->setDisabled(true);
    }
    else if (Settings::source() == Settings::EnumSource::Manual)
    {
        locationConfUi.manualRadioButton->setChecked(true);
    }

    if (Settings::manualSelection() == Settings::EnumManualSelection::Place)
    {
        locationConfUi.PlaceRadioButton->setChecked(true);
        disableCoordinates();
    }
    else
    {
        locationConfUi.coordinatesRadioButton->setChecked(true);
        disableAddress();
    }

    setConfigLocation(location);

    connect(locationConfUi.checkPushButton, SIGNAL(pressed()), this, SLOT(checkLocation()));

    locationConfUi.rangeSpinBox->setValue(Settings::range());
    locationConfUi.rangeSpinBox->setMinimum(0.1);
    locationConfUi.rangeSpinBox->setMaximum(5.0);
}

void Location::disableAddress()
{
    locationConfUi.countryCodeLineEdit->setDisabled(true);
    locationConfUi.countryLineEdit->setDisabled(true);
    locationConfUi.cityLineEdit->setDisabled(true);
    locationConfUi.addressLineEdit->setDisabled(true);
}

void Location::disableCoordinates()
{
    locationConfUi.latitudeLineEdit->setDisabled(true);
    locationConfUi.longitudeLineEdit->setDisabled(true);
}

void Location::setConfigLocation(Plasma::DataEngine::Data loc)
{
    locationConfUi.countryCodeLineEdit->setText(loc["country code"].toString());
    locationConfUi.countryLineEdit->setText(loc["country"].toString());
    locationConfUi.cityLineEdit->setText(loc["city"].toString());

    if (!(loc["address"].toString().isEmpty() && !locationConfUi.addressLineEdit->text().isEmpty()))
        locationConfUi.addressLineEdit->setText(loc["address"].toString());

    if (loc["latitude"].toString() != "" && loc["longitude"].toString() != "")
    {
        locationConfUi.latitudeLineEdit->setText(loc["latitude"].toString());
        locationConfUi.longitudeLineEdit->setText(loc["longitude"].toString());
    }
}

void Location::configChanged()
{
    if (locationConfUi.geolocationRadioButton->isChecked())
    {
        Settings::setSource(Settings::EnumSource::GeolocationDataengine);
    }
    else if (locationConfUi.manualRadioButton->isChecked())
    {
        Settings::setSource(Settings::EnumSource::Manual);

        if (locationConfUi.PlaceRadioButton->isChecked())
            Settings::setManualSelection(Settings::EnumManualSelection::Place);
        else if (locationConfUi.coordinatesRadioButton->isChecked())
            Settings::setManualSelection(Settings::EnumManualSelection::Coordinates);

        Settings::setCountryCode(locationConfUi.countryCodeLineEdit->text());
        Settings::setCountry(locationConfUi.countryLineEdit->text());
        Settings::setCity(locationConfUi.cityLineEdit->text());
        Settings::setAddress(locationConfUi.addressLineEdit->text());
        Settings::setLatitude(locationConfUi.latitudeLineEdit->text());
        Settings::setLongitude(locationConfUi.longitudeLineEdit->text());
    }

    Settings::setRange(locationConfUi.rangeSpinBox->value());

    Settings::self()->writeConfig();

    Plasma::DataEngineManager::self()->engine("geoweb")->query("location");
}

void Location::checkLocation()
{
    locationConfUi.checkPushButton->setDisabled(true);
    QString urlstring;

    if (locationConfUi.PlaceRadioButton->isChecked())
    {
        urlstring = "http://maps.google.com/maps/api/geocode/xml?address=" + locationConfUi.addressLineEdit->text().replace(" ", "+") +
                    ",+" + locationConfUi.cityLineEdit->text().replace(" ", "+") +
                    ",+" + locationConfUi.countryLineEdit->text().replace(" ", "+") + ",+" +
                    locationConfUi.countryCodeLineEdit->text().replace(" ", "+") + "&sensor=false";
    }
    else if (locationConfUi.manualRadioButton->isChecked())
    {
        urlstring = "http://maps.google.com/maps/api/geocode/xml?latlng=" + locationConfUi.latitudeLineEdit->text() +
                      "," + locationConfUi.longitudeLineEdit->text() + "&sensor=false";
    }

    url = QUrl(urlstring);
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
}

void Location::redirect(QUrl url)
{
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
}

void Location::httpFinished()
{
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error())
    {
        QMessageBox::information(configParent, I18N_NOOP("HTTP"), I18N_NOOP("Check failed: ") + reply->errorString());
        locationConfUi.checkPushButton->setEnabled(true);
    }
    else if (!redirectionTarget.isNull())
    {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(configParent, I18N_NOOP("HTTP"), I18N_NOOP("Redirect to ") + newUrl.toString() + " ?",
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            url = newUrl;
            reply->deleteLater();
            redirect(url);
            return;
        }
    }
    else
    {
        locationConfUi.checkPushButton->setEnabled(true);

        // parse response XML
        GeocodeHandler handler(configParent, locationConfUi.coordinatesRadioButton->isChecked());
        QXmlSimpleReader reader;
        reader.setContentHandler(&handler);
        reader.setErrorHandler(&handler);

        QXmlInputSource xmlInputSource(reply);
        if (reader.parse(xmlInputSource))
        {
            setConfigLocation(handler.getData());
        }
    }

    reply->deleteLater();
    reply = 0;
}

#include "plasma-geoweb-location.moc"
