TEMPLATE = app

TARGET = clicamera

CONFIG += c++11 qt

QT += core gui multimedia

SOURCES += \
    main.cpp \
    camera.cpp

DISTFILES += \
    rpm/clicamera.yaml

HEADERS += \
    camera.h
