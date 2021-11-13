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
   blo->addWidget(send);
   QPushButton *ex = new QPushButton(tr("Exit"), this);
   blo->addWidget(ex);
   blo->addStretch();
   mlo->addLayout(blo);
   connect(send, SIGNAL(clicked()), this, SLOT(processSend()));
   connect(ex, SIGNAL(clicked()), this, SLOT(accept()));


//   qDebug() << ports_->currentData();
}
//UdpAcqCmdDialog::~UdpAcqCmdDialog() {}
mainWindow::~mainWindow() {}

//UdpAcqDevice& udpDevice(int n, QComboBox* ports)
//{
//   assert(static_cast<size_t>(n) < sizeof(ports));
//   if (ports->currentIndex() == 0)
//   {
//      QString key = QString("udp") + QString::number(n);
//      UdpAcqDevice ret = new UdpAcqDevice(key);

//      // update current index
//      ports.currentIndex() = new UdpAcqDevice(port(key));
//   }
//   return ret;
//}

void mainWindow::processSend()
{
   using namespace DeviceLib;

   int n = ports_->currentIndex();
   if (n == -1) n = 0;
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
// so now let's try and run the python stuff and figure out exactly how this communication is supposed to work


