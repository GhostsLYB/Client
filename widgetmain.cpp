#include "widgetmain.h"
#include "ui_widgetmain.h"
#include <QDebug>

WidgetMain::WidgetMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMain)
{
    ui->setupUi(this);

    listWidget = new QListWidget(this);
    this->layout()->addWidget(listWidget);

    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //自定义QListWidgetItem
    //应该查找数据库中的最近聊天信息表，将所有最近聊天的好友添加到最近聊天页面！！！！！！！！！！！！！！！
    QList<QString> nameList;
    nameList << "root" << "ghost";
    for(int i = 0; i < 2; i++)
    {
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(0,120));
        CustomListWidgetItem * itemWidget = new CustomListWidgetItem(listWidget);   //自定义QListWidgetItem
        itemWidget->setIndex(i);
        //设置头像图片，名称，最后时间，最后消息，是否免打扰（需要完善）
        itemWidget->setFriendName(nameList[i]);
        listWidget->setItemWidget(item,itemWidget); //为listWidget添加自定义item
        connect(itemWidget,&CustomListWidgetItem::isItemClicked,this,&WidgetMain::onItemClicked);
    }


}

//有新的聊天记录时的槽函数
//参数 image:头像图片路径;msgNum:最新的消息数量;friendName：好友名；msg:最后的消息
/*void WidgetMain::onAddFriendItem(QString image, int msgNum,QString friendName,QString msg)
{
    QListWidgetItem * item = new QListWidgetItem(listWidget);
    QWidget * itemWidget = new QWidget(listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(QSize(0,100));
    itemWidget = new QWidget(listWidget); Q_UNUSED(image) //自定义QWidget itemWidget 包含一个label和一个垂直布局
    QHBoxLayout * hLayoutWidget = new QHBoxLayout(itemWidget);//整体itemWidget布局
    QLabel * lb_image = new QLabel(tr("好友头像"),itemWidget);
    lb_image->setObjectName(tr("lb_image"));    //将image图片显示到lb_image控件中
    QVBoxLayout * vLayoutInfo = new QVBoxLayout(itemWidget);

    QHBoxLayout * hLayoutInfoTop = new QHBoxLayout(itemWidget);
    QLabel * lb_messageNumber = new QLabel(QString::number(msgNum),itemWidget);
    lb_messageNumber->setObjectName(tr("lb_messageNumber"));
    QLabel * lb_friendName = new QLabel(friendName,itemWidget);
    lb_friendName->setObjectName(tr("lb_frinedName"));
    QLabel * lb_lastTime = new QLabel(tr("lastTime"),itemWidget);
    lb_lastTime->setObjectName(tr("lb_lastTime"));
    hLayoutInfoTop->addWidget(lb_messageNumber);    //设置第一行label控件
    hLayoutInfoTop->addWidget(lb_friendName);
    hLayoutInfoTop->addWidget(lb_lastTime);
    hLayoutInfoTop->setStretch(0,2);                //设置第一行label控件伸缩比例
    hLayoutInfoTop->setStretch(1,20);
    hLayoutInfoTop->setStretch(2,5);
    QHBoxLayout * hLayoutInfoButtom = new QHBoxLayout(itemWidget);
    //QSpacerItem * hSpacer = new QSpacerItem(40,20,QSizePolicy::Preferred);
    QLabel *lb_lastMessage = new QLabel(msg,itemWidget);
    lb_lastMessage->setObjectName(tr("lb_lastMessage"));
    QLabel *lb_voiceIcon = new QLabel(itemWidget);
    lb_voiceIcon->setObjectName(tr("lb_voiceIcon"));
    hLayoutInfoButtom->addStretch(2);               //设置第二行label控件
    hLayoutInfoButtom->addWidget(lb_lastMessage);
    hLayoutInfoButtom->addWidget(lb_voiceIcon);
    hLayoutInfoButtom->setStretch(1,20);            //设置第二行label控件伸缩比例
    hLayoutInfoButtom->setStretch(2,5);
    vLayoutInfo->addLayout(hLayoutInfoTop);         //将两行信息组合
    vLayoutInfo->addLayout(hLayoutInfoButtom);
    hLayoutWidget->addWidget(lb_image);             //将头像控件和信息两部分组合
    hLayoutWidget->addLayout(vLayoutInfo);
    itemWidget->setLayout(hLayoutWidget);           //设置itemWidget整体布局
    listWidget->setItemWidget(item,itemWidget); //为listWidget添加自定义item
}*/

void WidgetMain::onItemClicked(QString friendName)
{
    emit sigItemClicked(friendName);
}

WidgetMain::~WidgetMain()
{
    delete ui;
}
