#ifndef TOTALSETTINGS_H
#define TOTALSETTINGS_H

#include <QDialog>

#include "core.h"

namespace Ui {
class TotalSettings;
}

class TotalSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TotalSettings(QWidget *parent = 0, Core *c = 0);
    ~TotalSettings();

protected slots:
    void animalTypeSelectionChanged();
    void updateAnimalTypes(QString set_name = "");

    // buttons:
    void addAnimalType();
    void deleteAnimalType();
    void addAnimal();
    void deleteAnimal();
    void addSegment();
    void deleteSegment();

private:
    Ui::TotalSettings *ui;
    Core *core;

    //void unswerSwitcher(int value, QString errorTitle, QString errorText);
};

#endif // TOTALSETTINGS_H
