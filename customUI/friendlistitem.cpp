#include "friendlistitem.h"
#include "ui_friendlistitem.h"

FriendListItem::FriendListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendListItem)
{
    ui->setupUi(this);
    lb_image = ui->lb_image;
    lb_friendName = ui->lb_friendName;
}

void FriendListItem::setPicture(QString path){
    QPixmap pix(80,80);
    pix.load(path);
    pix.scaled(QSize(100, 100));
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
