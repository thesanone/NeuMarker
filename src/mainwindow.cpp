#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QSize>
#include <QtSql>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QInputDialog>


//#include "wheelevent_forqsceneview.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    core = new Core(this);
    siv = new slice_item_review(this, core);
    appp = new add_pre_processing_pict(this, core);
    anc = new add_new_cat(this, core);
    as = new Algoritm_settings(this, core);
    ts = new TotalSettings(this, core);
    view = new WheelEvent_forQSceneView(this);
    view->setCursor(Qt::ArrowCursor);

    connect(this, SIGNAL(throw_sliceID(int)), siv, SLOT(set_pre_processing_id(int)));
    connect(siv, SIGNAL(go_process_pre_processing_by_id(int)), this, SLOT(draw_image_neurons(int)));
    connect(siv, SIGNAL(go_process_experiment(int)), this, SLOT(draw_experiment(int)));
    connect(as, SIGNAL(filter_configured()), this, SLOT(set_scene()));
    connect(as, SIGNAL(as_canseled()), core, SLOT(clearLABdetector()));
    connect(as, SIGNAL(as_canseled()), core, SLOT(clearScene()));

    ui->setupUi(this);
    ui->neurons_tab_layout->addWidget(view);
    ui->pict_processing_frame->hide();

    QStringList animalsTypes = core->getAnimalsTypesList();
    for(int i = 0; i < animalsTypes.size(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(animalsTypes.at(i));
        ui->animalsTypes_listWidget->insertItem(0, item);
    }

    //GUI actions
    connect(ui->actionAnimalsSettings, SIGNAL(triggered(bool)), this, SLOT(actionAnimalsSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete core;
}

void MainWindow::on_action_Add_new_cat_triggered()
{
    anc->exec();
}

void MainWindow::on_actionAdd_pre_processing_pictures_triggered()
{
    QString file_name = QFileDialog::getExistingDirectory(this, "C:/", tr("Way to cat images"));

    if(!file_name.isEmpty())
    {
        QDir dir(file_name);
        QStringList stringList = dir.entryList(QStringList() << "*.jpg", QDir::Files);

        appp->reset_form(selectedAnimalType, selectedAnimal, selectedSegment);
        appp->loadImages(file_name, stringList);

        appp->exec();
        this->update_slice_listWidget();
    }
}

void MainWindow::on_actionNeuMarker_help_triggered()
{
    if (!QDesktopServices::openUrl(QUrl("help.pdf")))
        QMessageBox::warning(this,"File not found","File 'help.pdf' not found", QMessageBox::Ok);
}

void MainWindow::actionAnimalsSettings()
{
    ts->exec();
}

void MainWindow::on_back_to_list_pushButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Close correction window?", "Are you sure you want to close correction window?", QMessageBox::Yes|QMessageBox::Cancel);
    if (reply == QMessageBox::Cancel) return;
    showSliceSelectionFrame();
    core->clearLABdetector();
    core->clearScene();
    as->clearScene();
    view->update();
}

void MainWindow::on_slice_listWidget_itemClicked(QListWidgetItem *item)
{
    emit throw_sliceID(item->data(Qt::UserRole).toInt());
    siv->exec();
    this->update_slice_listWidget();
}

void MainWindow::update_slice_listWidget()
{
    ui->slice_listWidget->clear();

    std::vector<QListWidgetItem *> items = core->getSliceWidgetItems(
                selectedAnimalType, selectedAnimal, selectedSegment);

    for(std::vector<QListWidgetItem*>::iterator i = items.begin(); i != items.end(); i++)
        ui->slice_listWidget->insertItem(0, *i);
}

void MainWindow::update_animalsID_listWidget(QString cat)
{
    QListWidgetItem* new_item = new QListWidgetItem(cat);
//    ui->animalsID_listWidget->clear();
//    std::vector<QListWidgetItem *> items = core->getCatsQListWidgetItems();
//    for(std::vector<QListWidgetItem*>::iterator i = items.begin(); i != items.end(); i++)
    ui->animals_listWidget->insertItem(0, new_item);
}

void MainWindow::draw_image_neurons(int scileID)
{
    // создает фильтр подгружая картинку
    core->start_process_with_ppi(scileID);
    as->update_views();
    as->show();
}

void MainWindow::draw_experiment(int id_experiment)
{
    view->setScene(core->create_experiment_scene(id_experiment));

    ui->reset_pushButton->setEnabled(true);
    ui->reverse_pushButton->setEnabled(true);
    ui->save_experiment_pushButton->setEnabled(true);
    ui->upload_data_pushButton->setEnabled(true);
    ui->pushButton_backToImageProcessing->setEnabled(false);

    showImageProcessingFrame();
}

void MainWindow::on_submit_pushButton_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Cells group name"),
                                         tr("Enter cells group name: "), QLineEdit::Normal,
                                         "name", &ok);
    if (!ok || text.isEmpty()) return;

    int res = core->submit_scene("lab filter", text, "no");
    if (res)
    {
        QMessageBox::information(this,"Success", "Experiment added, "+QString::number(res)+" neurons foud", QMessageBox::Ok);
        //ui->submit_pushButton->setEnabled(false);
        ui->save_experiment_pushButton->setEnabled(true);
        ui->upload_data_pushButton->setEnabled(true);
    }
    else QMessageBox::warning(this, "Something wrong", "Submit error, or 0 neurons found", QMessageBox::Ok);
}

void MainWindow::on_reset_pushButton_clicked()
{
    core->sceneSetAllRectRed();
}

void MainWindow::on_reverse_pushButton_clicked()
{
    core->sceneSetAllRectGreen();
}

void MainWindow::set_scene()
{
    showImageProcessingFrame();

    ui->save_experiment_pushButton->setEnabled(false);
    ui->upload_data_pushButton->setEnabled(false);
    ui->submit_pushButton->setEnabled(true);
    ui->reset_pushButton->setEnabled(true);
    ui->reverse_pushButton->setEnabled(true);
    ui->pushButton_backToImageProcessing->setEnabled(true);

    view->setScene(core->create_graphics_scene());
}

void MainWindow::on_save_experiment_pushButton_clicked()
{
    int res = core->save_experiment_changes();
    if (res) QMessageBox::information(this,"Success", "Experiment changes saved, "+QString::number(res)+" neurons foud", QMessageBox::Ok);
    else QMessageBox::warning(this, "Something wrong", "Save error, or 0 neurons found", QMessageBox::Ok);
}

void MainWindow::on_animalsTypes_listWidget_itemSelectionChanged()
{
    QListWidgetItem *item = ui->animalsTypes_listWidget->currentItem();
    if(!item || item->text() == selectedAnimalType) return;

    selectedAnimalType = item->text();
    ui->segments_listWidget->clear();
    ui->animals_listWidget->clear();

    bool selectFirstAnimal = true;
    bool selectFirstSlice = true;

    QStringList items = core->getAnimalsList(selectedAnimalType);
    for(int i = 0; i < items.size(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(items.at(i));
        ui->animals_listWidget->insertItem(0, item);
        if(items.at(i) == selectedAnimal)
        {
            ui->animals_listWidget->setCurrentItem(item);
            selectFirstAnimal = false;
        }
    }

    if(items.size() > 0 && selectFirstAnimal)
        ui->animals_listWidget->setCurrentRow(0);

    items = core->getSegmentsList(selectedAnimalType);
    for(int i = 0; i < items.size(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(items.at(i));
        ui->segments_listWidget->insertItem(0, item);
        if(items.at(i) == selectedSegment)
        {
            ui->segments_listWidget->setCurrentItem(item);
            selectFirstSlice = false;
        }
    }

    if(items.size() > 0 && selectFirstSlice)
        ui->segments_listWidget->setCurrentRow(0);

    this->update_slice_listWidget();
}

void MainWindow::on_segments_listWidget_itemSelectionChanged()
{
    QListWidgetItem *item = ui->segments_listWidget->currentItem();
    if(!item || item->text() == selectedSegment) return;

    selectedSegment = item->text();
    this->update_slice_listWidget();
}

void MainWindow::on_animals_listWidget_itemSelectionChanged()
{
    QListWidgetItem *item = ui->animals_listWidget->currentItem();
    if(!item || item->text() == selectedAnimal) return;

    selectedAnimal = item->text();
    this->update_slice_listWidget();
}

void MainWindow::on_upload_data_pushButton_clicked()
{
    core->upload_experiment_from_scene();
}

void MainWindow::showImageProcessingFrame()
{
    ui->pict_processing_frame->show();
    ui->slice_select_frame->hide();
    ui->animals_segments_select_frame->hide();
}

void MainWindow::showSliceSelectionFrame()
{
    ui->pict_processing_frame->hide();
    ui->slice_select_frame->show();
    ui->animals_segments_select_frame->show();
}

void MainWindow::on_pushButton_backToImageProcessing_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Close correction window?", "Are you sure you want to close correction window?\n    Unsaved changes will be lost.", QMessageBox::Yes|QMessageBox::Cancel);
    if (reply == QMessageBox::Cancel) return;
    showSliceSelectionFrame();
    as->show();
    core->clearScene();
}
