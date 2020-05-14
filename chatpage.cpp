#include "chatpage.h"
#include "ui_chatpage.h"
#include <QCursor>

#define CHAT_ITEM_HEIGHT 100

QString chatPagePath = "/storage/emulated/0/IM/file/";

ChatPage::ChatPage(Control * parentCtrl, SqliteControl *sqlite, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage),
    ctrl(parentCtrl),
    sqlite(sqlite)
{
    ui->setupUi(this);
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
    btn_soundRecord = qobject_cast<SoundRecordButton*>(ui->btn_soundRecord);
    connect(btn_soundRecord,&SoundRecordButton::sigRecordFinish,this,&ChatPage::onRecordFinish);
    btn_soundRecord->hide();        //录音按钮隐藏（此时msgType默认为0表示文字消息）
    ui->frame->hide();

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
    connect(btn_send,&QPushButton::clicked,[&](){onSend(3);});

}

void ChatPage::onSend(int msgFlag)
{
    //发送文字消息的数据格式：[总长][消息类型3][信息类型][接收用户名长][接收用户名][消息长度][消息]
    //消息类型为3表示转发 信息类型：3文字 7语音 8图片 9文件
    int tempFlag = msgType; //保存原消息类型
    if(msgFlag != -1)       //非默认参数时将消息类型改变
        msgType = msgFlag;
    QString strTextEdit = "";
    QString sendMsg = "";
    char temp[2048] = {0};
    sprintf(temp,"%4d",msgType);
    strTextEdit.append(temp);                       //追加消息类型msgType
//    sprintf(temp,"%4d",msgType);
//    strTextEdit.append(temp);                       //追加信息类型（3表示文字信息）
    sprintf(temp,"%4d",qstrlen(lb_friendName->text().toUtf8().data()));
    strTextEdit.append(temp);                       //用户名长度
    strTextEdit.append(lb_friendName->text());      //追加接受用户名
    QString msg;
    QString showInfo;
    if(msgType == 3){       //文本消息为textEdit中的文本
        sendFilePath = "";  //文字消息时
        showInfo = te_sendBox->toHtml();
        msg = ui->te_sendBox->toHtml();
    //    qDebug() << "sendBox->toHtml = [" << msg << "]";
        msg.replace("\"","&quot;");//将单引号和双引号替换
    //    qDebug() << msg;
        msg.replace("'","&apos;");
    //    qDebug() << msg;
        sprintf(temp,"%4d",qstrlen(msg.toUtf8().data()));
        strTextEdit.append(temp);                       //追加消息长度
    //    strTextEdit.append(te_sendBox->toPlainText());  //追加消息
        strTextEdit.append(msg);
    }
    else if (msgType == 7) {    //语音消息为音频文件路径 sendFilePath
        msg = sendFilePath;
        showInfo = "";
        sprintf(temp,"%4d",qstrlen(sendFilePath.toUtf8().data()));
        strTextEdit.append(temp);               //追加文件名长度
        strTextEdit.append(sendFilePath);       //追加文件名
    }
    else if (msgType == 8){     //图片消息
        msg = sendFilePath;
        showInfo = "";
        sprintf(temp,"%4d",qstrlen(sendFilePath.toUtf8().data()));
        strTextEdit.append(temp);               //追加文件名长度
        strTextEdit.append(sendFilePath);       //追加文件名
    }
    else if (msgType == 9) {    //文件消息
        msg = sendFilePath;
        showInfo = "";
        sprintf(temp,"%4d",qstrlen(sendFilePath.toUtf8().data()));
        strTextEdit.append(temp);               //追加文件名长度
        strTextEdit.append(sendFilePath);       //追加文件名
    }

    sprintf(temp,"%4d",qstrlen(strTextEdit.toUtf8().data()));
    sendMsg.append(temp);           //设置四字节首部的消息长度
    sendMsg.append(strTextEdit);    //追加消息
    qDebug() << "send message : " << sendMsg;
    if(ctrl->sock->send(sendMsg))   //如果发送成功则将消息添加到消息窗口listWidget中
    {
        addToListWidget(lb_friendName->text(),msgType,"send",msg,"",
                    QDateTime::currentDateTime().toString("hh:mm"));
        if(msgType == 7 || msgType == 8 || msgType == 9)
            msg = "";
        ChatInfo chatInfo = {lb_friendName->text(),msgType,"send",msg,sendFilePath,
                             QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")};
        emit sigSend(chatInfo);
    }
    msgType = tempFlag; //还原消息类型
    if(msgType == 3)
        sendFilePath = "";
    te_sendBox->clear();
}

//处理来自其他人的消息 msg格式：对方用户名长度+对方用户名+消息长度+消息
void ChatPage::onRecvMessage(QString msg, int flag)
{
    //msg文字格式："   4root   9你好吗"
    //语音格式："   4root   8filePath"
    QByteArray bta = msg.toUtf8();
    qDebug() << "chatPage recv :"<<bta;
    QString len = bta.left(4);              //对方用户名长度
    bta.remove(0,4);
    QString peerName = bta.left(len.toUtf8().toInt());  //对方名
    bta.remove(0,len.toUtf8().toInt());
    len = bta.left(4);                      //消息长度
    bta.remove(0,4);                        //此时bta为消息
    QString wordMsg = "";
    QString url = "";
    if(flag == 3)
        wordMsg = bta;
    else{
        url = bta;
    }
    ChatInfo chatInfo = {peerName,flag,"recv",wordMsg,url,
                         QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")};
    emit sigRecv(chatInfo);

    bta.replace("&quot;","\"");//将单引号和双引号替换
    bta.replace("&apos;","'");
//    qDebug() << "recv html = [" << bta << "]";
    if(lb_friendName->text() == peerName)   //正处于与该用户的聊天页面，则添加一天聊天记录
    {
        addToListWidget(lb_friendName->text(),flag,"recv",
                        bta,"",chatInfo.time);
    }
}

void ChatPage::onRecordFinish(QString audioPath)
{
    //保存音频文件路径到sendFilePath,用于发送文件到服务器
    sendFilePath = audioPath;
    qDebug() << "audio send";
    //发送音频文件到服务器
    ctrl->createSockAndSend(audioPath);
    //发送转发音频的消息到服务器并添加到listWdiget并存入数据库
    onSend();
//    addToListWidget(lb_friendName->text(),7,"send",audioPath,"",
//                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void ChatPage::addToListWidget(const QString &name,const int &flag,const QString &dirction,
                               const QString &wordOfUrl,const QString &imagePath,const QString &time)
{
    bool isSend = (dirction == "send")?true:false;
    QString userName = name;
    if(isSend)
        userName = GlobalDate::getImageUrl(GlobalDate::currentUserName());
    //需要通过name获取用户的的头像
    QString mImagePath = imagePath;
    if(mImagePath.isEmpty())
        mImagePath = GlobalDate::getImageUrl(userName);
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    item->setFlags(Qt::ItemIsEnabled);
    item->setSizeHint(QSize(0,CHAT_ITEM_HEIGHT));
    TextChatInfoItem * itemWidget = nullptr;
    QString msg = wordOfUrl;
    if(flag == 3)
    {
        msg.replace("&quot;","\"");//将单引号和双引号替换
        msg.replace("&apos;","'");
    }
    else if (flag == 7) {   //音频文件
        msg = QString("<img src=\":icon/app_icon/voiceInput.png\" />");
    }
    else if (flag == 8) {
        msg = QString("<img src=\"%1\" height=\"290\" width=\"190\" "
                      " margin-left=\"5\" margin-left=\"5\"/>").arg(wordOfUrl);
        item->setSizeHint(QSize(0,300));
    }
    else if (flag == 9){
        msg = wordOfUrl.mid(wordOfUrl.lastIndexOf('/')+1);
    }
    else {
        return;
    }

    itemWidget = new TextChatInfoItem(listWidget,mImagePath,msg,isSend,flag);
    if(flag == 7 || flag == 8 || flag == 9)
    {
        itemWidget->setFilePath(wordOfUrl);
    }
    connect(itemWidget,&TextChatInfoItem::sigRequestDownloadFile,this,&ChatPage::onDownloadFile);
    listWidget->setItemWidget(item,itemWidget);
    listWidget->setCurrentRow(listWidget->count()-1);
}

//打开聊天页面时初始化与该用户的聊天记录，读取userName_chatIn表
void ChatPage::initInfo(QList<ChatInfo> *list)
{
    this->lb_friendName->setText(list->begin()->peerName);
    listWidget->clear();
    //加载与friendName的聊天记录
    QString wordOrUrl = "";
    for(QList<ChatInfo>::iterator iter = list->begin();iter != list->end(); iter++)
    {
        if(iter->flag == 7 || iter->flag == 8 || iter->flag == 9){
            wordOrUrl = iter->url;
            qDebug() << iter->flag << ":" << wordOrUrl;
        }
        else {
            wordOrUrl = iter->word;
        }

        addToListWidget(iter->peerName,iter->flag,iter->direction,wordOrUrl,"",iter->time);
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
        msgType = 7;    //录音按钮显示，表示即将发送的消息为语音消息
        te_sendBox->hide();
        btn_send->hide();
        btn_otherSend->show();
    }
    else {
        btn_soundRecord->hide();  //录音按钮隐藏表示是即将发送的消息为文字消息
        msgType = 3;
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
//    te_sendBox->append("<img src=\":/icon/app_icon/ghost.png\" />");
    if(ui->frame->isHidden())
    {
        ui->frame->show();
        ui->btn_p1->show();
        ui->btn_p2->show();
        ui->btn_p3->show();
        ui->btn_sendFile->hide();
        ui->btn_sendPicture->hide();
    }
    else {
        ui->frame->hide();
    }
}


void ChatPage::on_btn_soundRecord_clicked()
{
    qDebug() << "start sound recording";
}

void ChatPage::on_btn_otherSend_clicked()
{
    qDebug() << "open send other file page";
    if(ui->frame->isHidden())
    {
        ui->frame->show();
        ui->btn_p1->hide();
        ui->btn_p2->hide();
        ui->btn_p3->hide();
        ui->btn_sendFile->show();
        ui->btn_sendPicture->show();
    }
    else {
        ui->frame->hide();
    }
}

void ChatPage::on_btn_p1_clicked()
{
    te_sendBox->append("<img src=\":/icon/app_icon/ghost.png\" />");
}

void ChatPage::on_btn_p2_clicked()
{
    te_sendBox->append("<img src=\":/icon/app_icon/add.png\" />");
}

void ChatPage::on_btn_p3_clicked()
{
    te_sendBox->append("<img src=\":/icon/app_icon/voiceInput.png\" />");
}

void ChatPage::on_btn_sendPicture_clicked()
{
    qDebug() << "btn send picture is clicked";
    sendFilePath = QFileDialog::getOpenFileName(this,"选择图片",chatPagePath,"*.jpg;;*.png");
    if(sendFilePath.isEmpty())
        return;
//    发送图片
    ctrl->createSockAndSend(sendFilePath);
    onSend(8);
}

void ChatPage::on_btn_sendFile_clicked()
{
    qDebug() << "btn send file is clicked";
    sendFilePath = QFileDialog::getOpenFileName(this,"选择文件",chatPagePath,"*");
    if(sendFilePath.isEmpty())
        return;
//    发送文件
    ctrl->createSockAndSend(sendFilePath);
    onSend(9);
}

