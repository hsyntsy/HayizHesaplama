#include "calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent) : QCalendarWidget(parent)
{

    m_temizGunler = new QList<QDate>;
    m_kirliGunler = new QList<QDate>;
}

void CalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{


    QCalendarWidget::paintCell(painter, rect, date);


    QBrush brs;
    if(date == selectedDate()){
        if(m_highlightType == 1){       //temiz
           painter->setPen(Qt::green);
            brs.setColor(Qt::green);
           m_temizGunler->push_back(date);

       }else if(m_highlightType == 2){
            painter->setPen(Qt::red);
            brs.setColor(Qt::red);
           m_kirliGunler->push_back(date);
        }
    }


    if(m_kirliGunler->contains(date)){
        painter->setPen(Qt::red);
        painter->fillRect(rect, QColor(255, 0, 0, 128));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
    }


    if(m_temizGunler->contains(date)){
        painter->setPen(Qt::green);
        painter->fillRect(rect, QColor(0, 255, 0, 128));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
    }

}

