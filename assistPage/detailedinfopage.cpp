#include "detailedinfopage.h"
#include "ui_detailedinfopage.h"

DetailedInfoPage::DetailedInfoPage(Control *ctrl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailedInfoPage),
    ctrl(ctrl)
{
    ui->setupUi(this);
    tab_wdiget = ui->tabWidget;
    tab_wdiget->tabBar()->hide();

}

void DetailedInfoPage::setCurrentPage(int index)
{
    tab_wdiget->setCurrentIndex(index);
}

int DetailedInfoPage::getCurrentPage()
{
    return tab_wdiget->currentIndex();
}

DetailedInfoPage::~DetailedInfoPage()
{
    delete ui;
}

void DetailedInfoPage::on_btn_cancle_clicked()
{
    if(tab_wdiget->currentIndex() == 1){//
        ui->tabWidget->setCurrentIndex(0);
}
    else{                //返回到mine界面
        emit sigBackToMine();
    }
}
