#include "register.h"
#include "ui_register.h"


Register::Register(Control * parentCtrl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle("Register");
    //设置属性，当窗口关闭时，删除对象
    this->setAttribute(Qt::WA_DeleteOnClose);
    if(parentCtrl == nullptr)
        ctrl = new Control();
    else
        ctrl = parentCtrl;
    connect(ctrl,&Control::sigRegisterResponse,this,&Register::onRegisterResponse);
    //返回按钮
    connect(ui->btn_cancel,&QPushButton::clicked,this,&Register::btn_cancelClicked);
    //注册按钮
    connect(ui->btn_register,&QPushButton::clicked,this,&Register::btn_registerClicked);
}

void Register::btn_cancelClicked()
{
    qDebug() << "Register emit isClicked";
    emit sigRegisterCancel();
}

void Register::btn_registerClicked()
{
    QString msg;
    //核对信息
    if(!checkInformation(msg)){
        msg = "";
        return;
    }
    //提交到服务器核对
    //SocketControl sock(this);
    bool isSend = ctrl->sock->send(msg);
    if(!isSend)
        ui->lb_waring->setText("Cannot connect to service!");
}

bool Register::checkInformation(QString & msg)      //检查用户数据格式，在正确时将数据存储到msg中
{
    msg = "";
    QString temp;
    char buf[1024] = {0};
    char * pbuf = buf;
    temp = ui->le_username->text();
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("username is cannot be empty!");
        return false;
    }
    else {
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    temp = ui->le_pwd->text();
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("Password cannot be empty!");
        return false;
    }
    if(ui->le_checkPwd->text().compare(temp) != 0)
    {
        ui->lb_waring->setText("The two passwords are different!");
        return false;
    }
    else{
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    int index = ui->cb_sex->currentIndex();
    temp = QString::number(index);
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("sex cannot be empty!");
        return false;
    }
    else {
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    temp = ui->le_age->text();
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("age cannot be empty!");
        return false;
    }
    else {
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    temp = ui->le_email->text();
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("email cannot be empty!");
        return false;
    }
    else {
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    temp = ui->le_phone->text();
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("phone cannot be empty!");
        return false;
    }
    else {
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    temp = ui->le_id->text();
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("ID cannot be empty!");
        return false;
    }
    else {
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    temp = ui->le_secret->text();
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("secret cannot be empty!");
        return false;
    }
    else {
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    temp = ui->le_answer->text();
    if(temp.isEmpty())
    {
        ui->lb_waring->setText("answer cannot be empty!");
        return false;
    }
    else {
        sprintf(pbuf,"%4d", qstrlen(temp.toUtf8().data()));
        msg.append(pbuf);
        msg.append(temp);
    }
    msg.append('\n');
    sprintf(pbuf,"%4d",1);
    temp = msg;
    msg = "";
    msg.append(pbuf);msg.append(temp);
    uint len = qstrlen(msg.toUtf8().data());
    qDebug()<< msg;
    sprintf(pbuf,"%4d", len);
    temp = msg;
    msg = "";
    msg.append(pbuf);msg.append(temp);
    qDebug()<< msg;
    ui->lb_waring->setText("all information is right!");
    return true;
}

void Register::onRegisterResponse(bool isSuccessed)
{
    QString text;
    if(isSuccessed)
        text = "Register successed,back to login page!";
    else
        text = "Register failed,Please check informaiton!";
    QMessageBox::StandardButton ret = QMessageBox::information(this,"information", text);
    if(isSuccessed && ret == QMessageBox::Ok)
        emit sigRegisterCancel();
}

Register::~Register()
{
    delete ui;
}
