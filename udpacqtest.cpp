#include "udpacqtest.h"
#include "ui_udpacqtest.h"
#include "udpacq.hpp"
#include <QtDebug>
// I don't think the line above is necessary given that I called this from main.

using namespace DeviceLib;

UdpAcqTest::UdpAcqTest(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UdpAcqTest)
{
    ui->setupUi(this);
    // localhost:8080
    UdpAcqDevice * test = new UdpAcqDevice("localhost:8080");

    qDebug() << "hi";

    delete test;
}

UdpAcqTest::~UdpAcqTest()
{
    delete ui;
}


//const QString& Setup::port(const QString& key)
//{ return ports_.value(key); }
//void Setup::setPortName(const QString& key, const QString& value, bool bSaveToFile)
//{
//   ports_.change(key, value);
//   if (bSaveToFile)
//      writeDefaults();
//}
