cmake_minimum_required(VERSION 2.6)

option(ENABLE_HELLO_PLUGIN "Enable Hello plugin for dataengine" OFF)
option(ENABLE_OPENSTREETMAP_PLUGIN "Enable Open Street Map plugin for dataengine" ON)

option(ENABLE_APPLETS "Enable applets" ON)

option(ENABLE_LOCATION_APPLET "Enable Location applet" ON)
option(ENABLE_HELLO_APPLET "Enable Hello applet (Hello plugin must be enabled too)" OFF)
option(ENABLE_OPENSTREETMAP_APPLET "Enable Open Street Map applet (Open Street Map plugin must be enabled too)" ON)
option(ENABLE_OPENSTREETMAP-MAP_APPLET "Enable Open Street Map applet displaying map." ON)

add_subdirectory(dataengine)
add_subdirectory(applets)