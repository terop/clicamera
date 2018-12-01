TEMPLATE = app
CONFIG += c++11
CONFIG += qt

QT += core gui multimedia multimediawidgets

SOURCES += \
    main.cpp \
    camera.cpp

DISTFILES += \
    rpm/clicamera.yaml

HEADERS += \
    camera.h
