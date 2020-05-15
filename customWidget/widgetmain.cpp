#include "widgetmain.h"
#include "ui_widgetmain.h"
#include <QDebug>

#define RECENT_CHAT_ITEM_HEIGHT 120

WidgetMain::WidgetMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMain)
{
    ui->setupUi(this);

    listWidget = new QListWidget(this);
    this->layout()->addWidget(listWidget);

    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//有新的聊天记录时的槽函数
//参数 image:头像图片路径;msgNum:最新的消息数量;friendName：好友名；msg:最后的消息
void WidgetMain::onAddFriendItem(QString image,QString msgNum,QString friendName,
                                 QString lastMsg,QString lastTime,QString voice)
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    item->setSizeHint(QSize(0,RECENT_CHAT_ITEM_HEIGHT));
    //设置好友头像，名称，最后时间，最后消息，是否免打扰
    CustomListWidgetItem * itemWidget = new CustomListWidgetItem(listWidget,image,msgNum,friendName,
                                                                 lastTime,lastMsg,voice);   //自定义QListWidgetItem
    itemWidget->setIndex(getItemNum());
    listWidget->setItemWidget(item,itemWidget); //为listWidget添加自定义item
    connect(itemWidget,&CustomListWidgetItem::isItemClicked,this,&WidgetMain::onItemClicked);
}

void WidgetMain::onCleanAllItems()
{
    listWidget->clear();
}

void WidgetMain::onItemClicked(QString friendName)
{
    emit sigItemClicked(friendName);
}

WidgetMain::~WidgetMain()
{
    delete ui;
}
