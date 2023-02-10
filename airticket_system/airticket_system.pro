QT       += core gui
QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    a320_seattable.cpp \
    a320_seattable_2.cpp \
    a330_seattable.cpp \
    a330_seattable_2.cpp \
    admininterface.cpp \
    adminsearch.cpp \
    b737_seattable.cpp \
    b737_seattable_2.cpp \
    book_ticket.cpp \
    changepassword.cpp \
    checkorder.cpp \
    main.cpp \
    myorder.cpp \
    newuser.cpp \
    orderinfo.cpp \
    passages.cpp \
    search.cpp \
    updateflightinfo.cpp \
    widget.cpp

HEADERS += \
    a320_seattable.h \
    a320_seattable_2.h \
    a330_seattable.h \
    a330_seattable_2.h \
    admininterface.h \
    adminsearch.h \
    b737_seattable.h \
    b737_seattable_2.h \
    book_ticket.h \
    changepassword.h \
    checkorder.h \
    myorder.h \
    newuser.h \
    orderinfo.h \
    passages.h \
    search.h \
    updateflightinfo.h \
    widget.h

FORMS += \
    a320_seattable.ui \
    a320_seattable_2.ui \
    a330_seattable.ui \
    a330_seattable_2.ui \
    admininterface.ui \
    adminsearch.ui \
    b737_seattable.ui \
    b737_seattable_2.ui \
    book_ticket.ui \
    changepassword.ui \
    checkorder.ui \
    myorder.ui \
    newuser.ui \
    orderinfo.ui \
    passages.ui \
    search.ui \
    updateflightinfo.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
