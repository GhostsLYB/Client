#include "chatpage.h"
#include "ui_chatpage.h"

ChatPage::ChatPage(Control * parentCtrl,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    ctrl = parentCtrl;
    connect(ctrl,&Control::sigRecvMessage,this,&ChatPage::onRecvMessage);
    btn_back = ui->btn_back;
    lb_friendName = ui->lb_friendName;
    btn_info = ui->btn_info;
    listWidget = ui->listWidget;
    te_sendBox = ui->te_sendBox;
    btn_send = ui->btn_send;

    btn_send->setEnabled(false);    //pte_sendBox为空时发送按钮不可用
    //控制发送按钮的使能状态
    connect(te_sendBox,&QTextEdit::textChanged,[&](){
       if(te_sendBox->toPlainText().isEmpty())
       {
           btn_send->setEnabled(false);
       }
       else{
           btn_send->setEnabled(true);
       }
    });
    //连接发送按钮的点击信号和槽函数
    connect(btn_send,&QPushButton::clicked,this,&ChatPage::onSend);

}

void ChatPage::onSend()
{
    //发送文字消息的数据格式：[总长][类型3][接收用户名长][接收用户名][消息长度][消息]
    QString strTextEdit = "";
    QString sendMsg = "";
    char temp[1024] = {0};
    sprintf(temp,"%4d",3);
    strTextEdit.append(temp);                       //追加类型
    sprintf(temp,"%4d",qstrlen(lb_friendName->text().toUtf8().data()));
    strTextEdit.append(temp);
    strTextEdit.append(lb_friendName->text());      //追加接受用户名
    sprintf(temp,"%4d",qstrlen(te_sendBox->toPlainText().toUtf8().data()));
    strTextEdit.append(temp);                       //追加消息长度
    strTextEdit.append(te_sendBox->toPlainText());  //追加消息
    sprintf(temp,"%4d",qstrlen(strTextEdit.toUtf8().data()));
    sendMsg.append(temp);
    sendMsg.append(strTextEdit);
    qDebug() << "send message : " << sendMsg;
    if(ctrl->sock->send(sendMsg))   //如果发送成功则将消息添加到消息窗口listWidget中
    {
        addToListWidget(lb_friendName->text(),3,"send",te_sendBox->toPlainText(),"",QDateTime::currentDateTime().toString("hh:mm"));
        ChatInfo chatInfo = {lb_friendName->text(),3,"send",te_sendBox->toPlainText(),"",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")};
        emit sigSend(chatInfo);
    }
    te_sendBox->clear();
}

//处理来自其他人的消息 msg格式：对方用户名长度+对方用户名+消息长度+消息
void ChatPage::onRecvMessage(QString msg)
{
    //msg格式："   4root   9你好吗"
    qDebug() << "chatPage recv :"<<msg;
    QString len = msg.left(4);
    msg.remove(0,4);
    QString peerName = msg.left(len.toUtf8().toInt());
    msg.remove(0,len.toUtf8().toInt());
    len = msg.left(4);
    msg.remove(0,4);
    ChatInfo chatInfo = {peerName,3,"recv",msg,"",QDateTime::currentDateTime().toString()};
    emit sigRecv(chatInfo);
    if(lb_friendName->text() == peerName)   //如果当前聊天好友与接收的消息发送用户相同，则添加一天聊天记录
    {
        QListWidgetItem *item = new QListWidgetItem("recv : "+msg,listWidget);
        listWidget->addItem(item);
    }
}

void ChatPage::addToListWidget(const QString &peerName,const int &flag,const QString &dirction,
                               const QString &word,const QString &url,const QString &time)
{
    QListWidgetItem * item;
    if(dirction == "send")
        item = new QListWidgetItem("send : "+word,listWidget);
    else {
        item = new QListWidgetItem("recv : "+word,listWidget);
    }
    listWidget->addItem(item);
}

void ChatPage::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);
    emit isClosed();
}

//打开聊天页面时初始化与该用户的聊天记录，读取userName_chatIn表
void ChatPage::initInfo(QList<ChatInfo> *list)
{
    this->lb_friendName->setText(list->begin()->peerName);
    listWidget->clear();
    //加载与friendName的聊天记录
    for(QList<ChatInfo>::iterator iter = list->begin();iter != list->end(); iter++)
    {
        addToListWidget(iter->peerName,iter->flag,iter->direction,iter->word,iter->url,iter->time);
    }
}

ChatPage::~ChatPage()
{
    delete ui;
}
