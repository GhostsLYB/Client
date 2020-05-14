#include "detailedinfopage.h"
#include "ui_detailedinfopage.h"

QString openFilePath = "/storage/emulated/0/IM/file/";

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
    lb_tip = ui->lb_tip;
    le_infoInput = ui->le_infoInput;
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

void DetailedInfoPage::setCurrentPage(int index, QString userName, bool isReFresh)
{
    tab_wdiget->setCurrentIndex(index);
    if(index == 0){
        ui->btn_save->hide();
        lb_tabTitle->setText("个人信息");
        //需要刷新(默认不刷新)或者userName与currUserName不同时，需要重新查询数据
        if(isReFresh ||strncmp(userName.toUtf8().data(),
                   currUserName.toUtf8().data(),
                   qstrlen(userName.toUtf8().data())) != 0)
        {
            currUserName = userName;
            sqlite->getTableData("user_info", userName, mineData);
            initAllInfoPage();
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

void DetailedInfoPage::initAllInfoPage()
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
    QString modifyInfo = "";
    QString modifyTip = "";
    if(btnNameText == "头像"){
        QString imagePath = QFileDialog::getOpenFileName(this,
                                                         "choice image",
                                                         openFilePath,
                                                         "*.png;;*.PNG;;*.jpg");
        if(imagePath == "")
            return;
        //设置头像提示正在上传
        //((LongButton*)this->sender())->setBtnInfoIcon("://icon//uploading.png");
        //将图片发送到服务器

        ctrl->createSockAndSend(imagePath);
        QString sendStr = "";
        char sendbuf[100] = {0};
        //发送更改信息请求，修改服务器数据库(修改完成后需要又反馈信息)
        //修改数据的请求信息格式 总长+类型+更改类型+用户名长+用户名+更改信息长度+更改信息
        QString imageName = imagePath.mid(imagePath.lastIndexOf('/')+1);
        int nameSize = qstrlen(imageName.toUtf8().data());
        int totalSize = 16 + nameSize + qstrlen(mineData[0].toUtf8().data());
        sprintf(sendbuf, "%4d%4d%4d%4d%s%4d%s", totalSize,6,0,
                qstrlen(mineData[0].toUtf8().data()),
                mineData[0].toUtf8().data(),
                nameSize,imageName.toUtf8().data());
        sendStr = QString("%1").arg(sendbuf);
        qDebug() << sendbuf;
        qDebug() << sendStr;
        ctrl->sock->send(sendStr);
        //服务器信息修改完成后再修改本地数据库（期间应提示正在上传（需要完善））
        sqlite->update("user_info","username",mineData[0],"imageUrl",imagePath);
        //设置更改后的头像
        qobject_cast<LongButton*>(sender())->setBtnInfoIcon(imagePath);
        return;//更改头像时不需要跳转到修改页面
    }
    else if(btnNameText == "昵称"){
        modifyPageTitle = "更改名字";
        modifyInfo = mineData[0];
        modifyTip = "Change to a nice and domineering name";
    }
    else if(btnNameText == "number"){
        modifyPageTitle = "Set Number";
        modifyInfo = mineData[1];
        modifyTip = "只能设置一次，且唯一";
    }
    else if (btnNameText == "地区") {
        modifyPageTitle = "更改地址";
        modifyInfo = mineData[4];
        modifyTip = "告诉我你是哪里人";
    }
    else if(btnNameText == "signature"){
        modifyPageTitle = "更改签名";
        modifyInfo = mineData[3];
        modifyTip = "Change my mood";
    }else if (btnNameText == "电话") {
        modifyPageTitle = "更改电话";
        modifyInfo = mineData[8];
        modifyTip = "又换电话号喽";
    }
    else
        return;
    btn_save->show();
    lb_tabTitle->setText(modifyPageTitle);
    lb_tip->setText(modifyTip);
    le_infoInput->setText(modifyInfo);
    if(modifyPageTitle != "头像")
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

void DetailedInfoPage::on_btn_save_clicked()
{
    QString title = lb_tabTitle->text();
    QString info = le_infoInput->text();
    int infoSize = qstrlen(info.toUtf8().data());
    QString tableName = "user_info";
    QString modifyField = "";
    int modifyFlag = -1;
    char sendbuf[1024] = {0};
    if(title == "更改名字"){
        modifyField = "username";
        currUserName = le_infoInput->text();
        modifyFlag = 1;
    }
    else if(title == "Set Number"){
        modifyField = "number";
        modifyFlag = 2;
    }
    else if(title == "更改地址"){
        modifyField = "address";
        modifyFlag = 3;
    }
    else if(title == "更改签名"){
        modifyField = "personalSignature";
        modifyFlag = 4;
    }
    else if(title == "更改电话"){
        tableName = "users";
        modifyField = "phone";
        modifyFlag = 5;
    }
    else {
        return;
    }
    //发送修改信息请求
    sprintf(sendbuf,"%4d%4d%4d%4d%s%4d%s",
            16+qstrlen(mineData[0].toUtf8().data())+qstrlen(info.toUtf8().data()),
            6,modifyFlag,
            qstrlen(mineData[0].toUtf8().data()),
            mineData[0].toUtf8().data(),
            infoSize,info.toUtf8().data());
    QString msg = QString(sendbuf);
    qDebug() << msg;
    ctrl->sock->send(msg);
    //修改本地数据库
    bool res = sqlite->update(tableName,"username",mineData[0],modifyField,info);
    if(modifyField == "username")//用户名users表也需要更改
        sqlite->update("users","username",mineData[0],modifyField,info);
    if(!res)
        qDebug() << "mine info modify fail";
    //返回到mine界面,有修改信息，需要刷新显示信息
    setCurrentPage(0, currUserName, true);
}
