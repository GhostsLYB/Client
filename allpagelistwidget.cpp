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
    //home page
    homepage = new HomePage(ctrl,this);
    ui->tab_home->layout()->addWidget(homepage);
    connect(homepage,&HomePage::sigChatWith,this,&AllPageListWidget::onChatWith);
    connect(homepage,&HomePage::sigExit,[&](){ui->tabWidget->setCurrentIndex(0);});
    //login page
    login = new Login(ctrl);
    ui->tab_login->layout()->addWidget(login);
    connect(login,&Login::sigRegisterClicked,[&](){ui->tabWidget->setCurrentIndex(1);});
    connect(login,&Login::sigLoginSuccessed,this,&AllPageListWidget::onLoginSuccessed);
    //register page
    regist = new Register(ctrl);
    ui->tab_register->layout()->addWidget(regist);
    connect(regist,&Register::sigRegisterCancel,[&](){ui->tabWidget->setCurrentIndex(0);});
    //chat page
    chatPage = new ChatPage(ctrl);
    ui->tab_chat->layout()->addWidget(chatPage);
    connect(chatPage->btn_back,&QPushButton::clicked,[&](){ui->tabWidget->setCurrentIndex(2);});
    connect(chatPage,&ChatPage::sigSend,this,&AllPageListWidget::onChatMsgInert);
    connect(chatPage,&ChatPage::sigRecv,this,&AllPageListWidget::onChatMsgInert);

    if(isLogin)
        ui->tabWidget->setCurrentIndex(2);
    else
        ui->tabWidget->setCurrentIndex(0);

    if(!ctrl->sock->isconnected())
        ctrl->sock->toConnect();

    //    QJsonObject *ob = new QJsonObject();
    //    ob->insert("flag",QJsonValue(1));
    //    ob->insert("message","msg part");

}

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

void AllPageListWidget::onChatMsgInert(ChatInfo chatInfo)
{
    sqlite->insertData(sqlite->getDatabaseName()+"_chatInfo",nullptr,&chatInfo);
}

void AllPageListWidget::onLoginSuccessed(QString userName)
{
    ui->tabWidget->setCurrentIndex(2);
    homepage->setIndex(0);
    homepage->setTopShow();
    sqlite->setDatabase(userName);
//    sqlite->importTxtForChatInfo();
    qDebug() << "login user:"<<userName;//登陆成功
}

AllPageListWidget::~AllPageListWidget()
{
    delete ui;
}
