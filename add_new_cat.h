#ifndef ADD_NEW_CAT_H
#define ADD_NEW_CAT_H

#include <QDialog>
#include "core.h"

namespace Ui {
class add_new_cat;
}

class add_new_cat : public QDialog
{
    Q_OBJECT

public:
    explicit add_new_cat(QWidget *parent = 0, Core* c = 0);
    ~add_new_cat();

signals:
    void update_cat_list(QString cat);

private slots:
    void on_buttonBox_anc_accepted();

private:
    Ui::add_new_cat *ui;
    Core* core;
};

#endif // ADD_NEW_CAT_H
