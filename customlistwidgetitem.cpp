#include "customlistwidgetitem.h"
#include "ui_customlistwidgetitem.h"

CustomListWidgetItem::CustomListWidgetItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomListWidgetItem)
{
    ui->setupUi(this);

    QHBoxLayout * hLayoutWidget = new QHBoxLayout(this);//整体itemWidget布局
    lb_image = new QLabel(tr("好友头像"),this);
    lb_image->setObjectName(tr("lb_image"));
    QVBoxLayout * vLayoutInfo = new QVBoxLayout(this);
    QHBoxLayout * hLayoutInfoTop = new QHBoxLayout(this);
    lb_messageNumber = new QLabel(tr("n"),this);
    lb_messageNumber->setObjectName(tr("lb_messageNumber"));
    lb_friendName = new QLabel("root",this);
    lb_friendName->setObjectName(tr("lb_frinedName"));
    lb_lastTime = new QLabel(tr("lastTime"),this);
    lb_lastTime->setObjectName(tr("lb_lastTime"));
    hLayoutInfoTop->addWidget(lb_messageNumber);    //设置第一行label控件
    hLayoutInfoTop->addWidget(lb_friendName);
    hLayoutInfoTop->addWidget(lb_lastTime);
    hLayoutInfoTop->setStretch(0,2);                //设置第一行label控件伸缩比例
    hLayoutInfoTop->setStretch(1,20);
    hLayoutInfoTop->setStretch(2,5);
    QHBoxLayout * hLayoutInfoButtom = new QHBoxLayout(this);
    //QSpacerItem * hSpacer = new QSpacerItem(40,20,QSizePolicy::Preferred);
    lb_lastMessage = new QLabel(tr("last message"),this);
    lb_lastMessage->setObjectName(tr("lb_lastMessage"));
    lb_voiceIcon = new QLabel(this);
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
    this->setLayout(hLayoutWidget);           //设置itemWidget整体布局
}

void CustomListWidgetItem::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    isMousePressed = true;
}

void CustomListWidgetItem::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if(isMousePressed)
    {
        emit isItemClicked(getFriendName());
        isMousePressed = false;
    }
}

CustomListWidgetItem::~CustomListWidgetItem()
{
    delete ui;
}
