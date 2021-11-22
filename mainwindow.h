#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include "udpacq.hpp"

//*---------------------------------------------------------------------------
//
// Class: UdpAcqCmdDialog : ModalDialog
//
// Window for sending UDP Acq commands
//
//----------------------------------------------------------------------------
using namespace DeviceLib;

class mainWindow : public QDialog
{
   Q_OBJECT
   typedef QDialog inherited;
public:
   mainWindow(QWidget *owner = nullptr);
   virtual ~mainWindow();


protected slots:
   void processSend();
   void thread();

private:
   QComboBox *ports_;
   QLineEdit *cmd_;
   QLabel *resp_;

//   UdpAcqDevice *acq_;
private:
   mainWindow(const mainWindow& cp);             // not implemented
   mainWindow& operator=(const mainWindow& rhs); // not implemented
};

#endif

