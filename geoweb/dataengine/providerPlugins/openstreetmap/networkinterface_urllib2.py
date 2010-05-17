#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Network interface module for OSM plugin implemented with urllib2."""

import urllib2


class NetworkInterface():

    """Class providing network interface for OSM."""

    def __init__(self, parent=None):
        """Constructor.

        @param parent: parent object
        @type parent: OSM
        """
        self._parent = parent

    def download(self, url):
        """Download given url and send data to parent.

        @param url: url
        @type url: str
        """
        try:
            fetch = urllib2.urlopen(url, None, 30)
            data = fetch.read()
            self._parent.receive_data(data)
        except urllib2.URLError:
            raise Exception("Server unreachable.")
