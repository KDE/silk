#!/usr/bin/env kross
# -*- coding: utf-8 -*-

"""Plugin for Geoweb.

Getting places from Open Street Map.
"""

import Provider
import Location

from osm import OSM


PLUGIN = None


def init():
    """Init plugin. Called only once."""
    global PLUGIN
    PLUGIN = OSM(Provider, Location)


def request():
    """Called every time when we need an update."""
    PLUGIN.start()
