######################################################################
# This will only build the slicewidget and the graphicswidget
#  
# You most likely want to use the cmake-based build. 
######################################################################

TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += /home/rich/src/project-silk/webslice/. .
QT += webkit

# Input
HEADERS += slicewidget.h slicegraphicswidget.h
SOURCES += main.cpp slicewidget.cpp slicegraphicswidget.cpp
