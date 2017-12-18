#include "slice_item_review.h"
#include "ui_slice_item_review.h"

#include <QListWidgetItem>
#include <QMessageBox>

slice_item_review::slice_item_review(QWidget *parent, Core *c) :
    QDialog(parent),
    ui(new Ui::slice_item_review)
{
    core = c;
    ui->setupUi(this);
    connect(ui->cancel_pushButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

   /* std::vector<QListWidgetItem*> items = core->getCatsQListWidgetItems();
    for(std::vector<QListWidgetItem*>::iterator i = items.begin(); i != items.end(); i++)
        catIdVector.push_back((*i)->text());

    items.clear();
    items = core->getSegmentsQListWidgetItems();
    for(std::vector<QListWidgetItem*>::iterator i = items.begin(); i != items.end(); i++)
        segmentsVector.push_back((*i)->text());

    ui->segmentID_siv_comboBox->addItems(segmentsVector);
    ui->catID_siv_comboBox->addItems(catIdVector);*/

    ui->animalType_siv_comboBox->addItems(core->getAnimalsTypesList());

    ui->slizeNumber_lineEdit->setValidator(new QIntValidator(0, 65536, ui->slizeNumber_lineEdit));
}

void slice_item_review::set_pre_processing_id(int id)
{
    pre_processing_id = id;
    SingleSlice item = core->selectSliceByID(pre_processing_id);
    ui->full_name_lineEdit->setText(item.fullName);
    ui->animalType_siv_comboBox->setCurrentText(item.animalType);
    ui->animals_siv_comboBox->setCurrentText(QString::number(item.animalID));
    ui->segments_siv_comboBox->setCurrentText(item.segmentID);
    ui->markerType_siv_lineEdit->setText(item.markerType);
    ui->slizeNumber_lineEdit->setText(QString::number(item.sliceNumber));
    ui->position_lineEdit->setText(QString::number(item.position));
    ui->isInvert_checkBox->setChecked(item.isInvert);
    ui->comment_plainTextEdit->setPlainText(item.comment);
    ui->slice_image_label->setPixmap(QPixmap::fromImage(item.pict));

    ui->apply_changes_pushButton->setEnabled(false);
    ui->process_selected_experiment_pushButton->setEnabled(false);
    ui->delete_selected_experiment_pushButton->setEnabled(false);
    ui->upload_selected_experiment_pushButton->setEnabled(false);

    this->download_experiments_list();
}

/*void slice_item_review::update_cat_list(QString cat)
{
    catIdVector.clear();
    std::vector<QListWidgetItem*> items = core->getCatsQListWidgetItems();
    for(std::vector<QListWidgetItem*>::iterator i = items.begin(); i != items.end(); i++)
        catIdVector.push_back((*i)->text());
    catIdVector.push_back(cat);
    ui->catID_siv_comboBox->clear();
    ui->catID_siv_comboBox->addItems(catIdVector);
}*/

void slice_item_review::download_experiments_list()
{
    /*ui->experiments_listWidget->clear();
    std::vector<QListWidgetItem *> items =  core->getDetectionsResultsListBySliceID(pre_processing_id);
    for(std::vector<QListWidgetItem*>::iterator i = items.begin(); i != items.end(); i++)
        ui->experiments_listWidget->insertItem(0,*i);*/
    ui->neuDetectionResults_treeWidget->clear();
    QList<SingleNeuDetectionResult> items = core->getDetectionsResultsListBySliceID(pre_processing_id);
    for(QList<SingleNeuDetectionResult>::iterator i = items.begin(); i != items.end(); i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->neuDetectionResults_treeWidget);
        item->setData(0, Qt::UserRole, (*i).id);
        item->setText(0, (*i).cellsGroupName);
        item->setText(1, QString::number((*i).neuFound));
        item->setText(2, (*i).method);
        item->setText(3, (*i).accomplished);
    }
}

slice_item_review::~slice_item_review()
{
    delete ui;
}


void slice_item_review::on_apply_changes_pushButton_clicked()
{
    if(core->updateSliceString(pre_processing_id,
                               ui->animalType_siv_comboBox->currentText(),
                               ui->animals_siv_comboBox->currentText().toInt(),
                               ui->segments_siv_comboBox->currentText(),
                               ui->full_name_lineEdit->text(),
                               ui->markerType_siv_lineEdit->text(),
                               ui->isInvert_checkBox->isChecked(),
                               ui->slizeNumber_lineEdit->text().toInt(),
                               ui->position_lineEdit->text().toInt(),
                               ui->comment_plainTextEdit->toPlainText()) == 1)
    {
        ui->apply_changes_pushButton->setEnabled(false);
    }
    else
        QMessageBox::warning(this,"error", "Can't update", QMessageBox::Ok);
}

void slice_item_review::on_slizeNumber_lineEdit_textChanged(const QString)
{
    ui->apply_changes_pushButton->setEnabled(true);
}

void slice_item_review::on_position_lineEdit_textChanged(const QString)
{
    ui->apply_changes_pushButton->setEnabled(true);
}

void slice_item_review::on_isInvert_checkBox_clicked()
{
    ui->apply_changes_pushButton->setEnabled(true);
}

void slice_item_review::on_comment_plainTextEdit_textChanged()
{
    ui->apply_changes_pushButton->setEnabled(true);
}

/*void slice_item_review::on_experiments_listWidget_itemPressed(QListWidgetItem *)
{
    ui->process_selected_experiment_pushButton->setEnabled(true);
    ui->delete_selected_experiment_pushButton->setEnabled(true);
    ui->upload_selected_experiment_pushButton->setEnabled(true);
}
*/
void slice_item_review::on_new_experiment_pushButton_clicked()
{
    emit go_process_pre_processing_by_id(pre_processing_id);
    this->reject();
}

void slice_item_review::on_process_selected_experiment_pushButton_clicked()
{
    QTreeWidgetItem *item = ui->neuDetectionResults_treeWidget->currentItem();
    if(item)
    {
        emit go_process_experiment(item->data(0, Qt::UserRole).toInt());
        this->reject();
    }
}

void slice_item_review::on_full_name_lineEdit_textChanged(const QString)
{
    ui->apply_changes_pushButton->setEnabled(true);
}

void slice_item_review::on_delete_selected_experiment_pushButton_clicked()
{
    QTreeWidgetItem *item = ui->neuDetectionResults_treeWidget->currentItem();
    if(!item)
    {
        QMessageBox::critical(this,"Empty selection", "Select item first", QMessageBox::Ok);
        return;
    }

    if(core->deleteDetectionResultsString(item->data(0, Qt::UserRole).toInt()))
    {
        QMessageBox::information(this,"Success", "experiment string deleted", QMessageBox::Ok);
        this->download_experiments_list();
        ui->process_selected_experiment_pushButton->setEnabled(false);
        ui->delete_selected_experiment_pushButton->setEnabled(false);
        ui->upload_selected_experiment_pushButton->setEnabled(false);
    }
    else
        QMessageBox::warning(this,"!", "any error", QMessageBox::Retry);
}

void slice_item_review::on_animals_siv_comboBox_currentIndexChanged(int)
{
    ui->apply_changes_pushButton->setEnabled(true);
}

void slice_item_review::on_segments_siv_comboBox_currentIndexChanged(int)
{
    ui->apply_changes_pushButton->setEnabled(true);
}

void slice_item_review::on_upload_selected_experiment_pushButton_clicked()
{
    QTreeWidgetItem *item = ui->neuDetectionResults_treeWidget->currentItem();
    if(!item)
    {
        QMessageBox::critical(this,"Empty selection", "Select item first", QMessageBox::Ok);
        return;
    }

    core->upload_experiment(item->data(0, Qt::UserRole).toInt(),
            ui->full_name_lineEdit->text());
}

void slice_item_review::on_deleteSlice_siv_pushButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete this item?", "Are you sure you want to delete this item?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        if(core->delete_pre_processing_string(pre_processing_id))
        {
            QMessageBox::information(this,"Success", "pre processing string deleted", QMessageBox::Ok);
            this->reject();
        }
        else
            QMessageBox::warning(this,"!", "any error", QMessageBox::Retry);
    }
}

void slice_item_review::on_animalType_siv_comboBox_currentTextChanged(const QString &arg1)
{
    ui->animals_siv_comboBox->clear();
    ui->segments_siv_comboBox->clear();

    ui->animals_siv_comboBox->addItems(core->getAnimalsList(arg1));
    ui->segments_siv_comboBox->addItems(core->getSegmentsList(arg1));
}

void slice_item_review::on_neuDetectionResults_treeWidget_itemPressed(QTreeWidgetItem *, int)
{
    ui->process_selected_experiment_pushButton->setEnabled(true);
    ui->delete_selected_experiment_pushButton->setEnabled(true);
    ui->upload_selected_experiment_pushButton->setEnabled(true);
}

void slice_item_review::on_markerType_siv_lineEdit_textChanged(const QString)
{
    ui->apply_changes_pushButton->setEnabled(true);
}
