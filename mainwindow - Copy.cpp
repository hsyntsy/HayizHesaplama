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

    if(m_list.size() < 4){
        QMessageBox msgBox;
        qDebug() << "invalid input";
        msgBox.setText("Yeterli giriş sağlanamamıştır. En az 4 dönem bilgisi girilmelidir.");
        msgBox.exec();
    }


    int ilkT = 0, ilkK = 0, ikinciT = 0;

    QString ilkTStr = m_list.at(0);
    ilkT = ilkTStr.remove("-").toInt();

    QString ilkKStr = m_list.at(1);
    ilkK = ilkKStr.remove("-").toInt();

    QString ikinciTStr = m_list.at(2);
    ikinciT = ikinciTStr.remove("-").toInt();



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


    for(int i = 3; i < m_list.size(); i++){
        QString str = m_list.at(i);
        int val = str.remove("-").toInt();


        qDebug() << i << "  " << val << "  " << m_arizaType << "   " << m_nextTemizFasit;

        ui->messageTextEdit->append("___________" + QString::number(i) + "_________");
        ui->messageTextEdit->append("sahih Temiz gün sayısı: " + QString::number(m_normalT));
        ui->messageTextEdit->append("sahih Kirli gün sayısı: " + QString::number(m_normalK));

        ///kirli
        if(!isProcessingT){

            qDebug() <<  "kirli: " << QString::number(val);
            qDebug() <<  "m_normalK: " << QString::number(m_normalK);
            qDebug() <<  "m_normalT: " << QString::number(m_normalT);

            m_kirliler.push_back(val);
            m_kirlilerS.push_back(val);



            if(m_temizler.last() < 15 ){
                m_normalT = m_prevPrevTemiz;
            }

           if(m_arizaType == OZUR_KANI && val < 3){
               qDebug() << QString::number(i-3) + "'nci kirli " + QString::number(val) + " günü kaza et.";

           }else if(m_arizaType == DEVAMLI_KANAMA){
               int totalDonem = ilkT + ilkK;
               qDebug() << "devamli kanama";
               qDebug() << "m_normalT: " << m_normalT;
               qDebug() << "m_normalK: " << m_normalK;
               qDebug() << " HÜKMÜ m_prevTemiz: " << m_prevTemiz;
               qDebug() << " HÜKMÜ m_prevPrevTemiz: " << m_prevPrevTemiz;

             //  qDebug() << "m_normalK: " << m_normalK;

               ui->messageTextEdit->append("sahih Temiz gün sayısı: " + QString::number(m_normalT));
               ui->messageTextEdit->append("sahih Kirli gün sayısı: " + QString::number(m_normalK));


               ui->messageTextEdit->append("HÜKMÜ İSTİMRAR.");


               int kirliCnt = 0;
               if( m_temizler.last() < 15)
                  kirliCnt = m_kirliler.last() + m_kirliler.at(m_kirliler.size()-2) + m_temizler.last();
               else
                   kirliCnt = m_kirliler.last();

               qDebug() << "kirliCnt: " << kirliCnt;

               ///22-8-20-2-3-9
               if( m_normalT >= m_prevTemiz
                       && kirliCnt - (m_normalT - m_prevTemiz) >= m_normalK){
                   solveA();
               }else if(2 < m_normalK - (m_prevTemiz - m_normalT)
                        || (m_normalK > kirliCnt - (m_prevTemiz -m_normalT) && kirliCnt - (m_prevTemiz -m_normalT) > 2)){
                   solveB();
               }else if(2 >= m_normalK - (m_prevTemiz - m_normalT)
                        || kirliCnt - (m_prevTemiz -m_normalT) < 3){
                   solveC();
               }




           }

            if(val < 3){
                qDebug() << QString::number(i-1) + "'nci kirli gün sıkıntılı: " + QString::number(val);
                m_temizlerS.pop_back();
                m_prevTemiz = m_temizlerS.last();
                //m_normalT = ilkT;
                m_prevOzurKGun = val;

                if(!m_nextTemizFasit){
                    m_normalT = m_prevPrevTemiz;
                }else{
                    qDebug() << "----m_nextTemizFasit: " << m_nextTemizFasit;

                   // m_nextTemizFasit = false;
                }

///*************   25-5-20-6-18-13-22-8-8-4

                m_arizaType = OZUR_KANI;
                qDebug() << "ariza: OZUR_KANI";



            }

            ///           19-5-18-6-17-2-16-12     25-5-20-6-19-13
            else if(val > 10){

                m_temizlerS.pop_back();
                m_prevTemiz = m_temizlerS.last();


                qDebug() << QString::number(i-1) + "'nci kirli gün sıkıntılı: " + QString::number(val);
                m_prevOzurKGun = val;

                if(m_ozurKaniTemizDevam){
                    QString sttr = m_list.at(m_prevTemizIdx-2);


                    if(!m_nextTemizFasit){
                        m_normalT = sttr.toInt();
                    }else{
                       // m_nextTemizFasit = false;
                    }


                    qDebug() << "m_ozurKaniTemizDevam: " << m_ozurKaniTemizDevam;
                    m_ozurKaniTemizDevam = false;
                }else{  //normal kirli
                    if(!m_nextTemizFasit){
                        m_normalT = m_prevPrevTemiz;
                    }else{
                        m_nextTemizFasit = false;
                    }

                }

                m_arizaType = DEVAMLI_KANAMA;
                qDebug() << "ariza:, OZUR_KANI";
                int totalDonem = ilkT + ilkK;
                qDebug() << "devamli kanama";
                qDebug() << "m_normalT: " << m_normalT;
                qDebug() << "m_normalK: " << m_normalK;
                qDebug() << "m_prevTemiz: " << m_prevTemiz;
                qDebug() << "m_kirliler.last(): " << m_kirliler.last();

                ui->messageTextEdit->append("sahih Temiz gün sayısı: " + QString::number(m_normalT));
                ui->messageTextEdit->append("sahih Kirli gün sayısı: " + QString::number(m_normalK));

              //  qDebug() << "m_normalK: " << m_normalK;

                ///   19-5-18-6-17-2-16-12
                ///



                ui->messageTextEdit->append("HAKİKİ İSTİMRAR.");
                if( m_normalT >= m_prevTemiz
                        && m_kirliler.last() - (m_normalT - m_prevTemiz) >= m_normalK){
                    solveA();
                }else if(2 < m_normalK - (m_prevTemiz - m_normalT)
                         || (m_normalK > m_kirliler.last() - (m_prevTemiz -m_normalT) && m_kirliler.last() - (m_prevTemiz -m_normalT) > 2)){
                    solveB();
                }else if(2 >= m_normalK - (m_prevTemiz - m_normalT)
                         || m_kirliler.last() - (m_prevTemiz -m_normalT) < 3){
                    solveC();
                }

            }

            else{

                if(!m_nextTemizFasit){
                    m_normalT = m_prevPrevTemiz;
                }else{
                   // m_nextTemizFasit = false;
                }

                if( val > 2 && val < 11 && m_temizler.last() > 15){
                    m_normalK = val;
                    qDebug() << "----:::::m_normalK: " << m_normalK;
                }

            }



            isProcessingT = true;
        }

        ///temiz
        else{
            qDebug() <<  "temiz: " << QString::number(val);
            qDebug() <<  "m_normalK: " << QString::number(m_normalK);
            qDebug() <<  "m_normalT: " << QString::number(m_normalT);

            qDebug() <<  "m_arizaType: " << QString::number(m_arizaType);

            m_temizler.push_back(val);
            m_temizlerS.push_back(val);


///   25-5-20-6-22-4-13-4

            if(m_arizaType == NU_LL){

                if(val < 15){
                    m_arizaType = DEVAMLI_KANAMA;

                    m_temizlerS.pop_back();

                    qDebug() << "kirli..." << val << "  " << m_prevOzur;

                     if( m_kirliler.at(m_kirliler.size()-2) > 2 && m_kirliler.at(m_kirliler.size()-2) < 11){
                        m_normalK = m_kirliler.at(m_kirliler.size()-2);
                        qDebug() << "----:::::--m_normalK: " << m_normalK;
                     }else{

                         if( m_kirliler.at(m_kirliler.size()-3) > 2 && m_kirliler.at(m_kirliler.size()-3) < 11){
                            m_normalK = m_kirliler.at(m_kirliler.size()-3);
                            qDebug() << "----:::::--m_normalK: " << m_normalK;
                         }
                     }

                    m_prevOzurKGun = val;
                    m_normalT = m_prevPrevTemiz;

                    qDebug() << "normalT..." << m_normalT ;

                    if( i > 5){
                        QString sttr =  m_list.at(i-4);
                        m_prevPrevTemiz = sttr.toInt();

                    }


                    qDebug() << "m_prevPrevTemiz..." << m_prevPrevTemiz ;

                }else{
                    m_ozurKaniTemizDevam = true;

                    if( i > 4){
                        QString sttr =  m_list.at(i-2);
                        m_prevPrevTemiz = sttr.toInt();

                    }


                    qDebug() << "t>15 and NULL m_nextTemizFasit  => " << m_nextTemizFasit;
                    if(m_nextTemizFasit){
                      // m_nextTemizFasit = false;
                    }else{
                        m_prevTemiz = val;
                        m_prevTemizIdx = i;

                    }

                }
            }

            ////           20-5-22-6-13-5

            if(m_arizaType == OZUR_KANI){
                if(val < 15){
                    m_prevDevamliKanamaTGun = val;
                    m_arizaType = DEVAMLI_KANAMA;
                    qDebug() << "ariza::: DEVAMLI_KANAMA";
                    m_ozurKaniTemizDevam = false;

                    if( i > 6){
                        QString sttr =  m_list.at(i-4);
                        m_prevPrevTemiz = sttr.toInt();

                    }

                }
                else{

                    if( i > 4){
                        QString sttr =  m_list.at(i-2);
                        m_prevPrevTemiz = sttr.toInt();

                    }
                    m_ozurKaniTemizDevam = true;
                    m_arizaType = OZUR_KANI;
                    qDebug() << "ariza::: OZUR_KANI";

                    int temizler = 0;
                    for(int a = 0; a < m_temizler.size()-1; a++){
                        qDebug() << "--temizler: " << QString::number(m_temizler.at(a));
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

                    if(m_kirliler.last() < 3){
                        QString msg = QString::number(m_kirliler.size()+1) + ". kirli\n" + QString::number(m_kirliler.last()) + " günü kaza et.\n";
                        msg = msg + "\n\n" + dates;

                        ui->messageTextEdit->append(msg);
                        QMessageBox msgBox;
                        msgBox.setText(msg);
                        msgBox.exec();
                    }

                }
            }

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
    ui->messageTextEdit->append("A  sahih Temiz gün sayısı: " + QString::number(m_normalT));
    ui->messageTextEdit->append("A  sahih Kirli gün sayısı: " + QString::number(m_normalK));

   ui->messageTextEdit->append("\n\nA ŞIKKI.\n");



   int kirliCnt = 0;
   if( m_temizler.last() < 15)
      kirliCnt = m_kirliler.last() + m_kirliler.at(m_kirliler.size()-2)  + m_temizler.last();
   else
       kirliCnt = m_kirliler.last();


   qDebug() << "kirliCnt: " << kirliCnt;

   int ilkKazaSayisi = m_normalT - m_prevTemiz;

   int ilkKazaGecenGun = 0;
   for(int i = 0; i < m_prevTemizIdx+1; i++){
       QString str = m_list.at(i);
       ilkKazaGecenGun += str.remove("-").toInt();
   }

   QString dates = "";

   for(int i = 0; i < ilkKazaSayisi; i++){
       QDate dt = ui->startDateEdit->date().addDays(ilkKazaGecenGun+i);
        dates.append(dt.toString()+"\n");
   }

   QString msg = "Şu " + QString::number(ilkKazaSayisi) + " gün kaza edilecek:\n\n";


   ui->messageTextEdit->append("\n\n" + msg + dates + "\n");



   int sonradanKilinacak = kirliCnt - (m_normalT - m_prevTemiz) - m_normalK;

   int diffSonKan = kirliCnt - sonradanKilinacak;

   int sonradanKilinacakGecenGun = ilkKazaGecenGun + diffSonKan;

   dates = "";
   for(int i = 0; i < sonradanKilinacak; i++){
       QDate dt = ui->startDateEdit->date().addDays(sonradanKilinacakGecenGun+i);
       dates.append(dt.toString()+"\n");
   }

   msg = "Şu " + QString::number(sonradanKilinacak) + " gün namaz kılınmalı:\n\n";
   ui->messageTextEdit->append("\n\n" + msg + dates + "\n");

  // m_prevTemiz = kirliCnt - sonradanKilinacak;
   if(kirliCnt <= 10 && kirliCnt > 2)
        m_normalK = kirliCnt;

   if(sonradanKilinacak > 0){
       m_nextTemizFasit = true;
       m_aSolved = true;

       qDebug() << "m_nextTemizFasit: " << m_nextTemizFasit;
    }else{
       m_nextTemizFasit = false;
       qDebug() << "valll: " << m_temizler.last();


   }

   /*if(ilkKazaSayisi > 0){
       m_nextTemizFasit = true;
       m_aSolved = true;

       qDebug() << "m_nextTemizFasit: " << m_nextTemizFasit;
    }

*/
   m_ozurKaniTemizDevam = false;
   m_arizaType = NU_LL;
}

///19-5-18-12-21-7-16-11

///25-5-15-15-21-7-16-11


void MainWindow::solveB()
{

    ui->messageTextEdit->append("B  sahih Temiz gün sayısı: " + QString::number(m_normalT));
    ui->messageTextEdit->append("B  sahih Kirli gün sayısı: " + QString::number(m_normalK));


    ui->messageTextEdit->append("\n\nB ŞIKKI.\n");








    int kirliCnt = 0;
    if( m_temizler.last() < 15)
       kirliCnt = m_kirliler.last() + m_kirliler.at(m_kirliler.size()-2)  + m_temizler.last();
    else
        kirliCnt = m_kirliler.last();


    qDebug() << "kirliCnt: " << kirliCnt;

    int ilkKazaSayisi = m_normalT - m_prevTemiz;

    int ilkKazaGecenGun = 0;
    for(int i = 0; i < m_prevTemizIdx+1; i++){
        QString str = m_list.at(i);
        ilkKazaGecenGun += str.remove("-").toInt();
    }

    QString dates = "";

    for(int i = 0; i < ilkKazaSayisi; i++){
        QDate dt = ui->startDateEdit->date().addDays(ilkKazaGecenGun+i);
         dates.append(dt.toString()+"\n");
    }


    QString msg = "Şu " + QString::number(ilkKazaSayisi) + " gün kaza edilecek:\n\n";


    if(ilkKazaSayisi > 0)
        ui->messageTextEdit->append("\n\n" + msg + dates + "\n");
    else
        ilkKazaSayisi = 0;


    m_normalK = m_normalK - abs(m_normalT - m_prevTemiz);
    ui->messageTextEdit->append("B  sahih Kirli gün sayısı değişti: " + QString::number(m_normalK));


    if(m_prevTemiz > m_normalT){

        m_normalT = m_prevTemiz;
        ui->messageTextEdit->append("B  sahih Temiz gün sayısı değişti: " + QString::number(m_normalT));


    }

    int sonradanKilinacak = kirliCnt - ilkKazaSayisi - m_normalK;
    qDebug() << ":m_normalK: " << m_normalK;
    qDebug() << ":sonradanKilinacak: " << sonradanKilinacak;
    qDebug() << ":kirliCnt: " << kirliCnt;
    qDebug() << ":ilkKazaSayisi: " << ilkKazaSayisi;

    int diffSonKan = kirliCnt - sonradanKilinacak;

    int sonradanKilinacakGecenGun = ilkKazaGecenGun + diffSonKan;

    dates = "";
    for(int i = 0; i < sonradanKilinacak; i++){
        QDate dt = ui->startDateEdit->date().addDays(sonradanKilinacakGecenGun+i);
        dates.append(dt.toString()+"\n");
    }

    //20-5-22-6-24-13


    qDebug() << ":kirliCnt: " << kirliCnt;
    qDebug() << ":sonradanKilinacak: " << sonradanKilinacak;
   // qDebug() << ":kirliCnt: " << kirliCnt;

    msg = "Şu " + QString::number(sonradanKilinacak) + " gün namaz kılınmalı:\n\n";
    ui->messageTextEdit->append("\n\n" + msg + dates + "\n");

   // m_prevTemiz = kirliCnt - sonradanKilinacak;
    if(kirliCnt <= 10)
         m_normalK = kirliCnt;

    if(sonradanKilinacak > 0){
        m_nextTemizFasit = true;
        m_aSolved = true;

        qDebug() << "m_nextTemizFasit: " << m_nextTemizFasit;
     }else{
        m_nextTemizFasit = false;
        qDebug() << "valll: " << m_temizler.last();


    }

    /*if(ilkKazaSayisi > 0){
        m_nextTemizFasit = true;
        m_aSolved = true;

        qDebug() << "m_nextTemizFasit: " << m_nextTemizFasit;
     }

 */
    m_ozurKaniTemizDevam = false;
    m_arizaType = NU_LL;

}






void MainWindow::solveC()
{

    ui->messageTextEdit->append("C  sahih Temiz gün sayısı: " + QString::number(m_normalT));
    ui->messageTextEdit->append("C  sahih Kirli gün sayısı: " + QString::number(m_normalK));

    ui->messageTextEdit->append("\n\nC ŞIKKI.\n");

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
