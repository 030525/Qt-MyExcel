#ifndef ITEM_H
#define ITEM_H

#include <QTableWidgetItem>
#include <QRgbaFloat16>
//#include <QObject>

class Item : public QTableWidgetItem
{

public:
    Item();
    Item(QTableWidgetItem item);
    Item(QString & s);
    Item(QFont & f);
    Item(QFont & f,QColor &c,QColor & m);
    QFont getFont()const ;
    QColor getTextColor()const;
    QColor getbackgroundColor() const;

    virtual QTableWidgetItem* clone()const override;

    void Set(const QFont & f,const QColor & c,const QColor & b);

private:
    QFont font;
    QColor textColor;
    QColor backgroundColor;
};

#endif // ITEM_H
