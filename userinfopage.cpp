#include "userinfopage.h"
#include "ui_userinfopage.h"

UserInfoPage::UserInfoPage(Control *ctrl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoPage),
    ctrl(ctrl)
{
    ui->setupUi(this);
/*
//    QVBoxLayout *mainLayout = new QVBoxLayout(this);//整体垂直布局 包含两个布局
//    QHBoxLayout *baseInfoLayout = new QHBoxLayout(this);//基本信息布局 包含一个label和一个垂直布局
//    lb_image = new QLabel("image", this);
//    QVBoxLayout *infoLayout = new QVBoxLayout(this);//信息布局
//    lb_remark = new QLabel("remark", this);
//    lb_userName = new QLabel("userName", this);
//    lb_number = new QLabel("number", this);
//    lb_address = new QLabel("address", this);
//    infoLayout->addWidget(lb_remark);
//    infoLayout->addWidget(lb_userName);
//    infoLayout->addWidget(lb_number);
//    infoLayout->addWidget(lb_address);

//    baseInfoLayout->addWidget(lb_image);
//    baseInfoLayout->addLayout(infoLayout);
//    baseInfoLayout->setStretch(0, 1);
//    baseInfoLayout->setStretch(1, 4);
//    QVBoxLayout *operationLayout = new QVBoxLayout(this);//电话，朋友圈，按钮垂直布局
//    lb_phone = new QLabel("phone:", this);
//    QLabel *lb_dynamic = new QLabel("dynamic", this);
//    wid_friendDynamic = new QWidget(this);//包含四个好友动态图片（待完善）
//    QHBoxLayout *wid_frinedDynamicLayout = new QHBoxLayout(this);
//    wid_frinedDynamicLayout->addWidget(lb_dynamic);
//    wid_frinedDynamicLayout->addWidget(wid_friendDynamic);
//    wid_frinedDynamicLayout->setStretch(0, 1);
//    wid_frinedDynamicLayout->setStretch(1, 4);
//    wid_friendDynamic->setLayout(wid_frinedDynamicLayout);
//    btn_sendMsg = new QToolButton(this);
//    btn_sendMsg->setText("sendMsg");
//    operationLayout->addWidget(lb_phone);
//    operationLayout->addWidget(wid_friendDynamic);
//    operationLayout->addWidget(btn_sendMsg);

//    mainLayout->addLayout(baseInfoLayout);
//    mainLayout->addLayout(operationLayout);
*/
    btn_cancle          = ui->btn_cancle;
    lb_image            = ui->lb_image;
    lb_remark           = ui->lb_remark;
    lb_userName         = ui->lb_userName;
    lb_number           = ui->lb_number;
    lb_address          = ui->lb_address;
    lb_phone            = ui->lb_phone;
    lb_dynamic1         = ui->lb_dynamic1;
    lb_dynamic2         = ui->lb_dynamic2;
    lb_dynamic3         = ui->lb_dynamic3;
    lb_dynamic4         = ui->lb_dynamic4;
    btn_sendMsg         = ui->btn_sendMsg;
    btn_deleteFriend    = ui->btn_deleteFriend;
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
    lb_userName->setText("userName: "+userName);
    lb_remark->setText(userName);
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
