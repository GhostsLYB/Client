#include "homepage.h"
#include "ui_homepage.h"

HomePage::HomePage(Control * parentTrol, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
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
    connect(wid_main,&WidgetMain::sigItemClicked,this,&HomePage::onItemClicked);
    //通讯录：所有好友列表
    wid_friend = new WidgetFriend(ui->tab_friend);
    wid_friend->layout()->setMargin(0);
    wid_friend->layout()->setSpacing(0);
    ui->tab_friend->layout()->addWidget(wid_friend);
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
        else
            ui->wid_homePageTop->hide();
    });

}

void HomePage::onItemClicked(QString friendName)
{
    //弹出与好友friend Name的俩天窗口
    qDebug() << "chat with "<<friendName;
    emit sigChatWith(friendName);
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
    qDebug() << "file path : " << filePath;
//    qDebug() << "file name : " << fileName;
//    QTcpSocket * fileSocket = ctrl->createSocket();
    static QTcpSocket * fileSocket;
    fileSocket = nullptr;
    fileSocket = new QTcpSocket();
    fileSocket->connectToHost("39.105.105.251", 5188);
    connect(fileSocket, &QTcpSocket::connected, this,[&](){
        if(fileSocket->isWritable())
            qDebug() << "sock writable";
        if(fileSocket->isOpen())
            qDebug() << "sock opened";
        if(fileSocket->isValid())
            qDebug() << "sock Valid";
//        qint64 len = (fileSocket)->write(QString("  22   2   4root   6123456").toUtf8());
//        qDebug() << " send len = " << len;
        ctrl->sendFile(&fileSocket, filePath);
    });
}
