#include "allpagelistwidget.h"
#include "ui_allpagelistwidget.h"
#include <QFile>

AllPageListWidget::AllPageListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllPageListWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("MI");
    ui->tabWidget->tabBar()->hide();
    sqlite = new SqliteControl(this);
    ctrl = new Control(this);
    //未连接时，创建QTimer重复请求连接，知道连接成功，停止连接请求，销毁QTimer对象
    connect(ctrl->sock->socket(),&QTcpSocket::connected,ctrl->sock,&SocketControl::onConnected);
    connect(ctrl->sock->socket(),&QTcpSocket::disconnected,ctrl->sock,&SocketControl::onDisconnected);
    connect(ctrl->sock->socket(),&QTcpSocket::readyRead,ctrl,&Control::onRead);
    connect(ctrl,&Control::sigRecvDeleteFriend,this,&AllPageListWidget::onRecvDeleteFriend);
    //login page index = 0
    login = new Login(ctrl);
    ui->tab_login->layout()->addWidget(login);
    connect(login,&Login::sigRegisterClicked,[&](){ui->tabWidget->setCurrentIndex(1);});
    connect(login,&Login::sigLoginSuccessed,this,&AllPageListWidget::onLoginSuccessed);
    //register page index = 1
    regist = new Register(ctrl);
    ui->tab_register->layout()->addWidget(regist);
    connect(regist,&Register::sigRegisterCancel,[&](){ui->tabWidget->setCurrentIndex(0);});
    //home page index = 2
    homepage = new HomePage(ctrl,this);
    ui->tab_home->layout()->addWidget(homepage);
    connect(homepage,&HomePage::sigChatWith,this,&AllPageListWidget::onChatWith);
    connect(homepage,&HomePage::sigShowUserInfo,this, &AllPageListWidget::onShowUserInfo);
    connect(homepage,&HomePage::sigExit,[&](){ui->tabWidget->setCurrentIndex(0);});
    connect(homepage,&HomePage::sigSetMineInfo,this,&AllPageListWidget::onSetMineInfo);
    connect(this,&AllPageListWidget::sigGetTableDataFinish,homepage,&HomePage::onSetMineInfo);
    connect(homepage,&HomePage::sigShowAllInfo,this,&AllPageListWidget::onShowAllInfo);

    //chat page index = 3
    chatPage = new ChatPage(ctrl);
    ui->tab_chat->layout()->addWidget(chatPage);
    ui->tab_chat->layout()->setMargin(0);
    ui->tab_chat->layout()->setSpacing(0);
    connect(chatPage->btn_back,&QPushButton::clicked,[&](){
        ui->tabWidget->setCurrentIndex(2);
        homepage->setIndex(0);
    });
    connect(chatPage,&ChatPage::sigRequestDownloadFile,homepage,&HomePage::onDownloadFile);
    connect(chatPage,&ChatPage::sigSend,this,&AllPageListWidget::onChatMsgInsert);
    connect(chatPage,&ChatPage::sigRecv,this,&AllPageListWidget::onChatMsgInsert);
    //userInfo page index = 4
    userInfoPage = new UserInfoPage(ctrl ,sqlite);
    ui->tab_userInfo->layout()->addWidget(userInfoPage);
    connect(userInfoPage, &UserInfoPage::sigChatWith,this,&AllPageListWidget::onChatWith);
    connect(userInfoPage, &UserInfoPage::sigFriendList,[&](){
        ui->tabWidget->setCurrentIndex(2);
        homepage->setIndex(1);
    });
    connect(userInfoPage,&UserInfoPage::sigDeleteFriend,[&](){
        //刷新好友列表页面
        sqlite->getFriendList(GlobalDate::getFriendNameImageMap());
        homepage->onInitFriendList(GlobalDate::getFriendNameImageMap());
        //刷新最接近聊天界面
        QVector<QList<QString>> recentChatInfo;
        sqlite->getRecentChatInfo(GlobalDate::currentUserName(),recentChatInfo); //读取所有最近聊天信息
        homepage->cleanRecentChatItems();                   //清空原有项
        homepage->addRecentChatItems(recentChatInfo);       //添加所有新项
    });
    //detailedInfo page index = 5
    detailedInfoPage = new DetailedInfoPage(ctrl, sqlite, ui->tab_allInfo);
    ui->tab_allInfo->layout()->addWidget(detailedInfoPage);
    ui->tab_allInfo->layout()->setMargin(0);
    ui->tab_allInfo->layout()->setSpacing(0);
    connect(detailedInfoPage,&DetailedInfoPage::sigBackToMine,[&](){
        ui->tabWidget->setCurrentIndex(2);
    });
    if(isLogin)
        ui->tabWidget->setCurrentIndex(2);
    else
        ui->tabWidget->setCurrentIndex(0);

    if(!ctrl->sock->isconnected())
        ctrl->sock->toConnect();

}

//与好友聊天界面
void AllPageListWidget::onChatWith(QString friendName)
{
    ui->tabWidget->setCurrentIndex(3);
    chatPage->listWidget->clear();
    chatPage->lb_friendName->setText(friendName);
    QList<ChatInfo> list;
    sqlite->getDataList(sqlite->getDatabaseName()+"_chatInfo",nullptr,&list,friendName);
    if(!list.isEmpty())
        chatPage->initInfo(&list);
}

//显示用户信息界面
void AllPageListWidget::onShowUserInfo(QString userName, int backPage = 0)
{
    ui->tabWidget->setCurrentIndex(4);//显示用户信息界面
    if(userName != userInfoPage->getUserName()){
        userInfoPage->setUserName(userName);
        userInfoPage->setBackPage(backPage);
        QList<QString> data;
        sqlite->getTableData("user_info", userName, data);
        userInfoPage->setUserInfo(data);
    }
}

void AllPageListWidget::onChatMsgInsert(ChatInfo chatInfo)
{
    sqlite->insertData(sqlite->getDatabaseName()+"_chatInfo",nullptr,&chatInfo);
}

void AllPageListWidget::onLoginSuccessed(QString userName)
{
//    登陆成功前要同步服务器数据
//    sqlite->importTxtForChatInfo();
//    登陆成功后要从数据库中读取数据（最最近聊天，好友列表）显示
    ui->tabWidget->setCurrentIndex(2);
    homepage->setIndex(0);
    homepage->setTopShow();
    homepage->setUserName(userName);
    sqlite->setDatabase(userName);  //连接本地数据库
    //读取表recent_chatInfo所有内容显示在最近聊天界面
    QVector<QList<QString>> recentChatInfo;
    sqlite->getRecentChatInfo(userName,recentChatInfo); //读取所有最近聊天信息
    homepage->cleanRecentChatItems();                   //清空原有项
    homepage->addRecentChatItems(recentChatInfo);       //添加所有新项
    qDebug() << "login user:"<<userName;//登陆成功
    GlobalDate::setCurrentUserName(userName);
    sqlite->getFriendList(GlobalDate::getFriendNameImageMap());//初始化好友列表
    homepage->onInitFriendList(GlobalDate::getFriendNameImageMap());
}

//获取数据库中表格的数据
void AllPageListWidget::onSetMineInfo(QString tableName, QString userName, QList<QString> &data)
{
    sqlite->getTableData(tableName, userName, data);
    emit sigGetTableDataFinish();
}

//显示详细信息界面
void AllPageListWidget::onShowAllInfo(QString userName, int targetPage)
{
    ui->tabWidget->setCurrentIndex(5);//显示已登录用户的详细信息
    detailedInfoPage->setCurrentPage(targetPage,userName);
}

//处理接受到的好友删除消息
void AllPageListWidget::onRecvDeleteFriend(QString msg)
{
    QString userName;
    QString userName2;
    int len = msg.left(4).toInt();
    msg.remove(0,4);
    userName = msg.left(len);
    msg.remove(0,len);
    len = msg.left(4).toInt();
    msg.remove(0,4);
    userName2 = msg.left(len);
    qDebug() << userName << " " << userName2;
    sqlite->deleteFriend(userName, userName2);
    //刷新好友列表页面
    sqlite->getFriendList(GlobalDate::getFriendNameImageMap());
    homepage->onInitFriendList(GlobalDate::getFriendNameImageMap());
    //刷新最近聊天界面
    QVector<QList<QString>> recentChatInfo;
    sqlite->getRecentChatInfo(GlobalDate::currentUserName(),recentChatInfo); //读取所有最近聊天信息
    homepage->cleanRecentChatItems();                   //清空原有项
    homepage->addRecentChatItems(recentChatInfo);       //添加所有新项
}

AllPageListWidget::~AllPageListWidget()
{
    delete ui;
}
