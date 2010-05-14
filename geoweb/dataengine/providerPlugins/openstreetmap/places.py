#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Container for multiple places."""


class Places:

    """Class Places contains list of Place objects.

    Class also implements some additional methods for working with Place
    objects comparing to simple list.
    """

    def __init__(self):
        """Initialize local attributes."""
        self._places = []
        self._index = -1

    def __iter__(self):
        """Method for iteration.

        @return: self
        @rtype: Places
        """
        return self

    def next(self):
        """Returns next place in iteration.

        @return: place
        @rtype: Place
        """
        self._index += 1
        if self._index == len(self._places):
            self._index = -1
            raise StopIteration
        return self._places[self._index]

    def __contains__(self, item):
        """If object contains place with given id.

        @param item: osm_id of place
        @type item: int
        @return: True or False
        @rtype: bool
        """
        if not self._places:
            return False
        for place in self._places:
            if place.get_osm_id() == item:
                return True
        return False

    def __len__(self):
        """Returns number of places.

        @return: number of places
        @rtype: int
        """
        return len(self._places)

    def add(self, place):
        """Add place to object Places.

        @param place: valid place object, must be unique
        @type place: Place
        """
        if place in self:
            raise Exception("DuplicatePlace")
        if not place.is_valid():
            raise Exception("InvalidPlace")
        self._places.append(place)

    def set_display_name(self, osm_id, name, fall_back=False):
        """Set display name of place with given id.

        Fall back mode means that search in nominatim failed, so display name
        is improvised from normal name, place and city. Difference is that
        we need to check for duplicates in this mode.

        @param osm_id: id of place to change display name
        @type osm_id: int
        @param name: new display name of object
        @type name: str
        @param fall_back: if display name is set from fall back mode
        @type fall_back: bool
        """
        if fall_back:
            if self._check_for_duplicate_name(osm_id, name):
                raise Exception("DuplicatePlace")
        if osm_id not in self:
            raise Exception("NonexistingId")
        for place in self._places:
            if place.get_osm_id() == osm_id:
                place.set_display_name(name)
                return

    def search(self, osm_id):
        """Search for place with given id.

        @param osm_id: id of place we want to search
        @type osm_id: str
        @return: place we are searching for
        @rtype: Places
        """
        for place in self._places:
            if place.get_osm_id() == osm_id:
                return place

    def _check_for_duplicate_name(self, osm_id, name):
        """Check for duplicate places in fall back mode.

        Duplicate places are when display names and amenity are equal and
        id differs.

        @param osm_id: id of place
        @type osm_id: str
        @param name: display name of place
        @type name: str
        @return: True if given place is duplicate, False otherwise
        @rtype: bool
        """
        for place in self._places:
            if place.get_display_name() == name and \
            place.get_osm_id() != osm_id and \
            place.get_amenity() == self.search(osm_id).get_amenity():
                return True
        return False
