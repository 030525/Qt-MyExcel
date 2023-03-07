#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

    void closeEvent(QCloseEvent *event);
signals:
    void find_next(QString  text,bool match_case);
    void find_previous(QString  text,bool match_case);
    void startFromHead(int row = 0,int col = 0);

private slots:
    void on_pushButton_clicked();

private:
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
