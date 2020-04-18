QT       += core gui network

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
    configurationpanel.cpp \
    configurationpanelwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    intensitywidget.cpp \
    intensitydrawer.cpp \
    crosscorrelationwidget.cpp \
    dataanalyzerthread.cpp \
    rxusrp.cpp

HEADERS += \
    configurationpanel.h \
    configurationpanelwidget.h \
    mainwindow.h \
    intensitywidget.h \
    intensitydrawer.h \
    crosscorrelationwidget.h \
    common.h \
    dataanalyzerthread.h \
    rxusrp.h

FORMS += \
    configurationpanelwidget.ui \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc


win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files/UHD/lib/' -luhd
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files/UHD/lib/' -luhd
else:unix: LIBS += -L'C:/Program Files/UHD/lib/' -luhd

INCLUDEPATH += 'C:/Program Files/UHD/include'
DEPENDPATH += 'C:/Program Files/UHD/include'
INCLUDEPATH +='C:/Users/Ngan/Downloads/alglib-3.16.0.cpp.gpl/cpp/src'

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_72_0/lib64-msvc-14.1/ -llibboost_thread-vc141-mt-x64-1_72
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_72_0/lib64-msvc-14.1/ -llibboost_thread-vc141-mt-x64-1_72
else:unix: LIBS += -LC:/local/boost_1_72_0/lib64-msvc-14.1/ -llibboost_thread-vc141-mt-x64-1_72

INCLUDEPATH += C:/local/boost_1_72_0
DEPENDPATH += C:/local/boost_1_72_0
