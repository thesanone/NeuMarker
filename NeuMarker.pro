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

SOURCES += \
    src/main.cpp\
    src/mainwindow.cpp \
    src/core.cpp \
    src/add_pre_processing_pict.cpp \
    src/slice_item_review.cpp \
    src/add_new_cat.cpp \
    src/neuronsdetection_lab.cpp \
    src/neuronsmarker.cpp \
    src/mouselistener_forqgraphicspixmapitem.cpp \
    src/algoritm_settings.cpp \
    src/endlessprogressbar.cpp \
    src/labfilterinit.cpp \
    src/TotalSettings.cpp

HEADERS  += \
    include/mainwindow.h \
    include/core.h \
    include/add_pre_processing_pict.h \
    include/slice_item_review.h \
    include/add_new_cat.h \
    include/neuronsdetection_lab.h \
    include/neuronsmarker.h \
    include/wheelevent_forqsceneview.h \
    include/mouselistener_forqgraphicspixmapitem.h \
    include/algoritm_settings.h \
    include/synchro_qsceneview.h \
    include/endlessprogressbar.h \
    include/bigimageloader.h \
    include/labfilterinit.h \
    include/TotalSettings.h

FORMS    += \
    ui/mainwindow.ui \
    ui/add_pre_processing_pict.ui \
    ui/slice_item_review.ui \
    ui/add_new_cat.ui \
    ui/algoritm_settings.ui \
    ui/TotalSettings.ui

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
            -lopencv_imgproc #\
            #-lopencv_imgcodecs
}
