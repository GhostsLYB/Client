#include "widgetmine.h"
#include "ui_widgetmine.h"

WidgetMine::WidgetMine(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMine)
{
    ui->setupUi(this);
    lb_image        = ui->lb_image;
    lb_userName     = ui->lb_userName;
    lb_number       = ui->lb_number;
    btn_info        = ui->btn_info;
    btn_modifyInfo  = ui->btn_modifyInfo;
    btn_setup       = ui->btn_setup;
}

void WidgetMine::setUserName(QString userName)
{
    ui->lb_userName->setText(userName);
}

WidgetMine::~WidgetMine()
{
    delete ui;
}
