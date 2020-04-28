#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QString>
#include "socket/control.h"

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(Control * ctrl = nullptr, QWidget *parent = nullptr);
    ~Login();
private:
    bool checkInformation(QString &);
signals:
    void sigRegisterClicked();
    void sigLoginSuccessed(QString);
private slots:
    void btn_loginClicked();
    void btn_registerClicked();
    void btn_forgetPwdClicked();
    void cb_rememberPwdClicked();
    void socketConnected();
    void socketDisconnected();
    void onLoginResponse(QString);
private:
    Ui::Login *ui;
    Control * ctrl = nullptr;
};

#endif // LOGIN_H
