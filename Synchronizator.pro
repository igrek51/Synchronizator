QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Synchronizator
TEMPLATE = app

SOURCES +=\
    main.cpp \
    app.cpp \
    config.cpp \
    history.cpp \
    io.cpp \
    listbox.cpp \
    program.cpp \
    synchro.cpp \
    controls.cpp \
    files.cpp \
    strings.cpp \
    task.cpp \
    thread.cpp \
    events.cpp

DISTFILES += \
    resource.rc

HEADERS += \
    app.h \
    history.h \
    synchro.h \
    config.h \
    controls.h \
    files.h \
    io.h \
    strings.h \
    task.h \
    thread.h \
    version.h
