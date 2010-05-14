#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Network interface module for OSM plugin implemented with Qt."""

from PyQt4.QtCore import QObject, QUrl
from PyQt4.QtNetwork import QNetworkAccessManager, QNetworkRequest


class NetworkInterface(QObject):

    """Class providing network interface for OSM."""

    def __init__(self, parent=None):
        """Contructor

        @param parent: parent
        @type parent: OSM
        """
        QObject.__init__(self)
        self._parent = parent
        self._reply = None
        self._qnam = QNetworkAccessManager(self)

    def download(self, url):
        """Download given url and send data to parent.

        @param url: url
        @type url: str
        """
        self._reply = self._qnam.get(QNetworkRequest(QUrl(url)))
        self._reply.finished.connect(self._http_finished)

    def _http_finished(self):
        """Method for receiving data"""
        if self._reply.error():
            raise Exception("DownloadError")
        else:
            print self._reply.readAll()

        self._reply.deleteLater()
        self._reply = 0
