#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Example plugin.

Plugin shows how to use Provider and Location module and demonstrates
writing plugins for Geoweb.
New plugins are simply added by creating new folder in providerPlugins
directory. Name of the plugin is get from folder name. Main file in your new
directory has to be called plugin.ext, where ext has to be correctly set
according to language used (py, rb, js).
For managing plugins is used KDE Kross.
"""

import Provider
import Location

import inspect

# PLUGIN_NAME is folder name of the plugin. Can be set manually or
# automatically with inspect in init() function.
PLUGIN_NAME = ""


def init():
    """Function called when plugin is created.

    No data should be set to Provider in this function because Location don't
    have to provide valid location data yet.
    """
    global PLUGIN_NAME
    PLUGIN_NAME = inspect.currentframe().f_code.co_filename


def request():
    """Called when plugin data are requested.

    When this function is called, Location data are valid. In this function
    should be set data for provider.
    Data are set with Provider.serProperty(pluginName, key, value) method. This
    method can by called multiple times, all keys are added to this plugin
    datasource.
    At the end, when all data are set, Provider.done(pluginName) method has to
    be called indicating that all data are ready and set.

    Location methods:
    QString getCountryCode();
    QString getCountry();
    QString getCity();
    QString getLongitude();
    QString getLatitude();

    Provider methods:
    void setProperty(QString source, QString key, QString value);
    void done(const QString source);
    """
    Provider.setProperty(PLUGIN_NAME, "Hello!", "You are in %s." %
        Location.getCity())
    Provider.done(PLUGIN_NAME)
