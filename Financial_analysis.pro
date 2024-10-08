QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

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

LIBS += -LC:\curl-8.9.1_1-win64-mingw\bin -lcurl
LIBS += -LC:\curl-8.9.1_1-win64-mingw\lib -lcurl
LIBS += -lcrypto
LIBS += -lws2_32

INCLUDEPATH += C:\curl-8.9.1_1-win64-mingw\include


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
    src/news.cpp \
    src/qcustomplot.cpp

HEADERS += \
    include/add_action.h \
    include/add_rate.h \
    include/autorization.h \
    include/converter.h \
    include/credit.h \
    include/currencies.h \
    include/del_action.h \
    include/add_action.h \
    include/del_action.h \
    include/income_and_expenses.h \
    include/mainwindow.h \
    include/news.h \
    include/qcustomplot.h

FORMS += \
    ui/add_action.ui \
    ui/add_rate.ui \
    ui/del_action.ui \
    ui/mainwindow.ui

TRANSLATIONS += \
    Financial_analysis_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
