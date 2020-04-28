#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QMessageBox>
#include "socket/control.h"

using namespace std;

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(Control * parentCtrl = nullptr, QWidget *parent = nullptr);
    ~Register();
private:
    //检查用户输入信息，无误返回true，否则显示提示信息并返回false
    bool checkInformation(QString & s);
    bool isReading = false;

private slots:
    void btn_cancelClicked();
    void btn_registerClicked();
    void onRegisterResponse(bool);

signals:
    void sigRegisterCancel();

private:
    Ui::Register *ui;
    Control * ctrl = nullptr;
};

#endif // REGISTER_H
