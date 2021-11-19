//#include "mainwindow.h"

//mainWindow::mainWindow()
//{

//}

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QUdpSocket>
//#include <QPortSelector>
#include <QMessageBox>

#include "mainwindow.h"
#include "udpacq.hpp"
#include "acqtester.h"

using namespace DeviceLib;

mainWindow::mainWindow(QWidget *owner) :
   inherited(owner),
   ports_(0),
   cmd_(0),
   resp_(0)
{
   QVBoxLayout *mlo = new QVBoxLayout(this);
//   ports_= new DeviceLib::UdpPortSelector(this);
   // this was originally a custom class that allowed you to select ports. Will have to write my own method to determine which ports are available and add them to this combobox
   ports_ = new QComboBox(this);
   ports_->addItem("localhost:9999");
   ports_->addItem("localhost:9998");
   ports_->addItem("localhost:9997");
   ports_->addItem("localhost:9996");
   ports_->addItem("localhost:9995");




   //testing to see whether the frot end works...
   mlo->addWidget(ports_);
   QHBoxLayout *lo1 = new QHBoxLayout;
   QLabel *l = new QLabel(tr("Command:"), this);
   lo1->addWidget(l);
   cmd_ = new QLineEdit(this);
   lo1->addWidget(cmd_);
   mlo->addLayout(lo1);
   resp_ = new QLabel(this);
   mlo->addWidget(resp_);

   QHBoxLayout *blo = new QHBoxLayout;
   QPushButton *send = new QPushButton(tr("Send"), this);
   QPushButton *ex = new QPushButton(tr("Exit"), this);
   QPushButton *thread = new QPushButton(tr("Thread"), this);

   blo->addWidget(send);
   blo->addWidget(ex);
   blo->addWidget(thread);
   blo->addStretch();

   mlo->addLayout(blo);

   connect(send, SIGNAL(clicked()), this, SLOT(processSend()));
   connect(ex, SIGNAL(clicked()), this, SLOT(accept()));
   connect(thread, SIGNAL(clicked()), this, SLOT(thread()));
   // finished command connect w/ slot
   // ip_ or qhostaddress so you don't have QHostAddress::LocalHost anymoer
   // make udpsocket in the constructor, not when the function is called. That way you don't have to recreate and delete a thousand times.
   // fix so random numbers aren't fucking shit up


//   qDebug() << ports_->currentData();
}
//UdpAcqCmdDialog::~UdpAcqCmdDialog() {}
mainWindow::~mainWindow() {}

void mainWindow::thread(){
    using namespace DeviceLib;
    if (cmd_->text().length() == 0){
        return;
    }
    try{
        QString key = ports_->currentText();
        UdpAcqDevice ret = UdpAcqDevice(key);
        UdpAcqDevice& acq = ret;

        Acqtester *thread = new Acqtester(acq, 5, cmd_->text());
        thread->run();
        resp_->setText(thread->text());
    }
    catch (std::exception& ex){
        QMessageBox::warning(this, tr("UDP Command Exception"), tr(ex.what()));
    }
}

void mainWindow::processSend()
{
   using namespace DeviceLib;

   if (cmd_->text().length() == 0)
      return;
   try
   {
      QString key = ports_->currentText();
      UdpAcqDevice ret = UdpAcqDevice(key);
      UdpAcqDevice& acq = ret;

      ResourceLock lk(acq.access());
      resp_->setText(QString("send: ")+cmd_->text());
      QByteArray resp = acq.sendCommand(cmd_->text());
      resp_->setText(QString(resp));
      qDebug() << resp_->text();

   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, tr("UDP Command Exception"), tr(ex.what()));
   }

}

