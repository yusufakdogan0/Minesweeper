QT += core gui widgets

TARGET = Minesweeper
TEMPLATE = app

SOURCES += \
    main.cpp \
    Cell.cpp \
    MineField.cpp

HEADERS += \
    Cell.h \
    MineField.h
    Functions.h

RESOURCES += resources.qrc
