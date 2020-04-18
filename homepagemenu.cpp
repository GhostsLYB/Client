#include "homepagemenu.h"
#include "ui_homepagemenu.h"

HomePageMenu::HomePageMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePageMenu)
{
    ui->setupUi(this);

    ui->btn_mainpage->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->btn_friendpage->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->btn_findpage->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->btn_minepage->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    connect(ui->btn_mainpage,&QToolButton::clicked,this,&HomePageMenu::onPage1);
    connect(ui->btn_friendpage,&QToolButton::clicked,this,&HomePageMenu::onPage2);
    connect(ui->btn_findpage,&QToolButton::clicked,this,&HomePageMenu::onPage3);
    connect(ui->btn_minepage,&QToolButton::clicked,this,&HomePageMenu::onPage4);

}

HomePageMenu::~HomePageMenu()
{
    delete ui;
}
