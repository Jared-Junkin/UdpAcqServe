#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QUdpSocket>

//*---------------------------------------------------------------------------
//
// Class: UdpAcqCmdDialog : ModalDialog
//
// Window for sending UDP Acq commands
//
//----------------------------------------------------------------------------
class mainWindow : public QDialog
{
   Q_OBJECT
   typedef QDialog inherited;
public:
   mainWindow(QWidget *owner = nullptr);
   virtual ~mainWindow();


protected slots:
   void processSend();

private:
   QComboBox *ports_;
   QUdpSocket* udpsocket_;
   void receive();
   QLineEdit *cmd_;
   QLabel *resp_;

private:
   mainWindow(const mainWindow& cp);             // not implemented
   mainWindow& operator=(const mainWindow& rhs); // not implemented
};

#endif

