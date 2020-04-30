#include "detailedinfopage.h"
#include "ui_detailedinfopage.h"

DetailedInfoPage::DetailedInfoPage(Control *ctrl, SqliteControl *sqlite, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailedInfoPage),
    ctrl(ctrl),
    sqlite(sqlite)
{
    ui->setupUi(this);
    tab_wdiget = ui->tabWidget;
    lb_tabTitle = ui->lb_tabTitle;
    btn_save = ui->btn_save;
    tab_wdiget->tabBar()->hide();
    //allInfo page index = 0
    QVBoxLayout * layout = new QVBoxLayout(ui->tab_allInfo);//整体布局
    ui->tab_allInfo->setLayout(layout);
    layout->setMargin(0);
    layout->setSpacing(0);
    lbtn_image = new LongButton(ui->tab_allInfo);//头像
    lbtn_image->setBtnNameIcon("");
    lbtn_image->setBtnNameText("头像");
    lbtn_image->setBtnInfoText(" >");
    lbtn_userName = new LongButton(ui->tab_allInfo);//用户名
    lbtn_userName->setBtnNameIcon("");
    lbtn_userName->setBtnNameText("昵称");
    lbtn_userName->setBtnInfoIcon("");
    lbtn_number = new LongButton(ui->tab_allInfo);//趣聊号
    lbtn_number->setBtnNameIcon("");
    lbtn_number->setBtnNameText("number");
    lbtn_number->setBtnInfoIcon("");
    lbtn_address = new LongButton(ui->tab_allInfo);//地址
    lbtn_address->setBtnNameIcon("");
    lbtn_address->setBtnNameText("地区");
    lbtn_address->setBtnInfoIcon("");
    lbtn_personalSignature = new LongButton(ui->tab_allInfo);//个性签名
    lbtn_personalSignature->setBtnNameIcon("");
    lbtn_personalSignature->setBtnNameText("signature");
    lbtn_personalSignature->setBtnInfoIcon("");
    lbtn_phone = new LongButton(ui->tab_allInfo);//电话
    lbtn_phone->setBtnNameIcon("");
    lbtn_phone->setBtnNameText("电话");
    lbtn_phone->setBtnInfoIcon("");
    connect(lbtn_image,&LongButton::sigClicked,this,&DetailedInfoPage::onToModifyPage);
    connect(lbtn_number,&LongButton::sigClicked,this,&DetailedInfoPage::onToModifyPage);
    connect(lbtn_userName,&LongButton::sigClicked,this,&DetailedInfoPage::onToModifyPage);
    connect(lbtn_address,&LongButton::sigClicked,this,&DetailedInfoPage::onToModifyPage);
    connect(lbtn_personalSignature,&LongButton::sigClicked,this,&DetailedInfoPage::onToModifyPage);
    connect(lbtn_phone,&LongButton::sigClicked,this,&DetailedInfoPage::onToModifyPage);
    layout->addWidget(lbtn_image);
    layout->addWidget(lbtn_userName);
    layout->addWidget(lbtn_number);
    layout->addWidget(lbtn_address);
    layout->addWidget(lbtn_personalSignature);
    layout->addWidget(lbtn_phone);
    QSpacerItem * spacer = new QSpacerItem(20,150);
    layout->addSpacerItem(spacer);
    //modifyInfo page index = 1

    //setup page index = 2

}

void DetailedInfoPage::setCurrentPage(int index, QString userName)
{
    tab_wdiget->setCurrentIndex(index);
    if(index == 0){
        ui->btn_save->hide();
        lb_tabTitle->setText("个人信息");
        QString oldUserName = lbtn_userName->getBtnInfoText();
        if(strncmp(userName.toUtf8().data(),oldUserName.toUtf8().data(),qstrlen(userName.toUtf8().data())) != 0){
            sqlite->getTableData("user_info", userName, mineData);
            setAllInfo();
        }
    }
    else if(index == 1){
        ui->btn_save->show();
        lb_tabTitle->setText("修改信息");
    }
    else if(index == 2){
        ui->btn_save->hide();
        lb_tabTitle->setText("设置");
    }
}

void DetailedInfoPage::setAllInfo()
{
    //mineData: userName number imagepath person address time
    if(mineData.size() != 0){
        lbtn_image->setBtnInfoIconSize(QSize(50,50));
        if(mineData[2] != "")
            lbtn_image->setBtnInfoIcon(mineData[2]);
        lbtn_userName->setBtnInfoText(mineData[0]+" >");
        lbtn_number->setBtnInfoText(mineData[1]+" >");
        lbtn_address->setBtnInfoText(mineData[4]+" >");
        lbtn_personalSignature->setBtnInfoText(mineData[3].left(20)+" >");
        lbtn_phone->setBtnInfoText(mineData[8]+" >");
    }
}

int DetailedInfoPage::getCurrentPage()
{
    return tab_wdiget->currentIndex();
}

DetailedInfoPage::~DetailedInfoPage()
{
    delete ui;
}

void DetailedInfoPage::onToModifyPage(QString btnNameText)
{
    QString modifyPageTitle = "";
    if(btnNameText == "头像"){
        QString imagePath = QFileDialog::getOpenFileName(this,
                                                         "choice image",
                                                         "",
                                                         "*.png;;*.PNG;;*.jpg");
        if(imagePath == "")
            return;
        ((LongButton*)this->sender())->setBtnInfoIcon(imagePath);
        //将图片发送到服务器
        //发送更改信息请求，修改服务器数据库
        //修改本地数据库
        return;//更改头像时不需要跳转到修改页面
    }
    else if(btnNameText == "昵称"){
        //更改后的名字上传到服务器后会出现找不到用户的问题
        //需要在服务器存储并转发新旧名字
//        lb_tabTitle->setText("更改名字");
        modifyPageTitle = "更改名字";
    }
    else if(btnNameText == "number"){
//        lb_tabTitle->setText("Set Number");//只能被设置一次，且必须唯一
        modifyPageTitle = "Set Number";
    }
    else if (btnNameText == "地区") {
//        lb_tabTitle->setText("更改地址");
        modifyPageTitle = "更改地址";
    }
    else if(btnNameText == "signature"){
        modifyPageTitle = "更改签名";
    }else if (btnNameText == "电话") {
        modifyPageTitle = "更改电话";
    }
    else
        return;
    btn_save->show();
    lb_tabTitle->setText(modifyPageTitle);
    ui->tabWidget->setCurrentIndex(1);//显示修改信息界面
}

void DetailedInfoPage::on_btn_cancle_clicked()
{
    if(tab_wdiget->currentIndex() == 1){//
        ui->tabWidget->setCurrentIndex(0);
        lb_tabTitle->setText("个人信息");
        btn_save->hide();
}
    else{                //返回到mine界面
        emit sigBackToMine();
    }
}
