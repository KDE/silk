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

    def set_display_name(self, osm_id, name):
        """Set display name of place with given id.

        @param osm_id: id of place to change display name
        @type osm_id: int
        @param name: new display name of object
        @type name: str
        """
        if osm_id not in self:
            raise Exception("NonexistingId")
        if len(name) == 0:
            raise Exception("InvalidDisplayName")
        for place in self._places:
            if place.get_osm_id() == osm_id:
                place.set_display_name(name)
                break
