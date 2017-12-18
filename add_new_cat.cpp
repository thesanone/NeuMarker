#include "add_new_cat.h"
#include "ui_add_new_cat.h"

#include <QMessageBox>

add_new_cat::add_new_cat(QWidget *parent, Core *c) :
    QDialog(parent),
    ui(new Ui::add_new_cat)
{
    core = c;
    ui->setupUi(this);

    ui->catID_LineEdit->setValidator(new QIntValidator(0, 65536, ui->catID_LineEdit));
}

add_new_cat::~add_new_cat()
{
    delete ui;
}

void add_new_cat::on_buttonBox_anc_accepted()
{
    if(ui->catID_LineEdit->text() != "")
    {
        int res = core->add_new_cat(ui->catID_LineEdit->text().toInt(),
                                     ui->comment_textEdit->toPlainText(),
                                     ui->impacts_segments_LineEdit->text(),
                                     ui->motion_direct_LineEdit->text());
        switch (res) {
        case -1:
            QMessageBox::critical(this,"Error!", "SQLite addition error :( \n Check your write privelege", QMessageBox::Retry);
            break;
        case 0:
            QMessageBox::warning(this,"error", "Cat #"+ui->catID_LineEdit->text()+" alredy exist", QMessageBox::Retry);
            break;
        case 1:
        {
            QMessageBox::information(this,"Succes", "Cat #"+ui->catID_LineEdit->text()+" added", QMessageBox::Ok);
            emit update_cat_list(ui->catID_LineEdit->text());
            this->reject();
        }
            break;
        default:
            break;
        }
        /*if(res)
        {
            QMessageBox::information(this,"Succes", "Cat #"+ui->catID_LineEdit->text()+" added", QMessageBox::Ok);
            emit update_cat_list(ui->catID_LineEdit->text());
            this->reject();
        }
        else
            QMessageBox::warning(this,"error", "Cat #"+ui->catID_LineEdit->text()+" alredy exist", QMessageBox::Retry);*/
    }
    else QMessageBox::warning(this,"error", "Please enter cat ID", QMessageBox::Retry);
}
