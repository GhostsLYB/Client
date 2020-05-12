#include "login.h"
#include "ui_login.h"
#include <string.h>

Login::Login(Control * parentCtrl, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    ctrl = parentCtrl;
    connect(ctrl,&Control::sigLoginResponse,this,&Login::onLoginResponse);
    //Register页面根据socket连接状态不同改变显示信息
    connect(ctrl->sock->socket(),&QTcpSocket::connected,this,&Login::socketConnected);
    connect(ctrl->sock->socket(),&QTcpSocket::disconnected,this,&Login::socketDisconnected);

    connect(ui->btn_login, &QPushButton::clicked, this, &Login::btn_loginClicked);
    connect(ui->btn_register, &QPushButton::clicked, this, &Login::btn_registerClicked);
    connect(ui->btn_forgetpwd, &QPushButton::clicked,this, &Login::btn_forgetPwdClicked);
    connect(ui->cb_rememberpwd,&QCheckBox::clicked, this, &Login::cb_rememberPwdClicked);

}

void Login::btn_loginClicked()
{
    qDebug() << "clicked btn_login";
    QString msg;
    if(!checkInformation(msg))
    {
        msg = "";
        return;
    }
    qDebug() << "login page msg = " << msg;
    if(SocketControl::isconnected())
        SocketControl::socket()->write(msg.toUtf8());
    else
        ui->lb_socketState->setText("Cannot connect to service!");

}

void Login::btn_registerClicked()
{
    qDebug() << "clicked btn_register";
    emit sigRegisterClicked();
}

void Login::btn_forgetPwdClicked()
{
    qDebug() << "clicked btn_forgetpwd";
}

void Login::cb_rememberPwdClicked()
{
    if(ui->cb_rememberpwd->isChecked())
    {
        qDebug() << "choose remember password";
    }
    else
    {
        qDebug() << "choose not remember password";
    }
}

void Login::socketConnected(){
    ui->lb_socketState->setText("connect service successed!");
}

void Login::socketDisconnected(){
    ui->lb_socketState->setText("Cannot connect service!");
}

bool Login::checkInformation(QString &msg){
    char temp[1024] = {0};
    sprintf(temp, "%4d", 2);
    msg.append(temp);
    if(ui->le_username->text().isEmpty()){
        ui->lb_socketState->setText("User name cannot be empty!");
        msg = "";
        return false;
    }
    else {
        sprintf(temp, "%4d", qstrlen(ui->le_username->text().toUtf8().data()));
        msg.append(temp);
        msg.append(ui->le_username->text());
    }
    if(ui->le_passward->text().isEmpty()){
        ui->lb_socketState->setText("Password cannot be empty!");
        msg = "";
        return false;
    }
    else {
        sprintf(temp, "%4d", qstrlen(ui->le_passward->text().toUtf8().data()));
        msg.append(temp);
        msg.append(ui->le_passward->text());
    }
    sprintf(temp, "%4d", qstrlen(msg.toUtf8().data()));
    QString s = msg;
    msg = "";
    msg.append(temp);
    msg.append(s);
    qDebug() << msg;
    return true;
}

void Login::onLoginResponse(QString msg)
{
    qDebug() << "msg = " << msg;
    if(msg.compare("yes") == 0)
    {
        emit sigLoginSuccessed(ui->le_username->text());
    }
    else if(msg.compare("no") == 0){
        msg = "Incorrect password!";
    }
    else if(msg.compare("null") == 0)
        msg = "User "+ui->le_username->text()+" not exist!";
    else if(msg.compare("online") == 0)
        msg = "User "+ui->le_username->text()+" is online";
    else {
        msg = "unknow";
    }
    ui->lb_socketState->setText(msg);
}

Login::~Login()
{
    delete ui;
}
