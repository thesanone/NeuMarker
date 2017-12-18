#ifndef ALGORITM_SETTINGS_H
#define ALGORITM_SETTINGS_H

#include <QMainWindow>
#include <QCloseEvent>
#include "core.h"
#include "synchro_qsceneview.h"


namespace Ui {
class Algoritm_settings;
}

class Algoritm_settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit Algoritm_settings(QWidget *parent = 0, Core *c = 0);
    ~Algoritm_settings();

signals:
    void filter_configured();
    void as_canseled();

public slots:
    void update_views();
    void clearScene();

private slots:
    void on_L_max_horizontalSlider_valueChanged(int value);

    void on_a_min_horizontalSlider_valueChanged(int value);

    void on_L_min_horizontalSlider_valueChanged(int value);

    void on_a_max_horizontalSlider_valueChanged(int value);

    void on_b_min_horizontalSlider_valueChanged(int value);

    void on_b_max_horizontalSlider_valueChanged(int value);

    void on_submit_as_pushButton_clicked();

    void on_cansel_pushButton_clicked();

    void on_options_pushButton_clicked();

    void update_view2();

    void on_erod_pushButton_clicked();

    void on_dilate_pushButton_clicked();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::Algoritm_settings *ui;
    Core *core;
    Synchro_qsceneview* view1;
    Synchro_qsceneview* view2;
    QGraphicsScene* scene_for_view1;
    QGraphicsScene* scene_for_view2;
    QGraphicsPixmapItem* binImg;
};

#endif // ALGORITM_SETTINGS_H
