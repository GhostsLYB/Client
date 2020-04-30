#include "longbutton.h"
#include "ui_longbutton.h"

LongButton::LongButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LongButton)
{
    ui->setupUi(this);
    btn_name = ui->btn_name;
    btn_info = ui->btn_info;
    btn_name->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    btn_info->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->top_line->hide();
}

void LongButton::setBtnNameIcon(QString path)
{
    btn_name->setIcon(QIcon(path));
}

void LongButton::setBtnInfoIcon(QString path)
{
    btn_info->setIcon(QIcon(path));
}

void LongButton::setBtnNameIconSize(QSize size)
{
    btn_name->setIconSize(size);
}

void LongButton::setBtnInfoIconSize(QSize size)
{
    btn_info->setIconSize(size);
}

void LongButton::setLineShow(bool topShow, bool bottomShow)
{
    if(topShow)
        ui->top_line->show();
    else {
        ui->top_line->hide();
    }
    if(bottomShow)
        ui->bottom_line->show();
    else {
        ui->bottom_line->hide();
    }
}

void LongButton::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void LongButton::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    emit sigClicked(btn_name->text());
}

LongButton::~LongButton()
{
    delete ui;
}
