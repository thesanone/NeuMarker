#ifndef SLICE_ITEM_REVIEW_H
#define SLICE_ITEM_REVIEW_H

#include <core.h>
#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class slice_item_review;
}

class slice_item_review : public QDialog
{
    Q_OBJECT

public:
    explicit slice_item_review(QWidget *parent = 0, Core* c = 0);
    ~slice_item_review();

public slots:
    //void update_cat_list(QString cat);
    void download_experiments_list();

signals:
    void go_process_pre_processing_by_id(int id);
    void go_process_experiment(int id_experiment);

private slots:

    void set_pre_processing_id(int id);

    void on_apply_changes_pushButton_clicked();

    void on_full_name_lineEdit_textChanged(const QString);
    void on_slizeNumber_lineEdit_textChanged(const QString);
    void on_position_lineEdit_textChanged(const QString);
    void on_isInvert_checkBox_clicked();
    void on_comment_plainTextEdit_textChanged();

    //void on_experiments_listWidget_itemPressed(QListWidgetItem *);
    void on_new_experiment_pushButton_clicked();
    void on_process_selected_experiment_pushButton_clicked();
    void on_delete_selected_experiment_pushButton_clicked();

    void on_animals_siv_comboBox_currentIndexChanged(int);
    void on_segments_siv_comboBox_currentIndexChanged(int);

    void on_upload_selected_experiment_pushButton_clicked();

    void on_deleteSlice_siv_pushButton_clicked();

    void on_animalType_siv_comboBox_currentTextChanged(const QString &arg1);

    void on_neuDetectionResults_treeWidget_itemPressed(QTreeWidgetItem *, int);

    void on_markerType_siv_lineEdit_textChanged(const QString);

private:
    int pre_processing_id;
    Ui::slice_item_review *ui;
    Core* core;
    //QStringList segmentsVector;
    //QStringList catIdVector;
};

#endif // SLICE_ITEM_REVIEW_H
