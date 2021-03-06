QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 static

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    connect_db.cpp \
    create_column.cpp \
    create_table.cpp \
    main.cpp \
    mainwindow.cpp \
    newprofile.cpp \
    script_window.cpp \
    userdialog.cpp \
    rename_column.cpp \
    uncover_columns.cpp

HEADERS += \
    config.h \
    connect_db.h \
    create_column.h \
    create_table.h \
    mainwindow.h \
    newprofile.h \
    script_window.h \
    userdialog.h \
    rename_column.h \
    uncover_columns.h

FORMS += \
    connect_db.ui \
    create_column.ui \
    create_table.ui \
    mainwindow.ui \
    newprofile.ui \
    script_window.ui \
    userdialog.ui \
    rename_column.ui \
    uncover_columns.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc


QXLSX_PARENTPATH=./QXlsx         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./QXlsx/source/  # current QXlsx source path is ./source/
include(./QXlsx/QXlsx.pri)

