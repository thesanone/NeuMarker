#ifndef BIGIMAGELOADER_H
#define BIGIMAGELOADER_H

#include <QObject>
#include <QPixmap>
#include <QThread>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QFileInfo>
#include <QPixmapCache>

class BigImgLoader : public QObject
{
    Q_OBJECT
public:
    BigImgLoader(QObject *parent):QObject(parent)
    {
        imagePointer = NULL;
    }

    QString getFileName() const
    {
        return fileName;
    }

    void setFileName(const QString &value)
    {
        fileName = value;
    }

    QPixmap *getImagePointer()
    {
        return imagePointer;
    }

    /*cv::Mat getMatImage()
    {
        return matImage;
    }*/

public slots:
    void start()
    {
        /*if(imagePointer)
        {
            imagePointer = NULL;
            QPixmapCache::clear();
        }*/
        QFileInfo file(fileName);
        if(file.exists())
            imagePointer = new QPixmap(fileName);
        /*{
            QMessageBox::critical(0, "File does not exist!", "File " + fileName + " could not be found!", QMessageBox::Ok);
        }*/

        emit imageLoaded();
    }

    /*void startm()
    {
        //if(!matImage.empty()) matImage.
        if(fileName.isEmpty()) return;
        matImage = cv::imread(fileName.toLocal8Bit().constData());
        emit matImageLoaded();
    }

   void init(QThread *imageLoaderThread, EndlessProgressBar *progressBar, QObject* parent)
    {
        bool reCreateLinks = false;
        if(!imageLoaderThread)
        {
            imageLoaderThread = new QThread(parent);
            reCreateLinks = true;
        }
        if(!imageLoader)
        {
            imageLoader = new BigImgLoader(NULL);
            reCreateLinks = true;
        }
        if(!progressBar)
        {
            progressBar = new EndlessProgressBar(parent);
            reCreateLinks = true;
        }

        if(reCreateLinks)
        {
            this->moveToThread(imageLoaderThread);
            connect(imageLoaderThread, SIGNAL(started()), this, SLOT(start()));
            connect(this, SIGNAL(imageLoaded()), imageLoaderThread, SLOT(quit()));
            connect(this, SIGNAL(imageLoaded()), progressBar, SLOT(reject()));
        }
    }*/

signals:
    void imageLoaded();
    //void matImageLoaded();

private:
    QString fileName;
    QPixmap *imagePointer;
    //cv::Mat matImage;
};

#endif // BIGIMAGELOADER_H
