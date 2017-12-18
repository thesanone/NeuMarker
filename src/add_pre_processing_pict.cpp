#include "add_pre_processing_pict.h"
#include "ui_add_pre_processing_pict.h"
#include <QListWidgetItem>
//#include <QDebug>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QThread>
#include <QPixmapCache>

add_pre_processing_pict::add_pre_processing_pict(QWidget *parent, Core* c) :
    QDialog(parent),
    ui(new Ui::add_pre_processing_pict)
{
    counter = 0;
    core = c;
    pict = NULL;
    progressBar = NULL;
    imageLoader = NULL;
    imageLoaderThread = NULL;

    ui->setupUi(this);

    connect(this, SIGNAL(rejected()), this, SLOT(clearPict()));

    ui->animalType_comboBox->addItems(core->getAnimalsTypesList());

    ui->slizeNumber_lineEdit->setValidator(new QIntValidator(0, 65536, ui->slizeNumber_lineEdit));
}

add_pre_processing_pict::~add_pre_processing_pict()
{
    clearPict();
    delete ui;
}

void add_pre_processing_pict::loadImages(QString path, QStringList files)
{
    way2folder = path;
    filesInFolder = files;
    ui->pict_total_label->setText(QString::number(filesInFolder.size()));
    if(!filesInFolder.empty()) this->update_form();
    else
    {
        QMessageBox::critical(this,"error", "This folder hasn't .jpg files!", QMessageBox::Ok);
        this->reject();
    }
}

void add_pre_processing_pict::reset_form(QString animalType, QString animal, QString segmentID)
{
    ui->animalType_comboBox->setCurrentText(animalType);
    ui->animal_comboBox->setCurrentText(animal);
    ui->segmentID_comboBox->setCurrentText(segmentID);
    ui->full_name_lineEdit->clear();
    ui->slizeNumber_lineEdit->clear();
    ui->position_lineEdit->clear();
    ui->isInvert_checkBox->setChecked(false);
    ui->comment_plainTextEdit->clear();
    counter = 0;
    ui->slize_image_label->clear();
}

void add_pre_processing_pict::update_form()
{
    QString file_name = way2folder + "/" + filesInFolder.front();
    ui->full_name_lineEdit->setText(file_name);
    ++counter;
    ui->pict_counter_label->setText(QString::number(counter));

    clearPict();

    bool reCreateLinks = false;
    if(!imageLoaderThread)
    {
        imageLoaderThread = new QThread(this);
        reCreateLinks = true;
    }
    if(!imageLoader)
    {
        imageLoader = new BigImgLoader(NULL);
        imageLoader->moveToThread(imageLoaderThread);
        reCreateLinks = true;
    }
    if(!progressBar)
    {
        progressBar = new EndlessProgressBar("The image is loading, please wait", this);
        reCreateLinks = true;
    }

    if(reCreateLinks)
    {
        //imageLoader->moveToThread(imageLoaderThread);
        connect(imageLoaderThread, SIGNAL(started()), imageLoader, SLOT(start()));
        connect(imageLoader, SIGNAL(imageLoaded()), imageLoaderThread, SLOT(quit()));
        connect(imageLoader, SIGNAL(imageLoaded()), progressBar, SLOT(reject()));
    }

    imageLoader->setFileName(file_name);
    imageLoaderThread->start();
    progressBar->exec();
    pict = imageLoader->getImagePointer();

    ui->slize_image_label->setPixmap(pict->scaled(ui->slize_image_label->size(), Qt::KeepAspectRatio));
    filesInFolder.pop_front();
}

void add_pre_processing_pict::resizeEvent(QResizeEvent *)
{
    if(pict)
        ui->slize_image_label->setPixmap(
                    pict->scaled(ui->slize_image_label->size(), Qt::KeepAspectRatio));
}

void add_pre_processing_pict::on_add_and_next_pushButton_clicked()
{
    //if(ui->slizeNumber_lineEdit->text()!="")
    //{
    if(pict)
    {
        int res = core->add_pre_processing_pict(
                    ui->animalType_comboBox->currentText(),
                    ui->animal_comboBox->currentText().toInt(),
                    ui->segmentID_comboBox->currentText(),
                    ui->full_name_lineEdit->text(),
                    ui->markerType_lineEdit->text(),
                    pict->scaled(300,200, Qt::KeepAspectRatio),
                    ui->isInvert_checkBox->isChecked(),
                    ui->slizeNumber_lineEdit->text().toInt(),
                    ui->position_lineEdit->text().toInt(),
                    pict->width(),
                    pict->height(),
                    ui->comment_plainTextEdit->toPlainText());
        switch(res) {
            case -1:
                QMessageBox::critical(this,"Error!", "SQLite addition error :(\nCheck your write priveleges", QMessageBox::Retry);
                break;
            case 0:
                QMessageBox::warning(this,"error", "Slice with number #"+ui->slizeNumber_lineEdit->text()+" alredy added for this cat", QMessageBox::Cancel);
                break;
            case 1:
                if(!filesInFolder.empty()) this->update_form();
                    else this->reject();
                break;
        }

        /*{
            if(!filesInFolder.empty()) this->update_form();
            else this->reject();
        }
        else
            QMessageBox::warning(this,"error", "Slice with number #"+ui->slizeNumber_lineEdit->text()+" alredy added for this cat", QMessageBox::Retry);*/
    }
    //}
    //else
    //    QMessageBox::warning(this,"error", "Slice number emty", QMessageBox::Retry);
}

void add_pre_processing_pict::on_Ignore_this_image_pushButton_clicked()
{
    if(!filesInFolder.empty()) this->update_form();
    else this->reject();
}

void add_pre_processing_pict::on_close_appp_pushButton_clicked()
{
    this->reject();
}

void add_pre_processing_pict::clearPict()
{
    if(pict)
    {
        QPixmapCache::clear();
        delete pict;
        pict = NULL;
    }
}

void add_pre_processing_pict::on_animalType_comboBox_currentTextChanged(const QString &arg1)
{
    ui->animal_comboBox->clear();
    ui->segmentID_comboBox->clear();

    ui->animal_comboBox->addItems(core->getAnimalsList(arg1));
    ui->segmentID_comboBox->addItems(core->getSegmentsList(arg1));
}
