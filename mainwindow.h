#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core.h"
#include "slice_item_review.h"
#include "add_pre_processing_pict.h"
#include "add_new_cat.h"
#include "wheelevent_forqsceneview.h"
#include "algoritm_settings.h"
#include "TotalSettings.h"
#include <QMainWindow>
#include <QListWidgetItem>
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendImagesPath(QString way, QStringList files_in_folder);
    void throw_sliceID(int sliceID);

private slots:
    // actions
    void on_action_Add_new_cat_triggered();
    void on_actionAdd_pre_processing_pictures_triggered();
    void on_actionNeuMarker_help_triggered();
    void actionAnimalsSettings();

    /// Выделение среза из списков
    void on_slice_listWidget_itemClicked(QListWidgetItem *item);

    void update_slice_listWidget();
    void update_animalsID_listWidget(QString cat);

    /// Загружает в ядро картинку по sliceID && Вызывает окно настройки алгоритма
    void draw_image_neurons(int scileID);
    /// Рисует на сцене размеченные прямоугольники по id из NeuronsDetectionResults
    void draw_experiment(int id_experiment);
    /// Скрывает неактивные компоненты и устанавливает сцену из ядра
    void set_scene();

    // push Buttons
    void on_back_to_list_pushButton_clicked();
    void on_pushButton_backToImageProcessing_clicked();
    void on_submit_pushButton_clicked();
    void on_reset_pushButton_clicked();
    void on_reverse_pushButton_clicked();
    void on_save_experiment_pushButton_clicked();
    void on_upload_data_pushButton_clicked();

    // item selection changed
    void on_animalsTypes_listWidget_itemSelectionChanged();
    void on_segments_listWidget_itemSelectionChanged();
    void on_animals_listWidget_itemSelectionChanged();





private:
    QGraphicsView *view;
    Ui::MainWindow *ui;
    Core *core;
    add_new_cat *anc;
    slice_item_review *siv;
    add_pre_processing_pict *appp;
    Algoritm_settings *as;
    TotalSettings *ts;

    QString selectedAnimalType;
    QString selectedAnimal;
    QString selectedSegment;

    void showImageProcessingFrame();
    void showSliceSelectionFrame();
};

#endif // MAINWINDOW_H
