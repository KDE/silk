#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Module containing function for distance calculations."""

import math


def distance(lat1, long1, lat2, long2, units="meters"):
    """Compute distance 2 given places in given units.

    source http://www.johndcook.com/python_longitude_latitude.html

    @param lat1: latitude of the first point
    @type lat1: str
    @param long1: longitude of the first point
    @type long1: str
    @param lat2: latitude of the second point
    @type lat2: str
    @param long2: longitude of the second point
    @type long2: str
    @param units: units of result, can be: miles, km or meters;
        meters are default
    @type units: str
    @return: distance in given units
    @rtype: float
    """

    # Convert str to float
    lat1 = float(lat1)
    long1 = float(long1)
    lat2 = float(lat2)
    long2 = float(long2)

    # Convert latitude and longitude to
    # spherical coordinates in radians.
    degrees_to_radians = math.pi / 180.0

    # phi = 90 - latitude
    phi1 = (90.0 - lat1) * degrees_to_radians
    phi2 = (90.0 - lat2) * degrees_to_radians

    # theta = longitude
    theta1 = long1 * degrees_to_radians
    theta2 = long2 * degrees_to_radians

    # Compute spherical distance from spherical coordinates.

    # For two locations in spherical coordinates
    # (1, theta, phi) and (1, theta, phi)
    # cosine( arc length ) =
    #    sin phi sin phi' cos(theta-theta') + cos phi cos phi'
    # distance = rho * arc length

    cos = (math.sin(phi1) * math.sin(phi2) * math.cos(theta1 - theta2) +
           math.cos(phi1) * math.cos(phi2))
    arc = math.acos(cos)

    # Remember to multiply arc by the radius of the earth
    # in your favorite set of units to get length.
    # Radius as is defined by WGS 84
    # Miles
    if units == "miles":
        radius_in_units = 3963.19059
    # Kilemetres
    elif units == "km":
        radius_in_units = 6378.137
    # Metres
    else:
        radius_in_units = 6378137.0

    return arc * radius_in_units
