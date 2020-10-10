#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include "calendarwidget.h"

enum ARIZALAR{
    OZUR_KANI = 0,
    DEVAMLI_KANAMA = 1,
    NU_LL = 99
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    ARIZALAR m_arizaType= NU_LL;
    int m_prevDevamliKanamaTGun, m_prevOzurKGun = 0, m_prevTemiz = 0, m_prevTemizIdx = 0;
    int m_normalT, m_normalK, m_prevPrevTemiz, m_prevOzur;
    QList<int> m_kirliler, m_temizler;
    QList<int> m_kirlilerS, m_temizlerS;
    bool m_temizdenHukmi = false;

    bool m_temizSPopped = true;
    QList<QDate> m_kirliTarihler, m_temizTarihler;
    QStringList m_list;
    bool m_ozurKaniTemizDevam = false;
    bool m_nextTemizFasit = false;
    bool m_aSolved = false;
    bool m_addNextTemiz = false;
    int m_nextTemizEklenecek;

    bool m_processForError;

    void solveA();
    void solveB();
    void solveC();


private slots:
    void startTemiz();
    void finishTemiz();
    void startKirli();
    void finishKirli();

};
#endif // MAINWINDOW_H
