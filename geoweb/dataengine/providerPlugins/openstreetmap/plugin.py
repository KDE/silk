#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Plugin for Geoweb.

Getting places from Open Street Map.
"""

import Provider
import Location

import math
import inspect
from xml.sax import make_parser

from osm import OSM


PLUGIN_NAME = ""
# Range in KM around position.
RANGE = 0.5

# Acceptable types of places.
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


def init():
    """Sets PLUGIN_NAME."""
    global PLUGIN_NAME
    PLUGIN_NAME = inspect.currentframe().f_code.co_filename


def request():
    """Gets data from OSM, process them and set to Provider."""
    # Surrounding box.
    bottom, left, top, right = box(float(Location.getLatitude()),
            float(Location.getLongitude()), RANGE)
    # Make from acceptable types string for OSM
    amenity = ""
    for i in AMENITY_SUSTENANCE:
        amenity += "%s|" % i
    args = "/api/0.6/node[amenity=%s][bbox=%f,%f,%f,%f]" % \
            (amenity, left, bottom, right, top)
    # Create a parser
    parser = make_parser()
    # Create the handler
    osm = OSM(AMENITY_SUSTENANCE)
    # Tell the parser to use our handler
    parser.setContentHandler(osm)
    # Parse the input
    try:
        parser.parse("http://osmxapi.hypercube.telascience.org%s" % args)
        places = osm.get_places()
    except Exception:
        print "Error: failed to get data from: " \
        "http://osmxapi.hypercube.telascience.org"
        return
    # When we got all places, get full address from nominatim for each place.
    for place in places:
        try:
            args = "/search?format=xml&q=%s,%s,%s" % \
                    (place.get_name().encode("utf-8").replace(" ", "%20"),
                    Location.getCity().replace(" ", "%20"),
                    Location.getCountry().replace(" ", "%20"))
            parser.parse("http://nominatim.openstreetmap.org%s" % args)
            print "%s, %s" % (place.get_name(), place.get_amenity())
        except Exception:
            continue
    # Send data to Provider.
    for place in places:
        Provider.setProperty(PLUGIN_NAME, place.get_id_key(),
                place.get_display_name())
    Provider.done(PLUGIN_NAME)


def box(lat, lon, radius):
    """Creates surrounding box.

    Returns two lat/lon pairs as (lat1, lon1, lat2, lon2) which define a box
    that exactly surrounds a circle of radius of the given amount
    in kilometres.
    Source:
    http://www.zachary.com/s/blog/2005/01/12/python_zipcode_geo-programming

    @param lat: latitude
    @type lat: float
    @param lon: longitude
    @type lon: float
    @param radius: radius
    @type radius: float
    @return: (lat1, lon1, lat2, lon2)
    @rtype: tuple
    """
    miles_per_nautical_mile_60 = 1.15078 * 60.0
    rad = math.pi / 180.0
    radius *= 1.609344

    lat_range = radius / miles_per_nautical_mile_60
    lon_range = radius / (math.cos(lat * rad) * miles_per_nautical_mile_60)

    return (lat - lat_range, lon - lon_range, lat + lat_range, lon + lon_range)
