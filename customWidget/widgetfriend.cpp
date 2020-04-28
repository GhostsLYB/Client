#include "widgetfriend.h"
#include "ui_widgetfriend.h"

WidgetFriend::WidgetFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetFriend)
{
    ui->setupUi(this);

    listWidget = new QListWidget(this);
    this->layout()->addWidget(listWidget);

    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //读取数据库中的好友列表，将所有好友添加到通讯录列表中
    //提供好友头像路径和好友名构建一个好友项；
    QList<QString> nameList;
    nameList << "root" << "ghost";
    for (int i = 0; i < 2; i++) {
        QListWidgetItem * item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(0,120));
        FriendListItem * itemWidget = new FriendListItem(listWidget);   //自定义QListWidgetItem
        itemWidget->setIndex(i);
        //设置头像图片，名称（需要完善）
        itemWidget->setFriendName(nameList[i]);
        itemWidget->setPicture(":/icon/head_picture/2.png");
        listWidget->setItemWidget(item,itemWidget); //为listWidget添加自定义item
        connect(itemWidget,&FriendListItem::isItemClicked,this,&WidgetFriend::onItemClicked);
    }
}

void WidgetFriend::onItemClicked(QString friendName)
{
    qDebug()<<"Clicked item friend name:"<<friendName;
    emit sigItemClicked(friendName);
}

WidgetFriend::~WidgetFriend()
{
    delete ui;
}
