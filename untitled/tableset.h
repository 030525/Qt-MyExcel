#ifndef TABLESET_H
#define TABLESET_H

#include <QDebug>
#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class TableSet;
}

class TableSet : public QDialog
{
    Q_OBJECT

public:
    explicit TableSet(QWidget *parent = nullptr);
    ~TableSet();

    void closeEvent(QCloseEvent * event);

signals:
    void getInf(int row,int col);
    void clear(int row,int col);
private slots:

    void on_cancel_clicked();

    void on_ok_clicked();

private:
    Ui::TableSet *ui;
};

#endif // TABLESET_H
