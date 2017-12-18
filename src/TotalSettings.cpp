#include "TotalSettings.h"
#include "ui_TotalSettings.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QListWidgetItem>

TotalSettings::TotalSettings(QWidget *parent, Core *c) :
    QDialog(parent),
    ui(new Ui::TotalSettings)
{
    core = c;
    ui->setupUi(this);

    connect(ui->comboBox_animalSpecies, SIGNAL(currentIndexChanged(int)), this, SLOT(animalTypeSelectionChanged()));
    connect(ui->pushButton_addAnimalSpecies, SIGNAL(pressed()), this, SLOT(addAnimalType()));
    connect(ui->pushButton_deleteAnimalSpecies, SIGNAL(pressed()), this, SLOT(deleteAnimalType()));
    connect(ui->pushButton_addAnimal, SIGNAL(pressed()), this, SLOT(addAnimal()));
    connect(ui->pushButton_deleteAnimal, SIGNAL(pressed()), this, SLOT(deleteAnimal()));
    connect(ui->pushButton_addSegment, SIGNAL(pressed()), this, SLOT(addSegment()));
    connect(ui->pushButton_deleteSegment, SIGNAL(pressed()), this, SLOT(deleteSegment()));

    ui->comboBox_animalSpecies->addItems(core->getAnimalsTypesList());
    ui->comboBox_animalSpecies->setCurrentIndex(0);
}

TotalSettings::~TotalSettings()
{
    delete ui;
}

void TotalSettings::animalTypeSelectionChanged()
{
    QString animalType = ui->comboBox_animalSpecies->currentText();
    ui->listWidget_animalsID->clear();
    ui->listWidget_segments->clear();

    if(animalType.isEmpty()) return;

    ui->listWidget_animalsID->addItems(core->getAnimalsList(animalType));
    ui->listWidget_segments->addItems(core->getSegmentsList(animalType));

    QListWidgetItem *item = ui->listWidget_animalsID->item(0);
    if (item) ui->listWidget_animalsID->setCurrentItem(item);

    item = ui->listWidget_segments->item(0);
    if (item) ui->listWidget_segments->setCurrentItem(item);
}

void TotalSettings::updateAnimalTypes(QString set_name)
{
    ui->comboBox_animalSpecies->clear();
    ui->comboBox_animalSpecies->addItems(core->getAnimalsTypesList());
    ui->comboBox_animalSpecies->setCurrentText(set_name);
}

void TotalSettings::addAnimalType()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Animal species addition"),
                                         tr("Enter new species name: "), QLineEdit::Normal,
                                         "", &ok);
    if (!ok || text.isEmpty()) return;

    int res = core->addNewAnimalType(text);
    switch (res)
    {
    case -1:
        QMessageBox::critical(this,"Error!", "SQLite addition error :( \nCheck your write privelege 'tsaat'", QMessageBox::Retry);
        break;
    case 0:
        QMessageBox::warning(this,"error", "Animal specie '"+text+"' alredy exist", QMessageBox::Ok);
        break;
    case 1:
        //QMessageBox::information(this,"Succes", "Animal specie '"+text+"' added!", QMessageBox::Ok);
        updateAnimalTypes(text);
    }
}

void TotalSettings::deleteAnimalType()
{
    QString animalType = ui->comboBox_animalSpecies->currentText();

    if(animalType.isEmpty())
    {
        QMessageBox::warning(this,"Select animal species","Can't delte empty string", QMessageBox::Ok);
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete species", "Are you sure you want to delete '"+animalType+"'?", QMessageBox::Yes|QMessageBox::Cancel);
    if (reply != QMessageBox::Yes) return;

    int res = core->deleteAnimalType(animalType);
    switch (res)
    {
    case -1:
        QMessageBox::critical(this,"Error!", "SQLite delete error :( \n 'tsdat'", QMessageBox::Retry);
        break;
    case 0:
        QMessageBox::warning(this,"error", "Animal specie '"+animalType+"' alredy exist", QMessageBox::Ok);
        break;
    case 1:
        //QMessageBox::information(this,"Succes", "Animal specie '"+animalType+"' deleted!", QMessageBox::Ok);
        updateAnimalTypes();
    }
}

void TotalSettings::addAnimal()
{
    QString animalType = ui->comboBox_animalSpecies->currentText();

    if(animalType.isEmpty())
    {
        QMessageBox::warning(this,"Select animal species","Can't create animal for empty animal specie", QMessageBox::Ok);
        return;
    }

    bool ok;
    QString text = QInputDialog::getText(this, tr("Animal addition"),
                                         tr("Enter new animal id: "), QLineEdit::Normal,
                                         "", &ok);
    if (!ok || text.isEmpty()) return;

    int id = text.toInt();

    int res = core->addNewAnimal(animalType, id);
    switch (res)
    {
    case -1:
        QMessageBox::critical(this,"Error!", "SQLite addition error :( \nCheck your write priveleges 'tsaa'", QMessageBox::Retry);
        break;
    case 0:
        QMessageBox::warning(this,"error", "Animal #"+QString::number(id)+" alredy exist", QMessageBox::Ok);
        break;
    case 1:
        //QMessageBox::information(this,"Succes", "Animal #"+QString::number(id)+" added!", QMessageBox::Ok);
        animalTypeSelectionChanged();
    }
}

void TotalSettings::deleteAnimal()
{
    QString animalType = ui->comboBox_animalSpecies->currentText();

    if(animalType.isEmpty())
    {
        QMessageBox::warning(this,"Select animal species","Can't delete animal for empty animal specie", QMessageBox::Ok);
        return;
    }

    QListWidgetItem *item = ui->listWidget_animalsID->currentItem();
    if(!item)
    {
        QMessageBox::warning(this,"Select animal id","Can't delete empty animal", QMessageBox::Ok);
        return;
    }
    QString text = ui->listWidget_animalsID->currentItem()->text();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete animal",
                "Are you sure you want to delete '" + animalType + " #" + text + "'?", QMessageBox::Yes|QMessageBox::Cancel);
    if (reply != QMessageBox::Yes) return;

    int id = text.toInt();

    int res = core->deleteAnimal(animalType, id);
    switch (res)
    {
    case -1:
        QMessageBox::critical(this,"Error!", "SQLite delete error :( \n 'tsda'", QMessageBox::Retry);
        break;
    case 0:
        QMessageBox::warning(this,"error", "Can't delete animal #"+QString::number(id)+" :(\nCheck your write priveleges 'tsda'", QMessageBox::Ok);
        break;
    case 1:
        QMessageBox::information(this,"Succes", "Animal #"+QString::number(id)+" deleted!", QMessageBox::Ok);
        animalTypeSelectionChanged();
    }
}

void TotalSettings::addSegment()
{
    QString animalType = ui->comboBox_animalSpecies->currentText();

    if(animalType.isEmpty())
    {
        QMessageBox::warning(this,"Select animal species","Can't create animal for empty animal specie", QMessageBox::Ok);
        return;
    }

    bool ok;
    QString text = QInputDialog::getText(this, tr("Animal species addition"),
                                         tr("Enter new species name: "), QLineEdit::Normal,
                                         "", &ok);
    if (!ok || text.isEmpty()) return;

    int res = core->addNewSegment(animalType, text);
    switch (res)
    {
    case -1:
        QMessageBox::critical(this,"Error!", "SQLite addition error :( \nCheck your write priveleges 'tsas'", QMessageBox::Retry);
        break;
    case 0:
        QMessageBox::warning(this,"error", "Segment '"+text+"' alredy exist", QMessageBox::Ok);
        break;
    case 1:
        //QMessageBox::information(this,"Succes", "Segment '"+text+"' added!", QMessageBox::Ok);
        animalTypeSelectionChanged();
    }
}

void TotalSettings::deleteSegment()
{
    QString animalType = ui->comboBox_animalSpecies->currentText();

    if(animalType.isEmpty())
    {
        QMessageBox::warning(this,"Select animal species","Can't delete segment for empty animal specie", QMessageBox::Ok);
        return;
    }

    QListWidgetItem *item = ui->listWidget_segments->currentItem();
    if(!item)
    {
        QMessageBox::warning(this,"Select segment","Can't delete empty segment", QMessageBox::Ok);
        return;
    }
    QString text = ui->listWidget_segments->currentItem()->text();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete segement",
                "Are you sure you want to delete '" + animalType + " segment " + text + "'?", QMessageBox::Yes|QMessageBox::Cancel);
    if (reply != QMessageBox::Yes) return;

    int res = core->deleteSegment(animalType, text);
    switch (res)
    {
    case -1:
        QMessageBox::critical(this,"Error!", "SQLite delete error :( \n 'tsds'", QMessageBox::Retry);
        break;
    case 0:
        QMessageBox::warning(this,"error", "Can't delete segment '"+text+"' :(\nCheck your write priveleges 'tsds'", QMessageBox::Ok);
        break;
    case 1:
        QMessageBox::information(this,"Succes", "Segment '"+text+"' deleted!", QMessageBox::Ok);
        animalTypeSelectionChanged();
    }
}

