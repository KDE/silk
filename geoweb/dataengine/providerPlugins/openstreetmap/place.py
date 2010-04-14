#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Contains class for holding information about single place."""


class Place:

    """Class holding information about one place from OSM."""

    def __init__(self, osm_id):
        """Create place with osm_id and set attributes to empty strings.

        @param osm_id: unique id of the place
        @type osm_id: str
        """
        if not osm_id.isdigit():
            raise Exception("InvalidId")
        self._osm_id = osm_id
        self._lat = ""
        self._lon = ""
        self._name = ""
        self._amenity = ""
        self._display_name = ""

    def is_valid(self):
        """Checks if place is valid.

        All attributes except self._display_name has to be set.

        @return: True or False
        @rtype: bool
        """
        if len(self._osm_id) != 0 and len(self._lat) != 0 and \
           len(self._lon) != 0 and len(self._name) != 0 and \
           len(self._amenity) != 0:
            return True
        return False

    def set_lat(self, lat):
        """Sets latitude of the place.

        @param lat: latitude
        @type lat: float
        """
        try:
            float(lat)
            self._lat = lat
        except ValueError:
            raise Exception("InvalidLat")

    def set_lon(self, lon):
        """Sets longitude of the place.

        @param lon: longitude
        @type lon: float
        """
        try:
            float(lon)
            self._lon = lon
        except ValueError:
            raise Exception("InvalidLon")

    def set_name(self, name):
        """Sets name of the place.

        @param name: name
        @type name: str
        """
        if len(name) == 0:
            raise Exception("InvalidName")
        self._name = name

    def set_amenity(self, amn):
        """Sets amenity attribute of the place.

        @param amn: amenity
        @type amn: str
        """
        if len(amn) == 0:
            raise Exception("InvalidAmenity")
        self._amenity = amn

    def set_display_name(self, name):
        """Sets display name (complete address) of the place.

        @param name: display name
        @type name: str
        """
        if len(name) == 0:
            raise Exception("InvalidDisplayName")
        self._display_name = name

    def get_id_key(self):
        """Returns unique key of the place to be shown in datasource.

        Key consists of its amenity and id.

        @return: amenity_id
        @rtype: str
        """
        return "%s_%s" % (self._amenity, self._osm_id)

    def get_osm_id(self):
        """Returns place id.

        @return: osm_id
        @rtype: int
        """
        return self._osm_id

    def get_lat(self):
        """Returns place latitude.

        @return: latitude
        @rtype: float
        """
        return self._lat

    def get_lon(self):
        """Returns place longitude.

        @return: longitude
        @rtype: float
        """
        return self._lon

    def get_name(self):
        """Returns place name.

        @return: name
        @rtype: str
        """
        return self._name

    def get_amenity(self):
        """Returns place amenity.

        @return: amenity
        @rtype: str
        """
        return self._amenity

    def get_display_name(self):
        """Returns place display name (complete address) with amenity.

        @return: amenity display name
        @rtype: str
        """
        return "%s %s" % (self._amenity.title(), self._display_name)
