#include "friendlistitem.h"
#include "ui_friendlistitem.h"

FriendListItem::FriendListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendListItem)
{
    ui->setupUi(this);
    QHBoxLayout * hLayoutWidget = new QHBoxLayout(this);//整体itemWidget布局
    lb_image = new QLabel(tr("好友头像"),this);
    lb_image->setObjectName(tr("lb_image"));
    lb_friendName = new QLabel(tr("好友名称"), this);
    lb_friendName->setObjectName(tr("lb_friendName"));
    hLayoutWidget->addWidget(lb_image);
    hLayoutWidget->addWidget(lb_friendName);
    hLayoutWidget->setStretch(0, 1);
    hLayoutWidget->setStretch(1, 10);
}

void FriendListItem::setPicture(QString path){
    QPixmap pix(80,80);
    pix.load(path);
    pix.scaled(QSize(80, 80));
    lb_image->clear();
    lb_image->setPixmap(pix);
}

void FriendListItem::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    isMousePressed = true;
}

void FriendListItem::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if(isMousePressed)
    {
        emit isItemClicked(getFriendName());
        isMousePressed = false;
    }
}

FriendListItem::~FriendListItem()
{
    delete ui;
}
