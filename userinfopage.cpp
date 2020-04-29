#include "userinfopage.h"
#include "ui_userinfopage.h"

UserInfoPage::UserInfoPage(Control *ctrl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoPage),
    ctrl(ctrl)
{
    ui->setupUi(this);
    btn_cancle          = ui->btn_cancle;
    lb_image            = ui->lb_image;
    lb_remark           = ui->lb_remark;
    lb_userName         = ui->lb_userName;
    lb_number           = ui->lb_number;
    lb_address          = ui->lb_address;
    lb_personalSignature= ui->lb_personalSignature;
    lb_phone            = ui->lb_phone;
    lb_dynamic1         = ui->lb_dynamic1;
    lb_dynamic2         = ui->lb_dynamic2;
    lb_dynamic3         = ui->lb_dynamic3;
    lb_dynamic4         = ui->lb_dynamic4;
    btn_sendMsg         = ui->btn_sendMsg;
    btn_deleteFriend    = ui->btn_deleteFriend;
    perUserName         = lb_userName->text();
    perNumber           = lb_number->text();
    perAddress          = lb_address->text();
}

UserInfoPage::~UserInfoPage()
{
    delete ui;
}

//用户信息界面返回按钮槽函数
void UserInfoPage::on_btn_cancle_clicked()
{
    if(backPage == 0){//返回到通讯录
            emit sigFriendList();
    }else {
            emit sigChatWith(lb_remark->text());
    }
}

void UserInfoPage::setUserName(QString userName){
    lb_userName->setText(perUserName+userName);
    lb_remark->setText(userName);
}

QString UserInfoPage::getUserName()
{
    return lb_userName->text();
}

void UserInfoPage::setUserInfo(QList<QString> &data)
{
//    data：userName number imagePath personalSignature address time
    QPixmap pix;
    if(data[2] == "")
        pix.load("://icon//2.PNG");
    else {
        pix.load(data[2]);
    }
    lb_image->setPixmap(pix);
    lb_userName->setText(perUserName+data[0]);
    lb_number->setText(perNumber+data[1]);
    lb_address->setText(perAddress+data[4]);
    lb_personalSignature->setText(data[3]);
}

//发送消息按钮槽函数
void UserInfoPage::on_btn_sendMsg_clicked()
{
    emit sigChatWith(lb_remark->text());
}

//删除好友槽函数
void UserInfoPage::on_btn_deleteFriend_clicked()
{
    qDebug() << "delete friend button is clicked";
}
