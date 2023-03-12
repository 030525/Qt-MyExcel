#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
     ui->setupUi(this);

     this->move(400,100);
     Start();

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ColorDebug()
{
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        for(int j = 0;j < ui->tableWidget->columnCount();j++)
        {
            QTableWidgetItem * item = ui->tableWidget->item(i,j);
            if(item != nullptr)
            {
                qDebug() << "( " << i << " , " << j <<  " )"
                         << item->foreground().color().name()
                         << item->background().color().name();
            }
        }
    }

}



void MainWindow::isChanged()
{
    this->setWindowTitle(tr("文件未更新[*]"));
    this->setWindowModified(true);
}

void MainWindow::noChanged()
{
    this->setWindowModified(false);
    this->setWindowTitle("MyExcel");
}

void MainWindow::getClearSIGNAL(int row, int col)
{
    Table(row,col,&MainWindow::clearItem);
}

void MainWindow::start_file()
{
    create_directory();
}



void MainWindow::on_actionNew_triggered()
{
    if(saveToOther())
    {
        ts->setModal(true);
        ts->show();
    }
}

void MainWindow::on_actionSave_triggered()
{
    Save();
}

void MainWindow::on_actionSave_as_triggered()
{
    SaveAs();
}

bool MainWindow::write_file(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(0,"错误",QString("%1文件未打开:%2")
                              .arg(file.fileName())
                              .arg(file.errorString()));
        return false;
    }

    /*
    quint16 row,col;
    getSize(row,col);
    */
    int row,col;
    get_matrixSize(row,col);

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_12);

    //输入特定数字，确保文件格式可用
    out << quint32(FileNumber) << row << col ;

    //quint i,j;
    int itemNum = 0;

    for(int i = 0;i < row;i++)
    {
        for(int j = 0;j < col;j++)
        {
            //out << i << j << get_ItemString(i,j);
            auto item = ui->tableWidget->item(i,j);

            if(item != nullptr)
            {

                out << i << j << item->text() << item->font();
                itemNum++;
            }
        }
    }
    file.close();
    qDebug() << "write in " << itemNum << "  items";

    return true;
}

bool MainWindow::SaveAs()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("保存文件为\n"),"/File/",tr("保存类型为(*.xwj)\n"));

    if(filename.isEmpty()) return false;

    //文件保存成功,设置状态栏
    if(this->write_file(filename))
    {
        set_filename(filename);

        QFileInfo fileinf(filename);
        /*
        QString s = "\n已保存文件 : " + fileinf.fileName();
        s += " \n于路径 ：";
        s += fileinf.filePath();
        set_statusBar(s);
        */

        noChanged();
        return true;
    }

    return false;
}

bool MainWindow::Save()
{
    //无则存，有则该
    if(filename == "")
    {
        return SaveAs();
    }
    else
    {
        if(write_file(filename))
        {
            noChanged();
            QFileInfo fileinf(filename);
            set_statusBar(" 已更新文件 : " + fileinf.fileName());
            return true;
        }
        return false;
    }
}

bool MainWindow::saveToOther()
{
    //是否有没保存的更改
    if(this->isWindowModified())
    {
        int returnNum = QMessageBox::warning(this,"保存更改","是否保存更改",
                                             QMessageBox::Yes |
                                             QMessageBox::No |
                                             QMessageBox::Cancel);

        //创建后
        if(returnNum == QMessageBox::Yes)
        {
            return Save();
        }

        //不跳转
        if(returnNum == QMessageBox::Cancel)
        {
            return false;
        }
    }
    //跳转
    return true;
}

void MainWindow::on_actionOpen_triggered()
{
    if(saveToOther())
    {
        load_file();
    }

}

void MainWindow::on_actionFind_triggered()
{
    f->show();
}

void MainWindow::on_actionGoto_triggered()
{
    g->show();
}

void MainWindow::Cut(void (MainWindow::*dosome)(QTableWidgetItem *))
{

    QString str = "";

    QList<QTableWidgetSelectionRange> list = ui->tableWidget->selectedRanges();

    if(list.size() == 0) return;

    for(int i = list[0].topRow();i <= list[0].bottomRow();i++)
    {
        for(int j = list[0].leftColumn();j <= list[0].rightColumn();j++)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i,j);
            if(item)
            {
                str += item->text();

                //传入函数指针不为空，如果item不为空，删除item
                if(dosome != nullptr)
                {
                    (this->*dosome)(item);
                }
            }
            str += "\t";
        }
        str.chop(1);
        str += "\n";
    }

    str.chop(1);

    QApplication::clipboard()->setText(str);
    ui->tableWidget->clearSelection();

    if(dosome == nullptr) qDebug() << "copy text : " << str;
    else qDebug() << "cut text : " << str;
}

void MainWindow::Paste()
{
    QString str = QApplication::clipboard()->text();

    qDebug() << "paste text :" << str;

    int Row = ui->tableWidget->rowCount();
    int Col = ui->tableWidget->columnCount();

    QStringList rowlist = str.split('\n');

    int cRow = ui->tableWidget->currentRow();
    int cCol = ui->tableWidget->currentColumn();


    for(int i = 0;i < rowlist.size();i++)
    {
        if(i+cRow < Row)
        {
            QStringList itemlist = rowlist[i].split('\t');
            for(int j = 0;j < itemlist.size();j++)
            {
                if(j+cCol < Col)
                {
                    QTableWidgetItem *item = ui->tableWidget->item(i+cRow,j+cCol);
                    if(item)
                    {
                        item->setText(itemlist[j]);
                    }
                    else
                    {
                        item = new QTableWidgetItem(itemlist[j]);
                        ui->tableWidget->setItem(i+cRow,j+cCol,item);
                    }
                }
            }
        }
    }

    int colSize = rowlist[0].split('\t').size();

    int sRow = (rowlist.size() + cRow < Row)? (rowlist.size() + cRow):Row;
    int sCol = (colSize + cCol < Col)?(colSize + cCol) : Col;

    ui->tableWidget->setRangeSelected(QTableWidgetSelectionRange(cRow,cCol,sRow-1,sCol-1),true);
}

void MainWindow::deleteItem(QTableWidgetItem *item)
{
    delete item;
    qDebug() << "delete item";
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace)
    {
        QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();
        for(auto &i : list)
        {
            i->setText("");
        }
        ui->tableWidget->clearSelection();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    if(exist_table() == false) return;

    Cut(nullptr);
}

void MainWindow::on_actionCut_triggered()
{
    if(exist_table() == false) return;

    Cut(&MainWindow::deleteItem);
}

void MainWindow::on_actionPaste_triggered()
{

    QList<QTableWidgetSelectionRange> list = ui->tableWidget->selectedRanges();
     if(list.size() == 0) return;
    if(list[0].bottomRow() == list[0].topRow() && list[0].leftColumn() == list[0].rightColumn())
    {
        Paste();
    }


}


void MainWindow::on_actionRow_triggered()
{
    QList<QTableWidgetSelectionRange> list = ui->tableWidget->selectedRanges();
    if(list.size() == 0) return;
    if(list[0].bottomRow() == list[0].topRow() && list[0].leftColumn() == list[0].rightColumn())
    {
        ui->tableWidget->selectRow(ui->tableWidget->currentRow());
    }
}




void MainWindow::on_actionCol_triggered()
{
    QList<QTableWidgetSelectionRange> list = ui->tableWidget->selectedRanges();
    if(list.size() == 0) return;

    if(list[0].bottomRow() == list[0].topRow() && list[0].leftColumn() == list[0].rightColumn())
    {
       ui->tableWidget->selectColumn(ui->tableWidget->currentColumn());
    }


}

void MainWindow::on_actionAll_triggered()
{

    ui->tableWidget->selectAll();
}

void MainWindow::on_actionSort_triggered(bool key)
{
    QList<QTableWidgetSelectionRange> range = ui->tableWidget->selectedRanges();
    if(range.size() == 0)
    {
        set_statusBar("您未选择任意一个单元格");
        return;
    }
    else if(range[0].leftColumn() != range[0].rightColumn())
    {
        set_statusBar("不能一次选择多列排序");
        return;
    }

    int column = range[0].leftColumn();
    QVector<QVector<QString>> v;
    QVector<QString> l;

    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        for(int j = 0;j < ui->tableWidget->columnCount();j++)
        {
            QTableWidgetItem * item = ui->tableWidget->item(i,j);
            if(item)
            {
                l.push_back(item->text());
            }
            else l.push_back("");
        }
        v.push_back(l);
        l.clear();
    }
    if(key == true)
    {
        //升序
        std::sort(v.begin(),v.end(),[&](const QVector<QString> &a,const QVector<QString> & b){
            return a[column] < b[column];
        });
        set_statusBar("升序");
    }
    else
    {
        std::sort(v.begin(),v.end(),[&](const QVector<QString> &a,const QVector<QString> & b){
            return a[column] > b[column];
        });
        set_statusBar("降序");
        //降序
    }

    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        for(int j = 0;j < ui->tableWidget->columnCount();j++)
        {
            if(v[i][j] != "")
            {

                QTableWidgetItem * item = ui->tableWidget->item(i,j);
                if(item == nullptr)
                {
                    item = new QTableWidgetItem(v[i][j]);
                }
                else item->setText(v[i][j]);
            }
        }
    }
}

bool MainWindow::read_file(QString filename)
{
    QFile file(filename);

    //文件打开错误
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0,"文件打开错误",QString("cant read file %1,%2")
                             .arg(file.fileName()
                             .arg(file.errorString())));

        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_12);

    quint32 key;
    //quint16 row,col;
    int row,col;
    QString str;
    in >> key >> row >> col;

    //确认文件格式
    if(key != FileNumber)
    {
        QMessageBox::warning(0,"文件格式错误","这不是所需的表格文件\n");
        return false;
    }

    //开始设置表格行列数，并导入文件数据
    set_table(row,col);
    QFont f;
    QString t,b;
    QColor t1,b1;
    int itemNum = 0;
    while(!in.atEnd())
    {
        //输入信息
        in >> row >> col >> str >> f >> t >> b;

        //添加数据项
        t1.setNamedColor(t);
        b1.setNamedColor(b);
        set_Item(row,col,str,f,t1,b1);
        itemNum++;
    }
    qDebug() << "read out " << itemNum << "  items ";
    return true;
}

bool MainWindow::load_file()
{
    QString filename = QFileDialog::getOpenFileName(this,"打开文件:","./File/","file (*.xwj) ");

    if(filename == "") return false;

    if(read_file(filename))
    {
        set_filename(filename);
        noChanged();
        set_statusBar("已加载文件 : " + QFileInfo(filename).fileName());

        return true;
    }

    return false;
}


bool MainWindow::can_match(QString text, QString compare,bool match_case)
{
    if(match_case == true)
    {
        return text == compare;
    }
    else
    {
        return text.toLower() == compare.toLower();
    }
}

bool MainWindow::find_backforward(QString text, bool match_case)
{
    //非循环
    int row,col;
    int Row,Col;
    get_matrixSize(Row,Col);
    if(!get_nextItem(row,col))
    {
        qDebug() << "no next item can find ";
        return false;
    }


    qDebug() << "row " << row << " col " << col;
    int j = col;
    for(int i = row;i < Row;i++)
    {
        for(;j < Col;j++)
        {
            qDebug() << i << " , " << j;
            QTableWidgetItem *item = ui->tableWidget->item(i,j);

            if(item == nullptr) continue;

            QString localtext = item->text();

            if(can_match(text,localtext,match_case))
            {
                set_currentItem(i,j);


                return true;
            }
        }
        j = 0;
    }

    return false;
}

bool MainWindow::find_forward(QString text, bool match_case)
{
    int row,col;
    int Row,Col;
    get_matrixSize(Row,Col);
    if(!get_previousItem(row,col))
    {
        qDebug() << "no previous item can find ";
        return false;
    }


    int j = col;
    for(int i = row;i >= 0;i--)
    {
        for(;j >= 0;j--)
        {
            qDebug() << i << " , " << j;
            QTableWidgetItem *item = ui->tableWidget->item(i,j);
            if(item == nullptr) continue;

            QString localtext = item->text();

            if(can_match(text,localtext,match_case))
            {
                set_currentItem(i,j);

                return true;
            }
        }
        j = Col-1;
    }

    return false;
}

QTableWidgetItem* MainWindow::oneItemSelected()
{
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();

    if(list.size() == 0 || list.size() > 1) return nullptr;

    return (list[0]);
}

void MainWindow::getFindSIGNAL(int row, int col)
{


    int maxRow = ui->tableWidget->rowCount();
    int maxCol = ui->tableWidget->columnCount();

    if(row > maxRow || col > maxCol)
    {
        QMessageBox::critical(nullptr,"范围有误","超出了表格范围\n");
        return;
    }

    //清空选定
    ui->tableWidget->clearSelection();

    if (col == 0)
    {
        // 选中整行
        ui->tableWidget->setCurrentCell(row-1,0);
        ui->tableWidget->selectRow(row-1);

    }
    else if (row == 0)
    {
        // 选中整列
        //selectColumn不会移动光标，只会选中，所以先用setCurrentCell定位
        //Item如果存在，光标会跳转

        //为什么不用Item，因为如果为空，无法设置selected
        //而对table的设置只是一个视图而已，并不要求真的存在
        ui->tableWidget->setCurrentCell(0,col-1);
        ui->tableWidget->selectColumn(col-1);

    }
    else
    {
        //ui->tableWidget->setRangeSelected(QTableWidgetSelectionRange(row-1,col-1,row-1,col-1),true);
        ui->tableWidget->setCurrentCell(row-1,col-1);
    }

    //return true;


}



void MainWindow::getNewDialog()
{
    //设置新表格
    if(ts == nullptr)
    {

    ts = new TableSet(this);

    //如何让信号槽带参数

    /*typedef void (MainWindow::*pointToClear)(int ,int ) ;
    pointToClear point1 = &MainWindow::clearText;*/

    //参数列表一定要匹配
    QObject::connect(ts,SIGNAL(clear(int ,int )),this,SLOT( getClearSIGNAL(int ,int)));

    QObject::connect(ts,&TableSet::getInf,this,&MainWindow::set_table);
    QObject::connect(ts,&TableSet::getInf,[&](){filename = "";});

    }
    qDebug() << ts;
}

void MainWindow::getFindDialog()
{
    if(f == nullptr)
    {
    f = new FindDialog(this);
    QObject::connect(f,&FindDialog::find_next,this,&MainWindow::find_backforward);
    QObject::connect(f,&FindDialog::find_previous,this,&MainWindow::find_forward);
    }

    qDebug() << f;
}

void MainWindow::getGotoWidget()
{
    if(g == nullptr)
    {
        g = new gotoWidget(this);
        QObject::connect(g,SIGNAL(getInf(int ,int)),this,SLOT(getFindSIGNAL(int ,int)));
    }

    qDebug() << g;
}





void MainWindow::start_connect()
{
    //自动检测文本内容改变
    QObject::connect(ui->tableWidget,SIGNAL(itemChanged(Item *)),
                     this,SLOT(isChanged()));

    //右侧文本框改变字体
    QObject::connect(this,&MainWindow::change_Font,this,&MainWindow::get_changeFont);



}

void MainWindow::start_statueFont()
{
    //状态栏字体
    QFont f;
    f.setPointSize(20);

    ui->statusBar->resize(sizeHint().width(),sizeHint().height());
    ui->statusBar->setFont(f);
}

void MainWindow::start_statue()
{
    //文本改变状态
    noChanged();

    //文件名
    filename = "";

    //状态栏
    set_statusBar();
}

void MainWindow::start_tableMenu()
{
    set_tableMenu();
    QObject::connect(ui->tableWidget,&QWidget::customContextMenuRequested,
                     this,&MainWindow::add_tableMenu);

}

void MainWindow::start_widget()
{
    getNewDialog();

    getFindDialog();

    //getColorDialog();

    getGotoWidget();
}



void MainWindow::Start()
{

    start_file();

    start_statueFont();

    start_connect();

    start_statue();

    start_tableMenu();

    start_widget();

}

void MainWindow::set_filename(const QString &s)
{
    this->filename = s;
}

void MainWindow::set_tableMenu()
{
    tableMenu = new QMenu();


    tableMenu->addAction(this->ui->actionCut);
    tableMenu->addAction(this->ui->actionCopy);
    tableMenu->addAction(this->ui->actionPaste);
    tableMenu->addAction(this->ui->actionDelete);
    tableMenu->addMenu(ui->menuSelect);
    tableMenu->addAction(this->ui->actionGoto);
    tableMenu->addAction(this->ui->actionFind);
}

void MainWindow::add_tableMenu()
{
    tableMenu->exec(QCursor::pos());
}

bool MainWindow::exist_table()
{
    bool key = (ui->tableWidget->rowCount() == 0 || ui->tableWidget->columnCount() == 0);
    return !key;
}

void MainWindow::Table(int row, int col ,void(MainWindow::*dosome)(int k,int m))
{
    for(int i = 0;i < row;i++)
    {
        for(int j = 0;j < col;j++)
        {
            (this->*dosome)(i,j);
        }
    }
}




void MainWindow::recycle_ItemMemory(int big_row, int big_col,int row,int col)
{
    //创建新表，或者open其他表，只要没有内容的单元格都是虚的，不占有内存
    //我们的做法是保留所需要的item
    //这样不必每次delete allItem，生成的时候也存在部分单元格，使用也不需要重新创建
    //我们read_file，允许如此操作，生成单元格的策略是先判断有无单元格，没有才生成

    //矩阵形状可能变化

    //要在setRowCount前使用

    int deleteTimes = 0;
    for(int i = row;i < big_row;i++)
    {

        for(int j = 0;j<big_col;j++)
        {

            auto m = ui->tableWidget->item(i,j);
            if(m)
            {
                deleteTimes++;
                delete m;
            }
        }
        //ui->tableWidget->removeRow(row);
    }

    for(int j = col;j<big_col;j++)
    {

        for(int i = 0;i < big_row;i++)
        {
            auto m = ui->tableWidget->item(i,j);
            if(m)
            {
                deleteTimes++;
                delete m;
            }
        }

        //ui->tableWidget->removeColumn(j);
    }


    if(deleteTimes != 0)
    {
        isChanged();
        qDebug() << "delete  " << deleteTimes << "  items ";

    }

}

void MainWindow::set_table(int row, int col)
{
    //清除上次视图，对于new+new有必要，对于open+new没必要
    //没有清除单元格哦！！！

    int big_row = ui->tableWidget->rowCount();
    int big_col = ui->tableWidget->columnCount();

    /*
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    */

    //回收方阵以外的内存
    recycle_ItemMemory(big_row,big_col,row,col);

    ui->tableWidget->setRowCount(row);
    ui->tableWidget->setColumnCount(col);

    //重置方阵内的所有东西


}

void MainWindow::clearItem(int i,int j)
{
    QTableWidgetItem *item = ui->tableWidget->item(i,j);

    if(item)
    {
        qDebug() << "clear" << i << " , " << j;
        item->setText("");
        item->setBackground(QColor(Qt::white));
        item->setForeground(QColor(Qt::black));
    }


}

bool MainWindow::get_nextItem(int &row, int &col)
{
    int next_row,next_col;
    int Row,Col;
    get_matrixSize(Row,Col);
    get_currenItem(next_row,next_col);
    if(next_col + 1 < Col)
    {
        next_col++;
    }
    else
    {
        if(next_row + 1 < Row)
        {
            next_row++;
            next_col = 0;
        }
        else
        {
            qDebug() << "no next item";
            return false;
        }
    }

    row = next_row;
    col = next_col;

    qDebug() << "next item :" << row << " , " << col;

    return true;
}

bool MainWindow::get_previousItem(int &row, int &col)
{
    int previous_row,previous_col;
    int Row,Col;
    get_matrixSize(Row,Col);
    get_currenItem(previous_row,previous_col);
    if(previous_col - 1 >= 0)
    {
        previous_col--;
    }
    else
    {
        if(previous_row - 1 >= 0)
        {
            previous_row--;
            previous_col = Col-1;
        }
        else
        {
            qDebug() << "no previous item";
            return false;
        }
    }

    row = previous_row;
    col = previous_col;

    qDebug() << "previous item :" << row << " , " << col;

    return true;
}



void MainWindow::get_matrixSize(int &row, int &col)
{
    row = ui->tableWidget->rowCount();
    col = ui->tableWidget->columnCount();
    qDebug() << "matrix size :" << row << " , " << col;
}



void MainWindow::get_currenItem(int &row, int &col)
{
    row = ui->tableWidget->currentRow();
    col = ui->tableWidget->currentColumn();
    qDebug() << "currentItem :" << row << " , " << col;
}



void MainWindow::set_currentItem(int row, int col)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,col);
    if(item)
    {
        ui->tableWidget->setCurrentItem(item);
        qDebug() << "set currentItem : " << row << " , " << col;

        set_statusBar("( " + QString::number(row+1) + " , " + QString::number(col+1) +" )");
    }

}

void MainWindow::set_Item(int row, int col,const QString & s,const QFont &f,const QColor &textcolor,const QColor &backgourndcolor)
{
    //注意数据项的函数是item
    //不是itemAt
    QTableWidgetItem *item = ui->tableWidget->item(row,col);

    if(item == nullptr)
    {
        item = new QTableWidgetItem();

        ui->tableWidget->setItem(row,col,item);
    }

    item->setText(s);
    item->setFont(f);
}


void MainWindow::create_directory()
{
    QDir *dir = new QDir();


    if(!dir->exists("./File"))
    {
        dir->mkdir("./File");
    }

    delete(dir);
}


void MainWindow::set_statusBar(const QString & s)
{

    ui->statusBar->showMessage(s,3500);

}



void MainWindow::on_spinBox_valueChanged(int arg1)
{
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();

    for(int i = 0;i < list.size();i++)
    {
        QFont f = list[i]->font();
        f.setPointSize(arg1);
        list[i]->setFont(f);
    }
}


void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();

    for(int i = 0;i < list.size();i++)
    {
        QFont m = list[i]->font();
        m.setFamily(f.family());

        list[i]->setFont(m);
    }
}


void MainWindow::on_pushButton_toggled(bool checked)
{
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();

    for(int i = 0;i < list.size();i++)
    {
        QFont f = list[i]->font();
        f.setBold(checked);
        list[i]->setFont(f);
    }

    ui->tableWidget->clearSelection();
}


void MainWindow::on_pushButton_2_toggled(bool checked)
{
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();

    for(int i = 0;i < list.size();i++)
    {
        QFont f = list[i]->font();
        f.setItalic(checked);
        list[i]->setFont(f);
    }

    ui->tableWidget->clearSelection();
}


void MainWindow::on_pushButton_3_toggled(bool checked)
{
    QList<QTableWidgetItem*> list = ui->tableWidget->selectedItems();

    for(int i = 0;i < list.size();i++)
    {
        QFont f = list[i]->font();
        f.setUnderline(checked);
        list[i]->setFont(f);
    }

    ui->tableWidget->clearSelection();
}



void MainWindow::on_tableWidget_itemSelectionChanged()
{
    QTableWidgetItem *item = oneItemSelected();
    if(item == nullptr) return;

    qDebug() << item->font();
    emit change_Font(item->font());
}

void MainWindow::get_changeFont(const QFont &f)
{
    ui->fontComboBox->setCurrentFont(f);
    ui->spinBox->setValue(f.pointSize());

    ui->pushButton->setChecked(f.bold());
    ui->pushButton_2->setChecked(f.italic());
    ui->pushButton_3->setChecked(f.underline());
}

void MainWindow::on_pushButton_6_clicked()
{
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        for(int j = 0;j < ui->tableWidget->columnCount();j++)
        {
            qDebug() << i << " , " << j ;
        }
    }
}
