#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Module containing class implementing ContentHandler for OSM data."""

from xml.sax.handler import ContentHandler

from place import Place
from places import Places


class OSM(ContentHandler):

    """Class processing XML data from OSM service and holding places.

    Class is procesing data from xapi.openstreetmap.org and from
    nominatim.openstreetmap.org.
    """

    def __init__(self, types):
        """Initialize Places.

        @param types: list of allowed types of places
        @type types: list
        """
        ContentHandler.__init__(self)
        self._place = ""
        self._places = Places()
        self._valid_types = types

    def startElement(self, name, attrs):
        """Called on every start element of XML file.

        On elements from xapi creates Place object and set attributes.
        On elements from nominatim sets display_name(address) and if place
        is valid, add it to the Places.
        """
        # Node from xapi - create Place
        if name == "node":
            self._place = Place(attrs.getValue("id"))
            self._place.set_lat(attrs.getValue("lat"))
            self._place.set_lon(attrs.getValue("lon"))
        # Tag from xapi - assign name and type
        elif name == "tag":
            if attrs.getValue("k") == "name":
                self._place.set_name(attrs.getValue("v"))
            elif attrs.getValue("k") == "amenity":
                if attrs.getValue("v") in self._valid_types:
                    self._place.set_amenity(attrs.getValue("v"))
        # Place from nominatim - set display_name
        elif name == "place":
            self._places.set_display_name(attrs.getValue("osm_id"),
                    attrs.getValue("display_name"))

    def endElement(self, name):
        """Called on every end element of XML file."""
        # When we are at the end of node from xapi add place to places.
        if (name == "node") and self._place.is_valid():
            self._places.add(self._place)

    def get_places(self):
        """Returns places.

        @return: places
        @rtype: Places
        """
        return self._places
