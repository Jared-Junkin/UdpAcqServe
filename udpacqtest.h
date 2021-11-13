#ifndef UDPACQTEST_H
#define UDPACQTEST_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UdpAcqTest; }
QT_END_NAMESPACE

class UdpAcqTest : public QMainWindow
{
    Q_OBJECT

public:
    UdpAcqTest(QWidget *parent = nullptr);
    ~UdpAcqTest();

private:
    Ui::UdpAcqTest *ui;
};
#endif // UDPACQTEST_H
