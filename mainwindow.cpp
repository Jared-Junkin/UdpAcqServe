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

}
mainWindow::~mainWindow() {}

void mainWindow::processSend()
{
   if (cmd_->text().length() == 0)
      return;
   if (udpsocket_){
       delete udpsocket_;
   }
   try
   {

    udpsocket_ = new QUdpSocket(this);
    // I don't think this line down below is necessary. That's because bind makes the object being binded (the socket) listen for incoming requests on a particular interface/port. That means it'sused for the server side
    // and since UDP is a send&pray protocol, this doesn't need to be done on the server side & never needs to be done on the client side.
//    udpsocket_->bind(QHostAddress::LocalHost, 9999);

    QByteArray cmd = cmd_->text().toUtf8();
    QString key = ports_->currentText();
    QStringList pip = key.split(":");


    // would need to lock here & then unlock once it's done reading.
    udpsocket_->writeDatagram(cmd, QHostAddress::LocalHost, pip[1].toInt());
//    udpsocket_->readDatagram()

    connect(udpsocket_, &QUdpSocket::readyRead, this, &mainWindow::receive);
    // this is an example of qt being really stupid. There's absolutely no way to call this function without making udpsocoket (the parameter) a class member
    // that's absolutely moronic.
    // there's no way to feed parameters to a slot... that's what I was talking about

   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, tr("UDP Command Exception"), tr(ex.what()));
   }
}

void mainWindow::receive(){
    while (udpsocket_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpsocket_->pendingDatagramSize());
        udpsocket_->readDatagram(datagram.data(), datagram.size());
        resp_->setText(datagram.data());
    }
}
