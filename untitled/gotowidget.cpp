#include "gotowidget.h"
#include "ui_gotowidget.h"

gotoWidget::gotoWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gotoWidget)
{
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);


    ui->setupUi(this);
}

gotoWidget::~gotoWidget()
{
    delete ui;
}

void gotoWidget::on_pushButton_clicked()
{
    this->hide();
    emit getInf(ui->spinBox->value(),ui->spinBox_2->value());
    //qDebug() << ui->spinBox->value() << " , " << ui->spinBox_2->value();
}

void gotoWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
}

