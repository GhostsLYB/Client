#include "userinfopage.h"
#include "ui_userinfopage.h"

UserInfoPage::UserInfoPage(Control *ctrl, SqliteControl *sqlite, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoPage),
    ctrl(ctrl),
    sqlite(sqlite)
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
    if(userName == GlobalDate::currentUserName())
    {
        btn_deleteFriend->hide();
    }       //是好友
    else if(sqlite->isFriend(GlobalDate::currentUserName(),userName)) {
        btn_deleteFriend->show();
        btn_deleteFriend->setText("删除好友");
    }
    else {
        btn_deleteFriend->show();
        btn_deleteFriend->setText("添加好友");
    }
}

QString UserInfoPage::getUserName()
{
    return lb_remark->text();
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
    if(btn_deleteFriend->text() == "删除好友"){
        qDebug() << "delete friend button is clicked";
        QString userName = GlobalDate::currentUserName();
        QString userName2 = lb_remark->text();
        sqlite->deleteFriend(userName,userName2);
        //发送删除好友请求到服务器 总长+类型10+名1长+名1+名2长+名2
        char ch[1024] = {0};
        sprintf(ch,"%4d%4d%4d%s%4d%s"
                ,12+qstrlen(userName.toUtf8().data())+qstrlen(userName2.toUtf8().data())
                ,10
                ,qstrlen(userName.toUtf8().data())
                ,userName.toUtf8().data()
                ,qstrlen(userName2.toUtf8().data())
                ,userName2.toUtf8().data());
        QString msg = QString(ch);
        qDebug() << msg;
        ctrl->sock->send(msg);
        emit sigDeleteFriend();
    }
    else{
        qDebug() << "add friend button is clicked";

    }

}
