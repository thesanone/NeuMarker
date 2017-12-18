#ifndef LABFILTERINIT_H
#define LABFILTERINIT_H

#include <QObject>
#include <QThread>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "neuronsdetection_lab.h"

class LabFilterInit : public QObject
{
    Q_OBJECT
public:
    explicit LabFilterInit(QObject *parent = 0);
    virtual ~LabFilterInit();

    neuronsDetection_lab *getFilter();

    QString getFileName() const;
    void setFileName(const QString &value);

signals:
    void imageLoaded();

public slots:
    void start();

private:
    QString fileName;
    neuronsDetection_lab *filter;
};

#endif // LABFILTERINIT_H
