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
    btn_exitLogin  = ui->btn_modifyInfo;
    btn_setup       = ui->btn_setup;
    preNumber       = ui->lb_number->text();
    connect(btn_exitLogin,&QToolButton::clicked,this,&WidgetMine::onBtnExitLoginClicked);
}

void WidgetMine::setUserInfo(QList<QString> &data)
{
    lb_number->setText(preNumber + data[1]);
    QString imagePath = "";
    if(data[2] == "")
        imagePath = "://icon//2.PNG";
    else {
        imagePath = data[2];
    }
    QPixmap pix;
    pix.load(imagePath);
    pix.scaled(200,200);
    lb_image->setPixmap(pix);
}

void WidgetMine::setUserName(QString userName)
{
    ui->lb_userName->setText(userName);
}

QString WidgetMine::getUserName()
{
    return ui->lb_userName->text();
}

WidgetMine::~WidgetMine()
{
    delete ui;
}

void WidgetMine::on_btn_info_clicked()
{
    emit sigShowAllInfo(ui->lb_userName->text(), 0);
}

void WidgetMine::on_btn_setup_clicked()
{
    emit sigShowAllInfo(ui->lb_userName->text(), 2);
}

void WidgetMine::onBtnExitLoginClicked()
{
    emit sigExitLogin();
}
