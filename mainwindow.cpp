#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QString filename;

    filename = ":/qss/coffee.qss";


    QFile styleFile(filename );
    styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    QString style( styleFile.readAll() );
    this->setStyleSheet( style );


    m_addNextTemiz = false;

    ui->startDateEdit->setDate(QDate::currentDate());

    connect(ui->startTemizPushButton, SIGNAL(clicked()),this, SLOT(startTemiz()));
    connect(ui->finishTemizPushButton, SIGNAL(clicked()),this, SLOT(finishTemiz()));
    connect(ui->startKirliPushButton, SIGNAL(clicked()),this, SLOT(startKirli()));
    connect(ui->finishKirliPushButton, SIGNAL(clicked()),this, SLOT(finishKirli()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    m_temizlerS.clear();
    m_kirlilerS.clear();
    ui->messageTextEdit->clear();
    m_temizler.clear();
    m_kirliler.clear();
    m_kirliTarihler.clear();
    m_temizTarihler.clear();

    m_arizaType = NU_LL;


    m_nextTemizFasit = false;
        m_ozurKaniTemizDevam = false;

        QString input = ui->lineEdit->text();
        bool isProcessingT = true;
        m_list = input.split("-");



        int n = 3;
        QString ilkStr = m_list.at(0);
        int ilk = ilkStr.remove("-").toInt();
        int ilkT = 0, ilkK = 0, ikinciT = 0;

        if(ilk >14){
            isProcessingT = false;
            QString ilkTStr = m_list.at(0);
            ilkT = ilkTStr.remove("-").toInt();

            QString ilkKStr = m_list.at(1);
            ilkK = ilkKStr.remove("-").toInt();

            QString ikinciTStr = m_list.at(2);
            ikinciT = ikinciTStr.remove("-").toInt();

            if(m_list.size() < 4){
                QMessageBox msgBox;
                qDebug() << "invalid input";
                msgBox.setText("Yeterli giriş sağlanamamıştır. En az 4 dönem bilgisi girilmelidir.");
                msgBox.exec();
            }

            n = 3;
        }else{
            isProcessingT = true;
            QString ilkTStr = m_list.at(1);
            ilkT = ilkTStr.remove("-").toInt();

            QString ilkKStr = m_list.at(2);
            ilkK = ilkKStr.remove("-").toInt();

            QString ikinciTStr = m_list.at(3);
            ikinciT = ikinciTStr.remove("-").toInt();



            if(m_list.size() < 5){
                QMessageBox msgBox;
                qDebug() << "invalid input";
                msgBox.setText("Yeterli giriş sağlanamamıştır. En az 5 dönem bilgisi girilmelidir.");
                msgBox.exec();
            }

            n = 4;
        }




    if(ilkT < 15){
        QMessageBox msgBox;
        qDebug() << "invalid data";
        msgBox.setText("İlk temiz gün sayısı 15ten düşük.");
        msgBox.exec();
    }

    if(ilkK > 10){
        QMessageBox msgBox;
        qDebug() << "invalid data";
        msgBox.setText("İlk kirli gün sayısı 10dan yüksek.");
        msgBox.exec();
    }

    m_normalT = ikinciT;
    m_normalK = ilkK;


    isProcessingT = false;

    m_prevTemiz = ikinciT;
    m_prevTemizIdx = 2;
    m_prevPrevTemiz = ilkT;

    m_temizler.push_back(ilkT);
    m_temizler.push_back(ikinciT);
    m_temizlerS.push_back(ilkT);
    m_temizlerS.push_back(ikinciT);
    m_kirliler.push_back(ilkK);
    m_kirlilerS.push_back(ilkK);

    m_temizdenHukmi = false;

    for(int i = n; i < m_list.size(); i++){
        QString str = m_list.at(i);
        int val = str.remove("-").toInt();

        m_processForError = true;

        qDebug() << "\n\n\n" <<i << "  " << val << "  " << m_arizaType << "   " << m_nextTemizFasit;

        ui->messageTextEdit->append("___________" + QString::number(i) + "_________");

        ///kirli
        if(!isProcessingT){



            QString ddd = "kirli birinc m_temizlerS,:  ";
            for(int t = 0; t < m_temizlerS.size(); t++){
                ddd += "  " + QString::number(m_temizlerS.at(t));
            }

            qDebug() << ddd;
            m_kirliler.push_back(val);


            ///sahih kontrolleri
            ///



           ///           17-6-18-5-19-4-6-8-20-6-21-2-14-1

            if(val < 11 && val > 2){
                if(m_temizdenHukmi){
                    if(m_kirliler.size() > 0){
                        m_kirliler.pop_back();
                        if(m_kirliler.size() > 0)
                            m_kirliler.replace(m_kirliler.size()-1, m_kirliler.last()+val);
                    }
                }else{
                    m_kirlilerS.push_back(val);
                }
            }else{
                if(m_temizdenHukmi){
                    m_kirliler.pop_back();
                    m_kirliler.replace(m_kirliler.size()-1, m_kirliler.last()+val);

                }
            }



            if((val > 10 || val < 3) && !m_temizdenHukmi){

                if(m_temizlerS.size() > 1)
                    m_temizlerS.pop_back();
            }
            //-------------------

            QString str = "kirli ikinci m_temizlerS:  ";
            for(int t = 0; t < m_temizlerS.size(); t++){
                str += "  " + QString::number(m_temizlerS.at(t));
            }

            qDebug() << str;



            QString eee = "kirlilerS kirlide:  ";
            for(int t = 0; t < m_kirlilerS.size(); t++){
                eee = eee + "  " + QString::number(m_kirlilerS.at(t));
            }

            qDebug() << eee;


            ///HAKİKİ İSTİMRAR
            ///
            if(val > 10  && m_processForError){

                m_processForError = false;
                ui->messageTextEdit->append("HAKİKİ İSTİMRAR...");
                ui->messageTextEdit->append("Son Kirli: " + QString::number(m_kirliler.last()));
                qDebug() << "m_kirliler.last() " << m_kirliler.last();
                qDebug() << "m_kirlilerS.last() " << m_kirlilerS.last();

                qDebug() << "m_temizler.last() " << m_temizler.last();
                qDebug() << "m_temizlerS.last() " << m_temizlerS.last();

                if( m_temizlerS.last() >= m_temizler.last()
                        && m_kirliler.last() - (m_temizlerS.last() -  m_temizler.last()) >= m_kirlilerS.last()){
                    solveA();

                    //20-8-18-7-21-11
                }else if(2 >= m_kirlilerS.last() - (m_temizler.last() - m_temizlerS.last())
                         || m_kirliler.last() - (m_temizler.last() -m_temizlerS.last()) < 3
                         || ( (m_temizler.last() < m_temizlerS.last()) &&  (m_temizler.last() + m_kirliler.last() - m_temizlerS.last() < 3))){
                    solveC();
                }
                else if(( 2 < m_kirlilerS.last() - (m_temizlerS.last() - m_temizler.last()) &&  (m_temizlerS.last() - m_temizler.last()) > 0)
                         || /*(m_kirlilerS.last() > m_kirliler.last() - (m_temizler.last() -m_temizlerS.last()) &&*/
                             m_kirliler.last() - (m_temizler.last() -m_temizlerS.last()) > 2){
                    solveB();
                }


            }




/// 17-6-18-5-19-4-6-8-20-6-21-2-14-1

            ///HÜKMĞ İSTİMRAR
            /// 45-8-47-8-21-15

            if( m_processForError && (m_temizler.last() < 15 || m_temizdenHukmi) ){
                m_processForError = false;
                m_temizdenHukmi = false;
                ui->messageTextEdit->append("HÜKMî İSTİMRAR...");

                ui->messageTextEdit->append("Son Kirli: " + QString::number(m_kirliler.last()));


                qDebug() << "m_kirliler.last() " << m_kirliler.last();
                qDebug() << "m_kirlilerS.last() " << m_kirlilerS.last();
                qDebug() << "m_temizler.last() " << m_temizler.last();
                qDebug() << "m_temizlerS.last() " << m_temizlerS.last();


                qDebug() << "\nHükmi: ";
                QString ddd = "m_temizler:  ";
                for(int t = 0; t < m_temizler.size(); t++){
                    ddd = ddd + "  " + QString::number(m_temizler.at(t));
                }

                qDebug() << ddd;

                if( m_temizlerS.last() >= m_temizler.last()
                        && m_kirliler.last() - (m_temizlerS.last() -  m_temizler.last()) >= m_kirlilerS.last()){
                    solveA();
                }else if(2 >= m_kirlilerS.last() - (m_temizler.last() - m_temizlerS.last())
                         || m_kirliler.last() - (m_temizler.last() -m_temizlerS.last()) < 3
                         || ( (m_temizler.last() < m_temizlerS.last()) &&  (m_temizler.last() + m_kirliler.last() - m_temizlerS.last() < 3))){
                    solveC();
                }
                else if(( 2 < m_kirlilerS.last() - (m_temizlerS.last() - m_temizler.last()) &&  (m_temizlerS.last() - m_temizler.last()) > 0)
                         || /*(m_kirlilerS.last() > m_kirliler.last() - (m_temizler.last() -m_temizlerS.last()) &&*/
                             m_kirliler.last() - (m_temizler.last() -m_temizlerS.last()) > 2){
                    solveB();
                }




            }








            isProcessingT = true;
        }




























        /*  TEMİZ
         *
         *
         *
         *
         *
         * */

















        ///temiz
        else{
            ui->messageTextEdit->append("sahih Temiz gün sayısı: " + QString::number(m_temizlerS.last()));
            ui->messageTextEdit->append("sahih Kirli gün sayısı: " + QString::number(m_kirlilerS.last()));

            m_temizler.push_back(val);


            QString str = "temiz giriş:  ";
            for(int t = 0; t < m_temizlerS.size(); t++){
                str += "  " + QString::number(m_temizlerS.at(t));
            }

            qDebug() << str;




            qDebug() << "temmiz baş: ";
            QString ddd = "m_temizler:  ";
            for(int t = 0; t < m_temizler.size(); t++){
                ddd = ddd + "  " + QString::number(m_temizler.at(t));
            }
            qDebug() << ddd;

            qDebug() << "-------------m_addNextTemiz " << m_addNextTemiz << " : " << m_nextTemizEklenecek;



            /*
             *
             * 17-6-18-5-19-4-6-8-20-6-21-2-14-1
             * 20-6-21-2-14-1
             *
             *
             * */


///20-5-18-6-22-4-12-8
            ///sahih kontrolleri
            if(val > 14){
                m_temizlerS.push_back(val);

                if(m_kirliler.last() > 10 || m_kirliler.last() < 3){
                    if(m_temizlerS.size() > 1)
                        m_temizlerS.pop_back();
                }
                if(m_addNextTemiz){
                    m_addNextTemiz = false;
                    qDebug() << "---*-*-*-*---m_addNextTemiz " << m_addNextTemiz << " : " << m_nextTemizEklenecek;

                    int last = m_temizler.last();
                    int eklenecek = last + m_nextTemizEklenecek;
                    qDebug() << "---*-*-*-*---last " << last;
                    qDebug() << "---*-*-*-*---eklenecek " << eklenecek;

                    QList<int> temizler;
                    for(int i = 0; i < m_temizler.size(); i++){
                        if(i < m_temizler.size()-1)
                            temizler.push_back(m_temizler.at(i));
                        else
                            temizler.push_back(eklenecek);

                    }
                    m_temizler.clear();
                    m_temizler = temizler;


                    qDebug() << "temmiz baş: ";
                    QString ddd = "m_temizler:  ";
                    for(int t = 0; t < m_temizler.size(); t++){
                        ddd = ddd + "  " + QString::number(m_temizler.at(t));
                    }
                    qDebug() << ddd;

                    qDebug() << "-------------m_addNextTemiz " << m_addNextTemiz << " : " << m_nextTemizEklenecek;

                }




            }

            if(val < 15){







                qDebug() << "ekleme sonrası baş: ";
                QString ddd = "m_temizler:  ";
                for(int t = 0; t < m_temizler.size(); t++){
                    ddd = ddd + "  " + QString::number(m_temizler.at(t));
                }
                qDebug() << ddd;

                qDebug() << "-------------m_addNextTemiz " << m_addNextTemiz;



                QString ttt = "kirlilerS1:  ";
                for(int t = 0; t < m_kirlilerS.size(); t++){
                    ttt = ttt + "  " + QString::number(m_kirlilerS.at(t));
                }

                qDebug() << ddd;

                if(m_kirliler.last() < 3 || m_kirliler.last() > 10)
                    m_temizlerS.push_back(m_temizler.last());


                if(m_kirlilerS.size() > 1  && m_kirliler.last() > 2 &&  m_kirliler.last() <11)
                    m_kirlilerS.pop_back();


                if(m_kirliler.size() > 0)
                    m_kirliler.replace(m_kirliler.size()-1, m_kirliler.last()+val);

                if(m_temizler.size() > 0)
                    m_temizler.pop_back();


///21-8-24-14-18-5-30-11-17-7-33-12-20-7-50-14-18-7-20-12




                if(m_temizlerS.size() > 1/* && (m_kirliler.last() > 2 && m_kirliler.last() < 11)*/){
                    qDebug() << m_temizlerS.last() << " popped.";
                    m_temizlerS.pop_back();

                }




                QString eee = "kirlilerS2:  ";
                for(int t = 0; t < m_kirlilerS.size(); t++){
                    eee = eee + "  " + QString::number(m_kirlilerS.at(t));
                }

                qDebug() << eee;


                ///18-8-20-5-23-2-9-3
                ///
                ///
                m_temizdenHukmi = true;
                //qDebug() << "ddm_temizdenHukmi" << m_temizdenHukmi;
              /* if(m_kirliler.size() > 0)
                qDebug() << "m_kirliler.last(): " << m_kirliler.last();

                qDebug() << "m_temizlerS.m_temizlerS(): " << m_temizlerS.size();*/

            }


            //----------------





            if(val > 15  &&  m_temizler.at(m_temizler.size()-2) > 15 && m_kirliler.last() < 3){
                int temizler = 0;
                for(int a = 0; a < m_temizler.size()-1; a++){
                    temizler += m_temizler.at(a);
                }
                int kirliler = 0;
                for(int a = 0; a < m_kirliler.size()-1; a++){
                    kirliler += m_kirliler.at(a);
                }
                QString dates = "";
                for(int a = 0; a < m_kirliler.last(); a++){
                    dates += ui->startDateEdit->date().addDays(kirliler+temizler+a+1).toString()+"\n\n";
                }

                QString msg = QString::number(m_kirliler.size()+1) + ". kirli\n" + QString::number(m_kirliler.last()) + " günü kaza et.\n";
                msg = msg + "\n\n" + dates;

                ui->messageTextEdit->append(msg);
                QMessageBox msgBox;
                msgBox.setText(msg);
                msgBox.exec();
            }





            QString ttt = "temiz çıkış m_temizlerS:  ";
            for(int t = 0; t < m_temizlerS.size(); t++){
                ttt = ttt + "  " + QString::number(m_temizlerS.at(t));
            }

            qDebug() << ddd;



            isProcessingT = false;

        }








    }
















    int totalDays = 0;
    for(int i = 0; i < m_list.size(); i++){
        QString str = m_list.at(i);
        totalDays += str.remove("-").toInt();
    }

    ui->finishDateEdit->setDate(ui->startDateEdit->date().addDays(totalDays));

}




void MainWindow::solveA()
{

    m_nextTemizFasit = false;
    ui->messageTextEdit->append("___________________________\n");
    ui->messageTextEdit->append("sahih Temiz gün sayısı: " + QString::number(m_temizlerS.last()));
    ui->messageTextEdit->append("sahih Kirli gün sayısı: " + QString::number(m_kirlilerS.last()));

    ui->messageTextEdit->append("\n\nA ŞIKKI.\n");


    int oncekiKazaSayisi = m_temizlerS.last()- m_temizler.last();

    int ilkKazaGecenGun = 0;

    for(int i = 0; i < m_temizler.size(); i++){
        ilkKazaGecenGun = ilkKazaGecenGun + m_temizler.at(i);
    }

    for(int i = 0; i < m_kirliler.size()-1; i++){
        ilkKazaGecenGun = ilkKazaGecenGun + m_kirliler.at(i);
    }

   QString dates = "";


   if(oncekiKazaSayisi < 0)
       oncekiKazaSayisi = 0;

   for(int i = 0; i < oncekiKazaSayisi; i++){
       QDate dt = ui->startDateEdit->date().addDays(ilkKazaGecenGun+i);
        dates.append(dt.toString()+"\n");
   }

   QString msg = "Şu " + QString::number(oncekiKazaSayisi) + " gün kaza edilecek:\n\n";


   //// 23-8-21-15-16-7-20-4-7-3-21-5-25-2-3-9

   ui->messageTextEdit->append("\n\n" + msg + dates + "\n");


/// 17-6-18-5-19-4-6-8-20-6-21-2-14-1

   int sonrakiKilinacakSayisi = m_kirliler.last() - oncekiKazaSayisi - m_kirlilerS.last();
   int diffSonKan = m_kirliler.last() - sonrakiKilinacakSayisi;
   int sonradanKilinacakGecenGun = ilkKazaGecenGun + diffSonKan;

   if(sonrakiKilinacakSayisi < 0)
       sonrakiKilinacakSayisi = 0;

      dates = "";
      for(int i = 0; i < sonrakiKilinacakSayisi; i++){
          QDate dt = ui->startDateEdit->date().addDays(sonradanKilinacakGecenGun+i);
          dates.append(dt.toString()+"\n");
      }

     /* m_temizler.replace(m_temizler.size()-1, m_temizler.last()+sonrakiKilinacakSayisi);
      m_temizdenHukmi = true;
*/
     // m_temizdenHukmi = true;
      m_addNextTemiz = true;
      m_nextTemizEklenecek = sonrakiKilinacakSayisi;




      msg = "Şu " + QString::number(sonrakiKilinacakSayisi) + " gün namaz kılınmalı:\n\n";
      ui->messageTextEdit->append("\n\n" + msg + dates + "\n");




}

///19-5-18-12-21-7-16-11

///25-5-15-15-21-7-16-11


void MainWindow::solveB()
{

    ui->messageTextEdit->append("sahih Temiz gün sayısı: " + QString::number(m_temizlerS.last()));
    ui->messageTextEdit->append("sahih Kirli gün sayısı: " + QString::number(m_kirlilerS.last()));

    ui->messageTextEdit->append("\n\nB ŞIKKI.\n");


    qDebug() << "B'ye girerken: ";
    QString sss = "m_temizler:  ";
    for(int t = 0; t < m_temizler.size(); t++){
        sss = sss + "  " + QString::number(m_temizler.at(t));
    }

    qDebug() << sss;

    int oncekiKazaSayisi = m_temizlerS.last()- m_temizler.last();

    int ilkKazaGecenGun = 0;

    for(int i = 0; i < m_temizler.size(); i++){
        ilkKazaGecenGun = ilkKazaGecenGun + m_temizler.at(i);
    }

    for(int i = 0; i < m_kirliler.size()-1; i++){
        ilkKazaGecenGun = ilkKazaGecenGun + m_kirliler.at(i);
    }


    if(oncekiKazaSayisi < 0)
        oncekiKazaSayisi = 0;

    QString dates = "";

    for(int i = 0; i < oncekiKazaSayisi; i++){
        QDate dt = ui->startDateEdit->date().addDays(ilkKazaGecenGun+i);
        dates.append(dt.toString()+"\n");
    }

    QString msg = "Şu " + QString::number(oncekiKazaSayisi) + " gün kaza edilecek:\n\n";
    ui->messageTextEdit->append("\n\n" + msg + dates + "\n");


    if((m_temizler.last() - m_temizlerS.last()) > 0)
        m_kirlilerS.push_back(m_kirlilerS.last() - (m_temizler.last() - m_temizlerS.last()));
    else
        m_kirlilerS.push_back( - (m_temizler.last() - m_temizlerS.last()));



    ui->messageTextEdit->append("B  sahih Kirli gün sayısı değişti: " + QString::number(m_kirlilerS.last()));



    if(m_temizler.last() > m_temizlerS.last()){
        m_temizlerS.push_back(m_temizler.last());
        m_temizlerS.push_back(m_temizler.last());

        ui->messageTextEdit->append("B  sahih Temiz gün sayısı değişti: " + QString::number(m_temizlerS.last()));
    }



   int sonrakiKilinacakSayisi = m_kirliler.last() - oncekiKazaSayisi - m_kirlilerS.last();




   int sahT = m_temizlerS.last();
   int sahK = m_kirlilerS.last();
   qDebug() << "---sonrakiKilinacakSayisi: " << sonrakiKilinacakSayisi;

   if(sonrakiKilinacakSayisi < 0)
       sonrakiKilinacakSayisi = 0;

   if(sonrakiKilinacakSayisi <= sahT){
       int diffSonKan = m_kirliler.last() - sonrakiKilinacakSayisi;

       int sonradanKilinacakGecenGun = ilkKazaGecenGun + diffSonKan;

       dates = "";
       for(int i = 0; i < sonrakiKilinacakSayisi; i++){
           QDate dt = ui->startDateEdit->date().addDays(sonradanKilinacakGecenGun+i);
           dates.append(dt.toString()+"\n");
       }

       //20-5-22-6-24-13

      /* m_temizler.replace(m_temizler.size()-1, m_temizler.last()+sonrakiKilinacakSayisi);
       m_temizdenHukmi = true;
*/
      // m_temizdenHukmi = true;
       m_addNextTemiz = true;
       m_nextTemizEklenecek = sonrakiKilinacakSayisi;



       msg = "Şu " + QString::number(sonrakiKilinacakSayisi) + " gün namaz kılınmalı:\n\n";
       ui->messageTextEdit->append("\n\n" + msg + dates + "\n");
   }



   else{
       bool isTemiz = true;
       while(sonrakiKilinacakSayisi > 1){
           qDebug() << "---sonrakiKilinacakSayisi: " << sonrakiKilinacakSayisi;

           if(isTemiz){

               int kilinacakMiktar;
               if(sonrakiKilinacakSayisi > sahT){
                   kilinacakMiktar = sahT;
               }else{
                   kilinacakMiktar = sonrakiKilinacakSayisi;
               }
               int diffSonKan = m_kirliler.last() - kilinacakMiktar;

               int sonradanKilinacakGecenGun = ilkKazaGecenGun + diffSonKan;

               dates = "";
               for(int i = 0; i < kilinacakMiktar; i++){
                   QDate dt = ui->startDateEdit->date().addDays(sonradanKilinacakGecenGun+i);
                   dates.append(dt.toString()+"\n");
               }

               //20-5-22-6-24-13


               qDebug() << "sonrakiKilinacakSayisi: " << sonrakiKilinacakSayisi;
               qDebug() << "kilinacakMiktar: " << kilinacakMiktar;



              /* m_temizler.replace(m_temizler.size()-1, m_temizler.last()+kilinacakMiktar);
               m_temizdenHukmi = true;
*/

             //  m_temizdenHukmi = true;
               m_addNextTemiz = true;
               m_nextTemizEklenecek = sonrakiKilinacakSayisi;


               msg = "Şu " + QString::number(kilinacakMiktar) + " gün namaz kılınmalı:\n\n";
               ui->messageTextEdit->append("\n\n" + msg + dates + "\n");

                sonrakiKilinacakSayisi -= kilinacakMiktar;
                isTemiz = false;
           }else{
               sonrakiKilinacakSayisi -= sahK;
               msg = QString::number(sahK) + " gün hayız.\n";
               ui->messageTextEdit->append( msg + "\n");
               isTemiz = true;
           }

       }
   }

   ///20-3-22-7-24-12-18-11-19-15


   qDebug() << "B'den çıkarken: ";
   QString ddd = "m_temizler:  ";
   for(int t = 0; t < m_temizler.size(); t++){
       ddd = ddd + "  " + QString::number(m_temizler.at(t));
   }
   qDebug() << ddd;

   qDebug() << "-------------m_addNextTemiz " << m_addNextTemiz << " : " << m_nextTemizEklenecek;

}






void MainWindow::solveC()
{

    ui->messageTextEdit->append("sahih Temiz gün sayısı: " + QString::number(m_temizlerS.last()));
    ui->messageTextEdit->append("sahih Kirli gün sayısı: " + QString::number(m_kirlilerS.last()));

    ui->messageTextEdit->append("\n\nC ŞIKKI.\n");


    int sonradanKilinacak = m_kirliler.last() - m_kirlilerS.last();

    int diffSonKan = m_kirliler.last() - sonradanKilinacak;

    int ilkKazaGecenGun = 0;

    for(int i = 0; i < m_temizler.size(); i++){
        ilkKazaGecenGun = ilkKazaGecenGun + m_temizler.at(i);
    }

    for(int i = 0; i < m_kirliler.size()-1; i++){
        ilkKazaGecenGun = ilkKazaGecenGun + m_kirliler.at(i);
    }

    int sonradanKilinacakGecenGun = ilkKazaGecenGun + diffSonKan;


    if(sonradanKilinacak < 0)
        sonradanKilinacak = 0;


    QString dates = "";
    for(int i = 0; i < sonradanKilinacak; i++){
        QDate dt = ui->startDateEdit->date().addDays(sonradanKilinacakGecenGun+i);
        dates.append(dt.toString()+"\n");
    }


   /* m_temizler.replace(m_temizler.size()-1, m_temizler.last()+sonradanKilinacak);
    m_temizdenHukmi = true;

*/

  //  m_temizdenHukmi = true;
    m_addNextTemiz = true;
    m_nextTemizEklenecek = sonradanKilinacak;


    //20-5-22-6-24-13
    m_temizlerS.push_back(m_temizler.last());
    m_temizlerS.push_back(m_temizler.last());

    ui->messageTextEdit->append("C  sahih Temiz gün sayısı değişti: " + QString::number(m_temizlerS.last()));

    QString msg = "Şu " + QString::number(sonradanKilinacak) + " gün namaz kılınmalı:\n\n";
    ui->messageTextEdit->append("\n\n" + msg + dates + "\n");






}

void MainWindow::startTemiz()
{
    ui->calendarWidget->setSelectedDate(QDate(0,0,0));
    //ui->calendarWidget->resetTemizGunler();
    ui->calendarWidget->setHighlightType(1);
}



void MainWindow::finishTemiz()
{
    ui->calendarWidget->setSelectedDate(QDate(0,0,0));

    ui->calendarWidget->setHighlightType(0);

}



void MainWindow::startKirli()
{
    ui->calendarWidget->setSelectedDate(QDate(0,0,0));
  //  ui->calendarWidget->resetKirliGunler();
    ui->calendarWidget->setHighlightType(2);

}



void MainWindow::finishKirli()
{
    ui->calendarWidget->setSelectedDate(QDate(0,0,0));

    ui->calendarWidget->setHighlightType(0);

}
