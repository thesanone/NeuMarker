#include "algoritm_settings.h"
#include "ui_algoritm_settings.h"

//#include <QDebug>
//#include <QTime>
#include <QScrollBar>
#include <QMessageBox>

Algoritm_settings::Algoritm_settings(QWidget *parent, Core *c) :
    QMainWindow(parent),
    ui(new Ui::Algoritm_settings)
{
    ui->setupUi(this);
    ui->optional_settings_frame->hide();
    core = c;
    view1 = NULL;
    view2 = NULL;
    scene_for_view1 = NULL;
    scene_for_view2 = NULL;
    binImg = NULL;

    ui->erode_lineEdit->setValidator(new QIntValidator(0, 255, ui->erode_lineEdit));
    ui->dilate_lineEdit->setValidator(new QIntValidator(0, 255, ui->dilate_lineEdit));
    ui->binary_limit_1_lineEdit->setValidator(new QIntValidator(0, 65536, ui->binary_limit_1_lineEdit));
    ui->binary_limit_2_lineEdit->setValidator(new QIntValidator(0, 65536, ui->binary_limit_2_lineEdit));

    ui->binary_limit_1_lineEdit->setText("60");
    ui->binary_limit_2_lineEdit->setText("120");

    view1 = new Synchro_qsceneview(this);
    view2 = new Synchro_qsceneview(this);

    connect(view1, SIGNAL(synchro_wheel(int)), view2, SLOT(m_wheelEvent(int)));
    connect(view2, SIGNAL(synchro_wheel(int)), view1, SLOT(m_wheelEvent(int)));

    connect(view1->horizontalScrollBar(), SIGNAL(valueChanged(int)), view2->horizontalScrollBar(), SLOT(setValue(int)));
    connect(view2->horizontalScrollBar(), SIGNAL(valueChanged(int)), view1->horizontalScrollBar(), SLOT(setValue(int)));
    connect(view1->verticalScrollBar(), SIGNAL(valueChanged(int)), view2->verticalScrollBar(), SLOT(setValue(int)));
    connect(view2->verticalScrollBar(), SIGNAL(valueChanged(int)), view1->verticalScrollBar(), SLOT(setValue(int)));

    view1->setDragMode(QGraphicsView::ScrollHandDrag);
    view1->setDragMode(QGraphicsView::ScrollHandDrag);

    ui->for_graphycs_view_horizontalLayout->addWidget(view1);
    ui->for_graphycs_view_horizontalLayout->addWidget(view2);

    scene_for_view1 = new QGraphicsScene(this);
    view1->setScene(scene_for_view1);

    scene_for_view2 = new QGraphicsScene(this);
    view2->setScene(scene_for_view2);
}

Algoritm_settings::~Algoritm_settings()
{
    delete ui;
}

void Algoritm_settings::update_views()
{
    scene_for_view1->clear();
    scene_for_view2->clear();

    scene_for_view1->addPixmap(core->get_image_pixmap());
    binImg = new QGraphicsPixmapItem(core->get_binary_image_pixmap());
    scene_for_view2->addItem(binImg);

    ui->erode_lineEdit->clear();
    ui->dilate_lineEdit->clear();

    core->filter->setAllParametrs(ui->L_min_horizontalSlider->value(),
                                  ui->L_max_horizontalSlider->value(),
                                  ui->a_min_horizontalSlider->value(),
                                  ui->a_max_horizontalSlider->value(),
                                  ui->b_min_horizontalSlider->value(),
                                  ui->b_max_horizontalSlider->value());
    this->update_view2();

    EmptySliderMove em = core->filter->get_L_empty_move();
    ui->L_max_horizontalSlider->setMaximum(em.max);
    ui->L_max_horizontalSlider->setMinimum(em.min);
    em = core->filter->get_a_empty_move();
    ui->a_min_horizontalSlider->setMaximum(em.max);
    ui->a_min_horizontalSlider->setMinimum(em.min);

    QRectF rect(0,0, binImg->pixmap().width(), binImg->pixmap().height());
    //scene_for_view1->setSceneRect(rect);
    //scene_for_view2->setSceneRect(rect);
    view1->setSceneRect(rect);
    view2->setSceneRect(rect);
    view1->fitInView(view1->scene()->sceneRect(), Qt::KeepAspectRatio);
    view2->fitInView(view2->scene()->sceneRect(), Qt::KeepAspectRatio);
}

void Algoritm_settings::clearScene()
{
    if(scene_for_view1) scene_for_view1->clear();
    if(scene_for_view2) scene_for_view2->clear();
}

void Algoritm_settings::on_L_max_horizontalSlider_valueChanged(int value)
{
//    QTime t;
//    t.start();
    core->filter->set_L_max(value);
    ui->L_max_label->setText("L max " + QString::number(value));
    this->update_view2();
//    qDebug()<<t.elapsed();
}

void Algoritm_settings::on_a_min_horizontalSlider_valueChanged(int value)
{
//    QTime t;
//    t.start();
//    qDebug()<<t.elapsed();
    core->filter->set_a_min(value);
    ui->a_min_label->setText("a min " + QString::number(value));
    this->update_view2();
//    qDebug()<<t.elapsed();
}

void Algoritm_settings::on_L_min_horizontalSlider_valueChanged(int value)
{
    core->filter->set_L_min(value);
    ui->L_min_label->setText("L min " + QString::number(value));
    this->update_view2();
}

void Algoritm_settings::on_a_max_horizontalSlider_valueChanged(int value)
{
    core->filter->set_a_max(value);
    ui->a_max_label->setText("a max " + QString::number(value));
    this->update_view2();
}

void Algoritm_settings::on_b_min_horizontalSlider_valueChanged(int value)
{
    core->filter->set_b_min(value);
    ui->b_min_label->setText("b min " + QString::number(value));
    this->update_view2();
}

void Algoritm_settings::on_b_max_horizontalSlider_valueChanged(int value)
{
    core->filter->set_b_max(value);
    ui->b_max_label->setText("b max " + QString::number(value));
    this->update_view2();
}

void Algoritm_settings::on_submit_as_pushButton_clicked()
{
    if(ui->binary_limit_1_lineEdit->text().toInt()==60 && ui->binary_limit_2_lineEdit->text().toInt()==120)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "The binary limits are defined by default", "Are you sure you want to use default binary limits?", QMessageBox::Yes|QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel) return;
    }
    core->init_marker(ui->binary_limit_1_lineEdit->text().toInt(), ui->binary_limit_2_lineEdit->text().toInt());
    emit filter_configured();
    this->hide();
}

void Algoritm_settings::on_cansel_pushButton_clicked()
{
    this->close();
    emit as_canseled();
    clearScene();
}

void Algoritm_settings::on_options_pushButton_clicked()
{
    if(ui->optional_settings_frame->isVisible())
    {
        ui->optional_settings_frame->hide();
        ui->options_pushButton->setText("Show options Sliders");
    }
    else
    {
        ui->optional_settings_frame->show();
        ui->options_pushButton->setText("Hide options Sliders");
    }
}

void Algoritm_settings::update_view2()
{
    binImg->setPixmap(core->get_binary_image_pixmap());
}

void Algoritm_settings::on_erod_pushButton_clicked()
{
//    QTime t;
//    t.start();
    core->filter->erode(ui->erode_lineEdit->text().toInt());
    this->update_view2();
//    qDebug()<<t.elapsed();
}

void Algoritm_settings::on_dilate_pushButton_clicked()
{
//    QTime t;
//    t.start();
    core->filter->delite(ui->dilate_lineEdit->text().toInt());
    this->update_view2();
    //    qDebug()<<t.elapsed();
}

void Algoritm_settings::closeEvent(QCloseEvent *)
{
    emit as_canseled();
    clearScene();
}
