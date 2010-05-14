#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Module containing main class for open street map plugin."""

from PyQt4.QtXml import QXmlSimpleReader, QXmlInputSource

from osmhandler import OSMHandler
from distance import distance
from networkinterface_urllib2 import NetworkInterface
#from networkinterface_asyncore import NetworkInterface
#from networkinterface_qt import NetworkInterface


class OSM():

    """Main class for open street map."""

    # Amenity set for food and drink
    AMENITY_SUSTENANCE = [
        "restaurant",
        "food_court",
        "fast_food",
        "drinking_water",
        "bbq",
        "pub",
        "bar",
        "cafe",
        "biergarten",
    ]

    def __init__(self, provider, location):
        """Constructor.

        @param provider: provider from dataengine, for sending data
        @type provider: Provider
        @param location: location from dataengine
        @type: location: Location
        """
        self._provider = provider
        self._location = location

        self._plugin_name = "openstreetmap"
        self._has_list = False
        self._places = ""
        self._net_if = NetworkInterface(self)

        self._osm_hand = OSMHandler(self.AMENITY_SUSTENANCE,
        self._location, self)
        self._reader = QXmlSimpleReader()
        self._reader.setContentHandler(self._osm_hand)
        self._reader.setErrorHandler(self._osm_hand)

        self._amenity = ""
        for i in self.AMENITY_SUSTENANCE:
            self._amenity += "%s|" % i
        self._amenity = self._amenity.rstrip("|")

    def start(self):
        """Starts the downloading process."""
        url = "http://osmxapi.hypercube.telascience.org" \
        "/api/0.6/node[amenity=%s][bbox=%s]" % \
        (self._amenity, self._location.getBox())

        self._has_list = False
        self._places = None
        self._net_if.download(url)

    def receive_data(self, data):
        """Method called from network interface with new data.

        @param data: downloaded data
        @type data: str
        """
        if not self._has_list:
            self._process_main_list(data)
            return
        source = QXmlInputSource()
        source.setData(data)

        try:
            self._reader.parse(source)
        except Exception:
            pass

    def _process_main_list(self, data):
        """Process main list from xapi.openstreetmap.org

        @param data: downloaded data with main list
        @type data: str
        """
        source = QXmlInputSource()
        source.setData(data)
        self._reader.parse(source)
        self._has_list = True
        self._places = self._osm_hand.get_places()

        if len(self._places) == 0:
            print "No place found."
            return

        for place in self._places:
            try:
                url = "http://nominatim.openstreetmap.org" \
                "/reverse?format=xml&osm_type=N&osm_id=%s" % place.get_osm_id()
                self._net_if.download(url)
                print "%s, %s" % (place.get_name(), place.get_amenity())
            except Exception:
                continue

    def send_place(self, osm_id):
        """Method for sending data to dataengine.

        Called from OSMHandler when place is ready for sending. Before sending
        counts distance from current location to place.

        @param osm_id: id of place
        @type osm_id: str
        """
        # Send place types in categories
        self._provider.setProperty(self._plugin_name,
            "food_and_drink", self._amenity)

        place = self._places.search(osm_id)
        if place.is_complete():
            # Count distance
            try:
                place.set_distance(distance(self._location.getLatitude(),
                self._location.getLongitude(), place.get_lat(),
                place.get_lon()))
                # Send distance
                self._provider.setProperty(self._plugin_name,
                place.get_distance_id(), place.get_distance())
            except Exception:
                pass

            # Send rest of data
            # Main name
            self._provider.setProperty(self._plugin_name,
            place.get_osm_id_key(), place.get_display_name())
            # Type
            self._provider.setProperty(self._plugin_name,
            place.get_type_id(), place.get_type())
            # Latitude and Longitude
            self._provider.setProperty(self._plugin_name,
            place.get_latlng_id(), place.get_latlng())
            # Url
            self._provider.setProperty(self._plugin_name,
            place.get_url_id(), place.get_url())
            # Website
            if place.get_website():
                self._provider.setProperty(self._plugin_name,
                place.get_website_id(), place.get_website())
            # Cuisine
            if place.get_cuisine():
                self._provider.setProperty(self._plugin_name,
                place.get_cuisine_id(), place.get_cuisine())
            # Opening hours
            if place.get_opening_hours():
                self._provider.setProperty(self._plugin_name,
                place.get_opening_hours_id(), place.get_opening_hours())

            self._provider.done(self._plugin_name)
