#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Module containing class implementing ContentHandler for OSM data."""

from PyQt4.QtXml import QXmlDefaultHandler

from place import Place
from places import Places


class OSMHandler(QXmlDefaultHandler):

    """Class processing XML data from OSM service and holding places.

    Class is procesing data from xapi.openstreetmap.org and from
    nominatim.openstreetmap.org.
    """

    def __init__(self, types, location, parent=None):
        """Initialize Places.

        @param types: list of allowed types of places
        @type types: list
        @param location: location from main aplication, needed for fall back
        @type location: Location
        @param parent: parent object
        @type parent: OSM
        """
        QXmlDefaultHandler.__init__(self)
        self._location = location
        self._parent = parent
        self._place = ""
        self._places = Places()
        self._valid_types = types
        self._error_str = ""
        self._reverse_geo = False
        self._current_text = ""
        self._querystring = ""
        self._osm_id = 0

    def startElement(self, namespaceURI, localName, name, attrs):
        """Called on every start element of XML file.

        On elements from xapi creates Place object and set attributes.
        On elements from nominatim sets display_name(address) and if place
        is valid, add it to the Places.
        """
        # Node from xapi - create Place
        if name == "node":
            self._place = Place(str(attrs.value("id")))
            self._place.set_lat(str(attrs.value("lat")))
            self._place.set_lon(str(attrs.value("lon")))
        # Tag from xapi - assign name and type
        elif name == "tag":
            if attrs.value("k") == "name":
                self._place.set_name(unicode(attrs.value("v"), "utf-8"))
            elif attrs.value("k") == "amenity":
                if str(attrs.value("v")) in self._valid_types:
                    self._place.set_amenity(str(attrs.value("v")))
            elif attrs.value("k") == "website":
                self._place.set_website(unicode(attrs.value("v"), "utf-8"))
            elif attrs.value("k") == "cuisine":
                self._place.set_cuisine(unicode(attrs.value("v"), "utf-8"))
            elif attrs.value("k") == "opening_hours":
                self._place.set_opening_hours(unicode(attrs.value("v"),
                "utf-8"))
        # Place from nominatim - set display_name
        elif name == "reversegeocode":
            self._reverse_geo = True
            self._querystring = attrs.value("querystring")
        elif name == "result":
            if self._reverse_geo:
                self._osm_id = str(attrs.value("osm_id"))
        self._current_text = ""
        return True

    def endElement(self, namespaceURI, localName, name):
        """Called on every end element of XML file."""
        # When we are at the end of node from xapi add place to places.
        if name == "node" and self._place.is_valid():
            self._places.add(self._place)
        # End of data from nominatim
        elif name == "result":
            if self._reverse_geo:
                self._places.set_display_name(self._osm_id, self._current_text)
                # Send places to dataengine
                self._parent.send_place(self._osm_id)
        elif name == "reversegeocode":
            self._reverse_geo = False
            self._osm_id = 0
        elif name == "error":
            if self._reverse_geo:
                self._error_str = self._current_text
                return False
        return True

    def characters(self, string):
        """Remember XML values."""
        self._current_text += unicode(string, "utf-8")
        return True

    def fatalError(self, exception):
        """Called in case of error.

        If nominatim can't find the id, try fall back mode.
        """
        if self._error_str == "OSM ID Not Found":
            try:
                (rest, osm_id) = str(self._querystring).rsplit("=", 1)
                self._places.set_display_name(osm_id, "%s, %s, %s" % \
                (self._places.search(osm_id).get_name(),
                self._location.getCity(), self._location.getCountry()), True)
                self._parent.send_place(osm_id)
            except Exception:
                pass
            return True
        raise Exception(self._error_str)

    def errorString(self):
        """Returns error string.

        @return: error string
        @rtype:str
        """
        return self._error_str

    def get_places(self):
        """Returns places.

        @return: places
        @rtype: Places
        """
        return self._places
