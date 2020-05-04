#include "chatpage.h"
#include "ui_chatpage.h"
#include <QCursor>

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
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_sendBox = ui->te_sendBox;
    te_sendBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_sendBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    btn_send = ui->btn_send;
    btn_send->hide();    //pte_sendBox为空时发送按钮不显示
    btn_voiceSend = ui->btn_voiceSend;
    btn_expression = ui->btn_expression;
    btn_otherSend = ui->btn_otherSend;
    btn_soundRecord = ui->btn_soundRecord;
    btn_soundRecord->hide();        //录音按钮隐藏

    //控制发送按钮的使能状态
    connect(te_sendBox,&QTextEdit::textChanged,[&](){
       if(te_sendBox->toPlainText().isEmpty())
       {
           btn_send->hide();
           btn_otherSend->show();
       }
       else{
           btn_send->show();
           btn_otherSend->hide();
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
//    sprintf(temp,"%4d",qstrlen(te_sendBox->toPlainText().toUtf8().data()));
    QString msg = ui->te_sendBox->toHtml();
    qDebug() << "sendBox->toHtml = [" << msg << "]";
    msg.replace("\"","&quot;");//将单引号和双引号替换
    qDebug() << msg;
    msg.replace("'","&apos;");
    qDebug() << msg;
    sprintf(temp,"%4d",qstrlen(msg.toUtf8().data()));
    strTextEdit.append(temp);                       //追加消息长度
//    strTextEdit.append(te_sendBox->toPlainText());  //追加消息
    strTextEdit.append(msg);
    sprintf(temp,"%4d",qstrlen(strTextEdit.toUtf8().data()));
    sendMsg.append(temp);
    sendMsg.append(strTextEdit);
    qDebug() << "send message : " << sendMsg;
    if(ctrl->sock->send(sendMsg))   //如果发送成功则将消息添加到消息窗口listWidget中
    {
        addToListWidget(lb_friendName->text(),3,"send",te_sendBox->toHtml(),"",
                        QDateTime::currentDateTime().toString("hh:mm"));
        ChatInfo chatInfo = {lb_friendName->text(),3,"send",msg,"",
                             QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")};
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
    ChatInfo chatInfo = {peerName,3,"recv",msg,"",
                         QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")};
    emit sigRecv(chatInfo);
    msg.replace("&quot;","\"");//将单引号和双引号替换
    msg.replace("&apos;","'");
    qDebug() << "recv html = [" << msg << "]";
    if(lb_friendName->text() == peerName)   //如果当前聊天好友与接收的消息发送用户相同，则添加一天聊天记录
    {
        addToListWidget(chatInfo.peerName,chatInfo.flag,chatInfo.direction,
                        chatInfo.word,chatInfo.url,chatInfo.time);
    }
}

void ChatPage::addToListWidget(const QString &peerName,const int &flag,const QString &dirction,
                               const QString &word,const QString &url,const QString &time)
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    item->setFlags(Qt::ItemIsEnabled);
    item->setSizeHint(QSize(0,50));
    TextChatInfoItem * itemWidget;
    QString msg = word;
    msg.replace("&quot;","\"");//将单引号和双引号替换
    msg.replace("&apos;","'");
//    qDebug() << "addToListWidget word = [" << word << "]";
    if(dirction == "send"){
        itemWidget = new TextChatInfoItem(listWidget,url,msg);
    }
    else {
        itemWidget = new TextChatInfoItem(listWidget,url,msg,false);
    }
    listWidget->setItemWidget(item,itemWidget);
    listWidget->setCurrentRow(listWidget->count()-1);
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

void ChatPage::on_btn_voiceSend_clicked()//改为录音输入，发送语音格式
{
    qDebug() << "changed send voice mode";
    if(btn_soundRecord->isHidden()){
        btn_soundRecord->show();
        te_sendBox->hide();
        btn_send->hide();
        btn_otherSend->show();
    }
    else {
        btn_soundRecord->hide();
        te_sendBox->show();
        if(!te_sendBox->toPlainText().isEmpty()){
            btn_otherSend->hide();
            btn_send->show();
        }
        else {
            btn_otherSend->show();
            btn_send->hide();
        }
    }
}



void ChatPage::on_btn_expression_clicked()
{
    qDebug() << "open expression chose page";
    te_sendBox->append("<img src=\":/icon/app_icon/ghost.png\" />");
}


void ChatPage::on_btn_soundRecord_clicked()
{
    qDebug() << "start sound recording";
}

void ChatPage::on_btn_otherSend_clicked()
{
    qDebug() << "open send other file page";
}
