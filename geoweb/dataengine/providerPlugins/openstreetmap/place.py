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
        self._distance = ""
        self._website = ""
        self._cuisine = ""
        self._opening_hours = ""

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

    def is_complete(self):
        """Checks if place is complete.

        Place is complete if is valid and has set display_name. Validity and
        completness must be divided into 2 methods because place is not
        complete when is addded to places - when is checked for validity.

        @return True of False
        @rtype: bool
        """
        if self.is_valid() and len(self._display_name) != 0:
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
        if len(name) == 0 or name.isspace():
            raise Exception("InvalidDisplayName")
        self._display_name = name

    def set_distance(self, dist):
        """Sets distance of place from current location.

        @param dist: sitance
        @type dist: str
        """
        try:
            float(dist)
            self._distance = dist
        except ValueError:
            raise Exception("InvalidDistance")

    def set_website(self, url):
        """Sets website of place.

        @param url: url of website
        @type url: str
        """
        if len(url) != 0:
            self._website = url

    def set_cuisine(self, cuis):
        """Sets cuisine for place.

        @param cuis: cuisine
        @type cuis: str
        """
        if len(cuis) != 0:
            self._cuisine = cuis.title()

    def set_opening_hours(self, hours):
        """Sets opening hours for place

        @param hours: opening hours
        @type hours: str
        """
        if len(hours) != 0:
            self._opening_hours = hours

    def get_osm_id_key(self):
        """Returns key of the place name to be shown in datasource.

        Key consists of its amenity and id.

        @return: amenity_id
        @rtype: str
        """
        return "osm_%s" % self._osm_id

    def get_type_id(self):
        """ID of type information

        @return: type id
        @rtype: str
        """
        return "type_%s" % self._osm_id

    def get_latlng_id(self):
        """ID of latitude and longitude

        @return: latlng id
        @rtype: str
        """
        return "latlng_%s" % self._osm_id

    def get_distance_id(self):
        """ID of distance

        @return: distance id
        @rtype: str
        """
        return "dist_%s" % self._osm_id

    def get_url_id(self):
        """ID of url

        @return url id
        @rtype: str
        """
        return "url_%s" % self._osm_id

    def get_website_id(self):
        """ID of website

        @return: website url id
        @rtype: str
        """
        return "web_%s" % self._osm_id

    def get_cuisine_id(self):
        """ID of cuisine

        @return cuisine id
        @rtype: str
        """
        return "cuisine_%s" % self._osm_id

    def get_opening_hours_id(self):
        """ID for opening hours

        @return: opening hours id
        @rtype: str
        """
        return "openingh_%s" % self._osm_id

    def get_type(self):
        """Returns type of the place

        @return: type of place
        @rtype: str
        """
        return self._amenity

    def get_latlng(self):
        """Returns latitude and longitude of place as one string

        @return: latitude and longitude
        @rtype: str
        """
        return "%s,%s" % (self._lat, self._lon)

    def get_url(self):
        """Returns url to the place on open street maps.

        @return: url to place
        @rtype: str
        """
        return "http://www.openstreetmap.org/index.html?" \
        "mlat=%s&mlon=%s&zoom=18" % (self._lat, self._lon)

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
        """Returns place display name.

        @return: display name
        @rtype: str
        """
        return self._display_name

    def get_distance(self):
        """Returns distance of place from current location rounded to int.

        @return: distance
        @rtype: str
        """
        return "%i" % round(self._distance)

    def get_website(self):
        """Returns place website

        @return: url of website
        @rtype: str
        """
        return self._website

    def get_cuisine(self):
        """Returns cuisine

        @return: cuisine
        @rtype: str
        """
        return self._cuisine

    def get_opening_hours(self):
        """Returns opening hours

        @return: opening hours
        @rtype: str
        """
        return self._opening_hours
