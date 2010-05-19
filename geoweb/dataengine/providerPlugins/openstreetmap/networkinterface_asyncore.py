#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Network interface module for OSM plugin implemented with asyncore."""

import socket
import asyncore
import urlparse


class NetworkInterface(asyncore.dispatcher):

    """Class providing network interface for OSM."""

    def __init__(self, parent=None):
        """Constructor.

        @param parent: parent object
        @type parent: OSM
        """
        asyncore.dispatcher.__init__(self)
        self._parent = parent
        self.buffer = None

    def handle_connect(self):
        pass

    def handle_close(self):
        self.close()

    def handle_read(self):
        print self.recv(8192)
         #self._parent.receive_data(data)

    def writable(self):
        return (len(self.buffer) > 0)

    def handle_write(self):
        sent = self.send(self.buffer)
        self.buffer = self.buffer[sent:]

    def download(self, url):
        """Download given url.

        @param url: url address
        @type url: str
        """
        parts = urlparse.urlparse(url)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect((parts[1], 80))
        self.buffer = "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n" % \
        (parts[2], parts[1])
        asyncore.loop()
