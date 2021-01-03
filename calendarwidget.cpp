#include "calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent) : QCalendarWidget(parent)
{

    m_temizGunler = new QList<QDate>;
    m_kirliGunler = new QList<QDate>;
    m_kazaGunleri = new QList<QDate>;
    m_kilinacakGunler = new QList<QDate>;

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
        else if(m_highlightType == 3){
            painter->setPen(Qt::yellow);
            brs.setColor(Qt::yellow);
           m_kazaGunleri->push_back(date);
        }
        else if(m_highlightType == 4){
            painter->setPen(QColor(148,0,211));
            brs.setColor(QColor(148,0,211));
           m_kilinacakGunler->push_back(date);
        }
    }


    // 25-5-20-8-12-5-20


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

    if(m_kazaGunleri->contains(date)){
        painter->setPen(Qt::yellow);
        painter->fillRect(rect, QColor(255, 255, 0, 128));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
    }

    if(m_kilinacakGunler->contains(date)){
        painter->setPen(Qt::yellow);
        painter->fillRect(rect, QColor(148, 0, 211, 128));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
    }

}

