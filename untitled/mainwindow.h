#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QIODevice>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>

#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QFont>


#include <QTimer>


#include "tableset.h"
#include "finddialog.h"
#include "gotowidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum
    {
        //用来确定文件格式
        FileNumber = 0x51789421,

        //显示最近文件的最大数量
        max_recentFile = 5
    };
public:
    QTimer *time = nullptr;
    //窗口
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void start_file();
    void start_connect();//初始槽函数连接

    void start_statueFont();
    void start_statue();

    void start_tableMenu();
    void start_widget();
    void Start();


private slots:
    void isChanged();
    void noChanged();

    void getClearSIGNAL(int row,int col);
    //选
    void getFindSIGNAL(int row,int col);//传入的是第i，从1开始

private slots://菜单快捷键

    //新
    void on_actionNew_triggered();

    //写
    void on_actionSave_triggered();//重写Save的点击
    void on_actionSave_as_triggered();

    //读
    void on_actionOpen_triggered();

    //找
    void on_actionFind_triggered();

    void on_actionGoto_triggered();

private://文件操作

    //写
    bool write_file(QString  file_name);//负责落实写入
    bool SaveAs();//保存新文件
    bool Save();

    bool saveToOther();//其他操作前，是否save


    //读
    bool read_file(QString file_name);
    bool load_file();

private slots://edit菜单   
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();

    void on_actionRow_triggered();
    void on_actionCol_triggered();
    void on_actionAll_triggered();

    void on_actionSort_triggered(bool key);

private:
    void Cut(void (MainWindow::*dosome)(QTableWidgetItem*));
    void Paste();
    void deleteItem(QTableWidgetItem*item);

private://按键
    void keyPressEvent(QKeyEvent *event);


private://对话框
    FindDialog *f = nullptr;
    TableSet *ts = nullptr;
    gotoWidget *g = nullptr;

    void getNewDialog();//模态
    void getFindDialog();
    void getGotoWidget();


signals://右侧框和table
    void change_Font(const QFont & f);
private slots:
    //表格改变

    void on_spinBox_valueChanged(int arg1);

    void on_fontComboBox_currentFontChanged(const QFont &f);

    //B
    void on_pushButton_toggled(bool checked);
    //I
    void on_pushButton_2_toggled(bool checked);
    //U
    void on_pushButton_3_toggled(bool checked);

    //font框改变
    void on_tableWidget_itemSelectionChanged();
    void get_changeFont(const QFont & f);




private:
    //ui设置
    Ui::MainWindow *ui;


private:

    QString filename;//当前文件名
    void set_filename(const QString & s);
private:
    //表右键菜单
    QMenu *tableMenu = nullptr;
    void set_tableMenu();
    void add_tableMenu();


    //表操作

    bool exist_table();
    void get_matrixSize(int &row,int &col);//获取表大小

    void Table(int startRow,int startCol,void(MainWindow::*dosome)(int i,int j));
    void set_table(int row,int col);//设置表大小

    void recycle_ItemMemory(int big_row,int big_col,int row,int col);//检测矩阵row，col以外的item内存并回收

    //找
    bool can_match(QString text,QString compare,bool match_case);
    bool find_backforward(QString text,bool match_case);
    bool find_forward(QString text,bool match_case);






    //单元格
    QTableWidgetItem* oneItemSelected();
    void clearItem(int k,int m);

    bool get_nextItem(int &row,int &col);
    bool get_previousItem(int & row,int &col);


    void get_currenItem(int &row,int &col);

    void set_currentItem(int row,int col);
    void set_Item(int row,int col,const QString & s,const QFont &f,const QColor &textcolor,const QColor &backgourndcolor);//设置单元格内容


    //其他
    void create_directory();//设置文件目录
    void set_statusBar(const QString & s = "Welcome to MyExcel !");//设置状态栏

};

#endif // MAINWINDOW_H
