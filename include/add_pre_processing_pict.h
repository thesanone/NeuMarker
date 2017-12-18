#ifndef ADD_PRE_PROCESSING_PICT_H
#define ADD_PRE_PROCESSING_PICT_H

#include <QDialog>
#include <core.h>
#include <QAbstractButton>
#include "endlessprogressbar.h"
#include "bigimageloader.h"

namespace Ui {
class add_pre_processing_pict;
}

class add_pre_processing_pict : public QDialog
{
    Q_OBJECT

public:
    explicit add_pre_processing_pict(QWidget *parent = 0, Core* c = 0);
    ~add_pre_processing_pict();

    void loadImages(QString path, QStringList files);

public slots:
    void reset_form(QString animalType, QString animal, QString segmentID);

private slots:

    void update_form();

    void resizeEvent(QResizeEvent * );

    void on_add_and_next_pushButton_clicked();

    void on_Ignore_this_image_pushButton_clicked();

    void on_close_appp_pushButton_clicked();

    void clearPict();

    void on_animalType_comboBox_currentTextChanged(const QString &arg1);

private:
    int counter;
    QPixmap* pict;
    Ui::add_pre_processing_pict *ui;
    QString way2folder;
    QStringList filesInFolder;

    EndlessProgressBar *progressBar;
    BigImgLoader *imageLoader;
    QThread *imageLoaderThread;

    Core* core;
};

#endif // ADD_PRE_PROCESSING_PICT_H
