#include "allpagelistwidget.h"
#include "ui_allpagelistwidget.h"
#include <QFile>

QString allResourceFilePath = "/storage/emulated/0/IM/file/";
//"E:/always/IM/file/";
//QString allResourceFilePath = GlobalDate::getGlobalFilePath();

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
    connect(ctrl,&Control::sigRecvAddFriend,this,&AllPageListWidget::onRecvAddFriend);
    connect(ctrl,&Control::sigRecvAddFriendResponse,this,&AllPageListWidget::onRecvAddFriendResponse);
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
    connect(homepage,&HomePage::sigSearchShow,[&](){
        ui->tabWidget->setCurrentIndex(6);
        le_input->clear();
        lb_waringInfo->setText("input user name for search!");
    });
    connect(homepage,&HomePage::syncFileDownloadFinish,[&](){
        sqlite->importSyncData(GlobalDate::currentUserName());
        //读取表recent_chatInfo所有内容显示在最近聊天界面
        QVector<QList<QString>> recentChatInfo;
        sqlite->getRecentChatInfo(GlobalDate::currentUserName(),recentChatInfo);
        homepage->cleanRecentChatItems();                   //清空原有项
        homepage->addRecentChatItems(recentChatInfo);       //添加所有新项
        //初始化好友列表
        sqlite->getFriendList(GlobalDate::getFriendNameImageMap());
        homepage->onInitFriendList(GlobalDate::getFriendNameImageMap());
    });
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
        ui->tabWidget->setCurrentIndex(2);    //删除好友后返回到homepage
    });
    connect(userInfoPage,&UserInfoPage::sigShowHomePage,[&](){
        ui->tabWidget->setCurrentIndex(2);
    });
    //detailedInfo page index = 5
    detailedInfoPage = new DetailedInfoPage(ctrl, sqlite, ui->tab_allInfo);
    ui->tab_allInfo->layout()->addWidget(detailedInfoPage);
    ui->tab_allInfo->layout()->setMargin(0);
    ui->tab_allInfo->layout()->setSpacing(0);
    connect(detailedInfoPage,&DetailedInfoPage::sigBackToMine,[&](){
        ui->tabWidget->setCurrentIndex(2);
    });
    //search page index = 6
    btn_searchBack = ui->btn_searchBack;
    btn_search = ui->btn_search;
    btn_search->setEnabled(false);
    le_input = ui->le_input;
    lb_waringInfo = ui->lb_waringInfo;
    connect(le_input,&QLineEdit::textChanged,[&](QString text){
        if(!text.isEmpty())
            btn_search->setEnabled(true);
        else
            btn_search->setEnabled(false);
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
    QFileInfo info(allResourceFilePath+GlobalDate::currentUserName()+"_chatInfo.txt");
    if(info.isFile())//聊天记录备份文件存在时导入
        sqlite->importSyncData(GlobalDate::currentUserName());
    ui->tabWidget->setCurrentIndex(3);
    chatPage->listWidget->clear();
    chatPage->lb_friendName->setText(friendName);
    QList<ChatInfo> list;//获取与该用户的聊天信息
    sqlite->getDataList(sqlite->getDatabaseName()+"_chatInfo",nullptr,&list,friendName);
    if(!list.isEmpty())
        chatPage->initInfo(&list);
}

//显示用户信息界面
void AllPageListWidget::onShowUserInfo(QString userName, int backPage = 0)
{
    ui->tabWidget->setCurrentIndex(4);//显示用户信息界面
    userInfoPage->setUserName(userName);
    userInfoPage->setBackPage(backPage);
    QList<QString> data;
    sqlite->getTableData("user_info", userName, data);
    userInfoPage->setUserInfo(data);
}

void AllPageListWidget::onChatMsgInsert(ChatInfo chatInfo)
{
    sqlite->insertData(sqlite->getDatabaseName()+"_chatInfo",nullptr,&chatInfo);
    //插入聊天信息后需要更新最近聊天信息界面
    QVector<QList<QString>> recentChatInfo;
    sqlite->getRecentChatInfo(GlobalDate::currentUserName(),recentChatInfo); //读取所有最近聊天信息
    homepage->cleanRecentChatItems();                   //清空原有项
    homepage->addRecentChatItems(recentChatInfo);       //添加所有新项
}

void AllPageListWidget::onLoginSuccessed(QString userName)
{
    qDebug() << "login user:"<<userName;//登陆成功
    //第一时间更改当前登陆用户名
    GlobalDate::setCurrentUserName(userName);
    ui->tabWidget->setCurrentIndex(2);
    homepage->setIndex(0);
    homepage->setTopShow();
    homepage->setUserName(userName);
    sqlite->setDatabase(userName);  //连接本地数据库
    //下载聊天记录文件
    homepage->onDownloadFile(allResourceFilePath+userName+"_chatInfo.txt");
    homepage->onDownloadFile(allResourceFilePath+userName+"_recent_chatList.txt");
    homepage->onDownloadFile(allResourceFilePath+userName+"_user_friendList.txt");
    homepage->onDownloadFile(allResourceFilePath+"user_info.txt");
    homepage->onDownloadFile(allResourceFilePath+"users.txt");

    //sqlite->importSyncData(userName);//同步服务器数据
    //读取表recent_chatInfo所有内容显示在最近聊天界面
//    QVector<QList<QString>> recentChatInfo;
//    sqlite->getRecentChatInfo(userName,recentChatInfo); //读取所有最近聊天信息
//    homepage->cleanRecentChatItems();                   //清空原有项
//    homepage->addRecentChatItems(recentChatInfo);       //添加所有新项
//    //初始化好友列表
//    sqlite->getFriendList(GlobalDate::getFriendNameImageMap());
//    homepage->onInitFriendList(GlobalDate::getFriendNameImageMap());
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
    QByteArray bta = msg.toUtf8();
    QString userName;
    QString userName2;
    int len = bta.left(4).toInt();
    bta.remove(0,4);
    userName = bta.left(len);
    bta.remove(0,len);
    len = bta.left(4).toInt();
    bta.remove(0,4);
    userName2 = bta.left(len);
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

void AllPageListWidget::onRecvAddFriend(QString msg)
{
    QByteArray bta = msg.toUtf8();
    QString userName;
    QString userName2;
    int len = bta.left(4).toInt();
    bta.remove(0,4);
    userName = bta.left(len);
    bta.remove(0,len);
    len = bta.left(4).toInt();
    bta.remove(0,4);
    userName2 = bta.left(len);//"   9刘彦斌   4root"
    //用户userName请求添加用户userName2
    qDebug() << userName << " " << userName2;
    QMessageBox::StandardButton btn = QMessageBox::question(this,"Add Request"
                                     ,QString("From %1's add request").arg(userName));
    sqlite->importSyncData(GlobalDate::currentUserName());//导入可能的新数据
    if(btn == QMessageBox::StandardButton::Yes){//同意添加
        qDebug() << userName2 << "is agree added";
        sqlite->insertFriendList(userName,userName2);//插入好友列表
        char ch[100] = {0};
        //响应添加好友成功消息格式 总长+类型12+名1长+请求名+名2长+响应名(msg:"   9刘彦斌   4root")
        sprintf(ch,"%4d%4d%s",4+qstrlen(msg.toUtf8().data()),12,msg.toUtf8().data());
        msg = QString(ch);
        qDebug() << "send msg = [" << msg << "]";
        ctrl->sock->send(msg);
        //刷新通讯录好友表列表
        sqlite->getFriendList(GlobalDate::getFriendNameImageMap());
        homepage->onInitFriendList(GlobalDate::getFriendNameImageMap());
    }
    else{   //不同意添加
        //什么也不做
        qDebug() << userName2 << "is not agree added";
    }
}

void AllPageListWidget::onRecvAddFriendResponse(QString msg)
{
    QByteArray bta = msg.toUtf8();
    QString userName;
    QString userName2;
    int len = bta.left(4).toInt();
    bta.remove(0,4);
    userName = bta.left(len);
    bta.remove(0,len);
    len = bta.left(4).toInt();
    bta.remove(0,4);
    userName2 = bta.left(len);//"   9刘彦斌   4root"
    //用户userName2响应用户userName的同意添加请求
    qDebug() << userName << " " << userName2;
    //存入数据库
    sqlite->insertFriendList(userName,userName2);
    //刷新通讯录好友列表
    sqlite->getFriendList(GlobalDate::getFriendNameImageMap());
    homepage->onInitFriendList(GlobalDate::getFriendNameImageMap());
}

AllPageListWidget::~AllPageListWidget()
{
    delete ui;
}

void AllPageListWidget::on_btn_searchBack_clicked()
{
//    if(searchBackPage == 0)
    ui->tabWidget->setCurrentIndex(2);//主页面
}

void AllPageListWidget::on_btn_search_clicked()
{
    qDebug() << "btn search clicked user is " << le_input->text();
    //判断用户是否存你在
    if(sqlite->userIsExist(le_input->text())){
        onShowUserInfo(le_input->text());
    }
    else {
        lb_waringInfo->setText(QString("user \"%1\" is not exist!").arg(le_input->text()));
    }
}
