#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QPainter>
#include <QBrush>
#include <QList>
#include <QDate>
#include <QDebug>
class CalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = nullptr);
    void setHighlightType(int t)
    {
        m_highlightType = t;
    }

    void resetTemizGunler()
    {
        m_temizGunler->clear();
    }
    void resetKirliGunler()
    {
        m_kirliGunler->clear();
    }



protected:


    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;




private:
    int m_highlightType = 0;
    QList<QDate> *m_temizGunler, *m_kirliGunler, *m_kazaGunleri, *m_kilinacakGunler;




signals:

};

#endif // CALENDARWIDGET_H
