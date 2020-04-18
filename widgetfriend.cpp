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

}

WidgetFriend::~WidgetFriend()
{
    delete ui;
}
