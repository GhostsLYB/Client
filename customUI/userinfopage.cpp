#include "userinfopage.h"
#include "ui_userinfopage.h"

UserInfoPage::UserInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoPage)
{
    ui->setupUi(this);
}

UserInfoPage::~UserInfoPage()
{
    delete ui;
}
