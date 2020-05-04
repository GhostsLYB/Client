#include "homepage.h"
#include "ui_homepage.h"

HomePage::HomePage(Control * parentTrol, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage),
    recvSendFileSocket(nullptr)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();
    this->setWindowTitle("HomePage");
    this->setAttribute(Qt::WA_DeleteOnClose);

    if(parentTrol == nullptr)
        ctrl = new Control(this);
    else
        ctrl = parentTrol;
    //有消息接收的连接
    //connect(ctrl,&Control::sigRecvMessage,chatPage,&ChatPage::onRecvMessage);

    //最近聊天好友列表
    wid_main = new WidgetMain(ui->tab_main);
    wid_main->layout()->setMargin(0);
    wid_main->layout()->setSpacing(0);
    ui->tab_main->layout()->addWidget(wid_main);
    connect(wid_main,&WidgetMain::sigItemClicked,this,&HomePage::onChatItemClicked);
    //通讯录：所有好友列表
    wid_friend = new WidgetFriend(ui->tab_friend);
    wid_friend->layout()->setMargin(0);
    wid_friend->layout()->setSpacing(0);
    ui->tab_friend->layout()->addWidget(wid_friend);
    connect(wid_friend, &WidgetFriend::sigItemClicked, this, &HomePage::onFriendItemClicked);
    //我的信息界面
    wid_mine = new WidgetMine(ui->tab_mine);
    QVBoxLayout *wid_mine_2_layout = new QVBoxLayout(ui->tab_mine);
    wid_mine_2_layout->setMargin(0);
    wid_mine_2_layout->setSpacing(0);
    wid_mine_2_layout->addWidget(wid_mine);
    ui->wid_mine_2->setLayout(wid_mine_2_layout);
    connect(wid_mine,&WidgetMine::sigShowAllInfo,[&](QString userName, int targetPage){
        emit sigShowAllInfo(userName, targetPage);//目标页为0（详细信息界面）
    });
    //主页的四个页面的切换
    connect(ui->wid_homePage,&HomePageMenu::btn_pageClicked,[&](int index){
        ui->tabWidget->setCurrentIndex(index);
        if(ui->wid_homePageTop->isHidden() && index != 3)
            ui->wid_homePageTop->show();
        if(index == 0)
            ui->wid_homePageTop->setWord("趣聊");
        else if (index == 1)
            ui->wid_homePageTop->setWord("Friend");
        else if (index == 2)
            ui->wid_homePageTop->setWord("Dynamic");
        else{
            ui->wid_homePageTop->hide();
            if(wid_mine->getUserName() != userName){
                wid_mine->setUserName(userName);
                emit sigSetMineInfo("user_info", userName, mineInfo);
            }
        }
    });

}

//最近聊天界面点击好友项信号的槽函数
void HomePage::onChatItemClicked(QString friendName)
{
    //弹出与好友friend Name的俩天窗口
    qDebug() << "chat with "<<friendName;
    emit sigChatWith(friendName);
}

//通讯录界面点击好友项信号的槽函数
void HomePage::onFriendItemClicked(QString friendName)
{
    qDebug() << "show user info " << friendName;
    emit sigShowUserInfo(friendName);
}

void HomePage::onSetMineInfo(){
//    qDebug() << mineInfo.size();
//    for(int i = 0; i < mineInfo.size();i++){
//        qDebug() << mineInfo[i];
//    }
    wid_mine->setUserInfo(mineInfo);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::setIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void HomePage::setTopShow()
{
    if(ui->wid_homePageTop->isHidden())
        ui->wid_homePageTop->show();
}

void HomePage::addRecentChatItems(QVector<QList<QString> > &datas)
{
    //将数据datas的每一项添加为一个最近聊天好友项
    for(int i = 0; i < datas.size(); i++)
    {
        QList<QString> data = datas[i];
        wid_main->onAddFriendItem(data[0],"",data[1],data[2],data[3],data[4]);
    }
}

void HomePage::cleanRecentChatItems()
{
    wid_main->onCleanAllItems();
}

void HomePage::on_btn_exit_clicked()
{
    QString exit = "  12   0   4exit";
    ctrl->sock->send(exit);
    emit sigExit();
}

void HomePage::on_btn_sendFile_clicked()
{
    filePath = QFileDialog::getOpenFileName(this,"open file","","*");
//    QString fileName = filePath.mid(filePath.lastIndexOf('/') + 1);
    if(filePath == "")
        return;
    qDebug() << "file path : " << filePath;
//    qDebug() << "file name : " << fileName;
//    QTcpSocket * fileSocket = ctrl->createSocket();
    static QTcpSocket * fileSocket;
    fileSocket = nullptr;
    fileSocket = new QTcpSocket();
    connect(fileSocket, &QTcpSocket::connected, this,[&](){
        ctrl->sendFile(&fileSocket, filePath);
    });
    fileSocket->connectToHost("39.105.105.251", 5188);
}

void HomePage::on_btn_recvFile_clicked()
{
    if(recvSendFileSocket != nullptr){
        delete recvSendFileSocket;
        recvSendFileSocket = nullptr;
    }
    recvSendFileSocket = new QTcpSocket();
    connect(recvSendFileSocket, &QTcpSocket::connected,[&](){
        ctrl->sendFileRequest(&recvSendFileSocket, ui->le_fileName->text());
    });
    connect(recvSendFileSocket, &QTcpSocket::readyRead, [&](){
        ctrl->recvFile(&recvSendFileSocket);
    });
    recvSendFileSocket->connectToHost("39.105.105.251", 5188);
}
