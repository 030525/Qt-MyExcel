#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    Qt::WindowFlags flag = Qt::Dialog;
    flag |= Qt::WindowCloseButtonHint;

    this->setWindowFlags(flag);

    ui->setupUi(this);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::closeEvent(QCloseEvent *event)
{
    this->setHidden(true);
    qDebug() << "hide find dialog";
}

void FindDialog::on_pushButton_clicked()
{
    if(ui->checkBox_3->isChecked())
    {
        emit startFromHead();
    }
    bool match_case = ui->checkBox->isChecked();
    QString text = ui->lineEdit->text();
    if(text != "")
    {
        if(ui->checkBox_2->isChecked())
        {
            emit find_next(text,match_case);
        }
        else
        {
            emit find_previous(text,match_case);
        }
    }
}

