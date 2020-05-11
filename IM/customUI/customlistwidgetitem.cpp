#include "customlistwidgetitem.h"
#include "ui_customlistwidgetitem.h"

CustomListWidgetItem::CustomListWidgetItem(QWidget *parent,QString image,QString newMsgNum,
                                           QString friendName,QString lastTime,
                                           QString lastMsg,QString voiceIcon) :
    QWidget(parent),
    ui(new Ui::CustomListWidgetItem)
{
    ui->setupUi(this);

    lb_image = ui->lb_image;
    lb_newMsgNum = ui->lb_newMsgNum;
    lb_friendName = ui->lb_friendName;
    lb_lastTime = ui->lb_lastTime;
    lb_lastMsg = ui->lb_lastMsg;
    lb_voiceIcon = ui->lb_voice;
    if(image != ""){
        QPixmap pix(image);
        lb_image->setPixmap(pix);
    }
    if(newMsgNum != ""){
        lb_newMsgNum->setText(newMsgNum);
    }
    if(friendName != ""){
        lb_friendName->setText(friendName);
    }
    if(lastTime != ""){
        lb_lastTime->setText(lastTime.mid(lastTime.indexOf(' ')+1, 5));
    }
    if(lastMsg != ""){
        lb_lastMsg->setText(lastMsg);
    }
    if(voiceIcon != ""){
        QPixmap pix(voiceIcon);
        lb_voiceIcon->setPixmap(pix);
    }
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
