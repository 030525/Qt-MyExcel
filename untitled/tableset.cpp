#include "tableset.h"
#include "ui_tableset.h"

TableSet::TableSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableSet)
{

    Qt::WindowFlags flag = Qt::Dialog;
    flag |= Qt::WindowCloseButtonHint;

    this->setWindowFlags(flag);

    ui->setupUi(this);

}

TableSet::~TableSet()
{
    delete ui;
}

void TableSet::closeEvent(QCloseEvent *event)
{
    //一样的
    //this->setHidden(true);
    this->hide();
    qDebug() << "hide tableset dialog";
}

void TableSet::on_ok_clicked()
{
    int row = ui->spinBox->value();
    int col = ui->spinBox_2->value();
    emit getInf(row,col);
    if(ui->createNew->isChecked())
    {
        emit clear(row,col);
    }
    this->hide();
}


void TableSet::on_cancel_clicked()
{
    this->hide();
}




