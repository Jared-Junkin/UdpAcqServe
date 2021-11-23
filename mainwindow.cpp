#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QThread>
#include <QPushButton>
#include <QUdpSocket>
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

   ports_ = new QComboBox(this);
   ports_->addItem("localhost:9999");
   ports_->addItem("localhost:9998");
   ports_->addItem("localhost:9997");
   ports_->addItem("localhost:9996");
   ports_->addItem("localhost:9995");

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

}
mainWindow::~mainWindow() {}

// this function creates a UdpAcqDevicec that can connect to host ("127.0.0.1", x). If one already exists, it acts as a getter and returns the UdpAcqDevice.
UdpAcqDevice &udpDevice(int n)
{
     static UdpAcqDevice * dev[10] = {0};  // singleton pattern
     if (dev[n] == 0){
         unsigned int x = 9999 - n;
         dev[n] = new UdpAcqDevice("127.0.0.1", x);
     }
     return *dev[n];
}

// this function takes the UdpAcqDevice associated with the host ports_->currentIndex(), and makes an Acqtester class which creates a new thread and then repeatedly calls cmd->text() in it
// the response is then displayed as resp_
void mainWindow::thread(){
    using namespace DeviceLib;
    if (cmd_->text().length() == 0){
        return;
    }
    try{

        UdpAcqDevice &acq = udpDevice(ports_->currentIndex());

        Acqtester *thread = new Acqtester(acq, 5, cmd_->text());
        thread->start();

        int i = 0;
        while(thread->text()=="" && i<100){
            QThread::msleep(1);
        }

        resp_->setText(thread->text());
    }
    catch (std::exception& ex){
        QMessageBox::warning(this, tr("UDP Command Exception"), tr(ex.what()));
    }
}

// this function sends a single command cmd_-> text() to the server and returns the response. It's single threaded and only calls sendCommand once. It also doesn't use the acqTester() class.
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

   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, tr("UDP Command Exception"), tr(ex.what()));
   }

}

