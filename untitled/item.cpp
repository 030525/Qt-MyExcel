#include "item.h"

Item::Item()
    :font(QFont()),textColor(QColor(Qt::black)),backgroundColor(QColor(Qt::white))
{

}

Item::Item(QTableWidgetItem item)
    :font(item.font()),textColor(item.foreground().color()),backgroundColor(item.background().color())
{

}

Item::Item(QString &s)
    :QTableWidgetItem(s),font(QFont()),textColor(QColor(Qt::black)),backgroundColor(QColor(Qt::white))
{

}

Item::Item(QFont &f)
    :font(f),textColor(QColor(Qt::black)),backgroundColor(QColor(Qt::white))
{

}

Item::Item(QFont &f, QColor &c, QColor &m)
    :font(f),textColor(c),backgroundColor(m)
{

}

QFont Item::getFont() const
{
    return this->font;
}

QColor Item::getTextColor() const
{
    return this->textColor;
}

QColor Item::getbackgroundColor() const
{
    return this->backgroundColor;
}





QTableWidgetItem *Item::clone() const
{
    return new Item(*this);
}

void Item::Set(const QFont &f, const QColor &c, const QColor &b)
{
    this->setFont(f);
    this->setForeground(c);
    this->setBackground(b);
}


