#ifndef GOTOWIDGET_H
#define GOTOWIDGET_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class gotoWidget;
}

class gotoWidget : public QDialog
{
    Q_OBJECT

public:
    explicit gotoWidget(QWidget *parent = nullptr);
    ~gotoWidget();
public slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);
signals:
    void getInf(int row,int col);
private:
    Ui::gotoWidget *ui;
};

#endif // GOTOWIDGET_H
