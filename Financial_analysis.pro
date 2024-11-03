QT       += core gui sql charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/add_action.cpp \
    src/add_rate.cpp \
    src/autorization.cpp \
    src/converter.cpp \
    src/credit.cpp \
    src/currencies.cpp \
    src/del_action.cpp \
    src/income_and_expenses.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/news.cpp

HEADERS += \
    include/add_action.h \
    include/add_rate.h \
    include/autorization.h \
    include/converter.h \
    include/credit.h \
    include/currencies.h \
    include/del_action.h \
    include/income_and_expenses.h \
    include/mainwindow.h \
    include/news.h

FORMS += \
    ui/add_action.ui \
    ui/add_rate.ui \
    ui/del_action.ui \
    ui/mainwindow.ui

TRANSLATIONS += \
    Financial_analysis_ru_RU.ts

INCLUDEPATH += include

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
