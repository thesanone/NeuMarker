#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T13:19:48
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeuMarker
TEMPLATE = app

CONFIG -= debug_and_release debug_and_release_target

SOURCES += main.cpp\
        mainwindow.cpp \
    core.cpp \
    add_pre_processing_pict.cpp \
    slice_item_review.cpp \
    add_new_cat.cpp \
    neuronsdetection_lab.cpp \
    neuronsmarker.cpp \
    mouselistener_forqgraphicspixmapitem.cpp \
    algoritm_settings.cpp \
    endlessprogressbar.cpp \
    labfilterinit.cpp \
    TotalSettings.cpp

HEADERS  += mainwindow.h \
    core.h \
    add_pre_processing_pict.h \
    slice_item_review.h \
    add_new_cat.h \
    neuronsdetection_lab.h \
    neuronsmarker.h \
    wheelevent_forqsceneview.h \
    mouselistener_forqgraphicspixmapitem.h \
    algoritm_settings.h \
    synchro_qsceneview.h \
    endlessprogressbar.h \
    bigimageloader.h \
    labfilterinit.h \
    TotalSettings.h

FORMS    += mainwindow.ui \
    add_pre_processing_pict.ui \
    slice_item_review.ui \
    add_new_cat.ui \
    algoritm_settings.ui \
    TotalSettings.ui

DESTDIR = $$PWD/Deploy

#win32 {
#    CONFIG(debug, debug|release){
#        LIBS += $$(OPENCV2_PATH)/build/x86/vc12/lib/opencv_core2411d.lib
#        LIBS += $$(OPENCV2_PATH)/build/x86/vc12/lib/opencv_highgui2411d.lib
#        LIBS += $$(OPENCV2_PATH)/build/x86/vc12/lib/opencv_imgproc2411d.lib
#    }
#    else {
#        LIBS += $$(OPENCV2_PATH)/build/x86/vc12/lib/opencv_core2411.lib
#        LIBS += $$(OPENCV2_PATH)/build/x86/vc12/lib/opencv_highgui2411.lib
#        LIBS += $$(OPENCV2_PATH)/build/x86/vc12/lib/opencv_imgproc2411.lib
#    }
#    message(in win32)
windows {
    INCLUDEPATH += $$(OPENCV2_PATH)/build/include

    CONFIG(debug, debug|release){
        LIBS += $$(OPENCV2_PATH)/build/x64/vc12/lib/opencv_core2411d.lib
        LIBS += $$(OPENCV2_PATH)/build/x64/vc12/lib/opencv_highgui2411d.lib
        LIBS += $$(OPENCV2_PATH)/build/x64/vc12/lib/opencv_imgproc2411d.lib
    } else {
        LIBS += $$(OPENCV2_PATH)/build/x64/vc12/lib/opencv_core2411.lib
        LIBS += $$(OPENCV2_PATH)/build/x64/vc12/lib/opencv_highgui2411.lib
        LIBS += $$(OPENCV2_PATH)/build/x64/vc12/lib/opencv_imgproc2411.lib
    }
} else:linux {
    LIBS += -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_imgcodecs
}
