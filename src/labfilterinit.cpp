#include "labfilterinit.h"

#include <QFileInfo>

LabFilterInit::LabFilterInit(QObject *parent) : QObject(parent)
{
    filter = NULL;
}

LabFilterInit::~LabFilterInit()
{
    //if(filter) delete filter;
}

neuronsDetection_lab *LabFilterInit::getFilter()
{
    return filter;
}

QString LabFilterInit::getFileName() const
{
    return fileName;
}

void LabFilterInit::setFileName(const QString &value)
{
    fileName = value;
}

void LabFilterInit::start()
{
    QFileInfo file(fileName);
    if(file.exists()) filter = new neuronsDetection_lab(cv::imread(fileName.toLocal8Bit().constData()));
    /*{
        QMessageBox::critical(NULL, "File does not exist!", "File " + fileName + " could not be found!");
    }*/
    emit imageLoaded();
}
