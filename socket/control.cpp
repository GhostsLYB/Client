#include "control.h"

#define SENDSIZE 4096   //发送数据包最大为4k
#define BUFSIZE  4296

Control::Control(QObject *parent) :
    QObject(parent),
    file(nullptr),
    isFirstRecvFile(true),
    recvFileSize(0),
    recvSize(0)
{
    sock = new SocketControl(this);
//    connect(this,&Control::sigDeleteSock,[](QTcpSocket* sock){delete sock;});
}

//创建一个新的套接字用于收发文件
QTcpSocket * Control::createSocket()
{
    return sock->createSocket();
}

void Control::createSockAndSend(QString filePath)
{
    //创建一个用于发送filePath文件的sock,并加入fileSockMap中
    //发送完成后从fileSockMap中删除并delete
    QTcpSocket * fileSock = createSocket();
    fileSockMap.insert(filePath,fileSock);
    connect(fileSock, &QTcpSocket::connected, this, &Control::onFileSockConnected);
//    connect(fileSock, &QTcpSocket::disconnected,this, &Control::onFileSockDisconnected);
    fileSock->connectToHost("39.105.105.251", 5188);
}

void Control::sendFile(QTcpSocket ** sock, QString filePath)
{
    //发送文件的格式 总长度+类型+文件名长+文件名+文件大小
    //            [ 4 ]+[ 4]+[  4 ]+[ * ]+[ 8位 ]
    QFileInfo info(filePath);
    QString fileName = info.fileName();
    qint64  fileSize = info.size();
    QFile file(filePath, this);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file" << filePath << "fail";
    }
    QByteArray data;
    QString sendbuf = "";
    char temp[1024] = {0};
    sprintf(temp, "%4d", 0);
    sendbuf.append(temp);       //总长度
    sprintf(temp, "%4d", 4);
    sendbuf.append(temp);       //类型
    uint fileNameLen = qstrlen(fileName.toUtf8().data());
    sprintf(temp, "%4d", fileNameLen);//文件名长度
    sendbuf.append(temp);
    sendbuf.append(fileName);   //文件名
    sprintf(temp, "%8lld", fileSize);
    sendbuf.append(temp);       //文件大小
    sprintf(temp, "%4d", qstrlen(sendbuf.toUtf8().data()) - 4);
    QByteArray sendBta = sendbuf.toUtf8().replace(0, 4, temp);//设置信息总长度
    (*sock)->write(sendBta);    //发送文件信息
    QThread * currThread = QThread::currentThread();
    currThread->msleep(50);
    while(!file.atEnd())
    {
        data = file.read(4096);
        (*sock)->write(data);//客户端长连接套接字，应该是短链接套接字
    }
    (*sock)->disconnectFromHost();
//    delete (*sock);
}

void Control::sendFileRequest(QTcpSocket ** sock, QString fileName){
    /*msg格式 总长+类型+文件名长+文件名       文件信息格式：文件名长+文件名+文件长度
             [4]+[ 4]+[4]+[ *  ]                  [   4  ]+[  * ]+[  8  ]*/
    char sendbuf[100] = {0};
    QString filePath = "E:\\always\\IM\\file\\" +fileName;
    file = new QFile(filePath);
    file->remove(); //删除原有同名文件文件
    if(!file->open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug() << "file [" << filePath << "[ open fail";
    }
    qDebug() << "file path is [" << filePath << "]";
    int fileNameSize = qstrlen(fileName.toUtf8().data());
    sprintf(sendbuf,"%4d%4d%4d%s", 8+fileNameSize, 5, fileNameSize, fileName.toUtf8().data());
    qDebug() << "sendbuf is [" << sendbuf << "]";
    (*sock)->write(QString(sendbuf).toUtf8());
}

void Control::recvFile(QTcpSocket **sock){
    if(isFirstRecvFile){    //第一次接收文件信息数据
        QByteArray msg = (*sock)->readAll();
        qDebug() << "first recv file info : " << msg;
        int fileNameSize = msg.left(4).toInt();
        msg.remove(0, 4);
        msg.remove(0, fileNameSize);
        recvFileSize = msg.toLong();
        isFirstRecvFile = false;
    }
    else {                  //接收文件数据并存入本地文件
        QByteArray msg = (*sock)->readAll();
//        qDebug() << "file data : " << msg;
        qint64 len = file->write(msg);
        recvSize += len;
        qDebug() << "write file size [" << len << "]" << "all write [" << recvSize <<"]";
        if(recvSize >= recvFileSize){
            file->close();
            isFirstRecvFile = true;
            recvFileSize = 0;
            recvSize = 0;
        }
    }
}
//服务器响应消息处理函数
void Control::processResponse(int flag, QString &msg)    //msg格式 ：信息段+信息段。。。
{
    //根据不同flag调用processMsg.h中的函数解析数据 并触发响应的信号
    switch (flag) {
    case 1: {
        bool isSuccessed = processRegisterMsg(msg);
        emit sigRegisterResponse(isSuccessed);
        return;
    }
    case 2: {
        processLoginMsg(msg);
        emit sigLoginResponse(msg);
        return;
    }
    case 3: {               //来自好友的消息 格式：好友名长度+好友名+消息长度+消息
        qDebug() << msg;
        emit sigRecvMessage(msg, 3);
        return;
    }
    case 7:{
        qDebug() << "recv audio msg = " << msg;
        emit sigRecvMessage(msg, 7);
        return;
    }

    default: return;
    }
}

void Control::onRead()        //读取消息长度len和消息类型flag后交给processResponse函数
{
    QByteArray bta;
    bta = SocketControl::socket()->read(4);
    int len;
    len = bta.toInt();qDebug()<<"len = "<<len;  //len
    bta = SocketControl::socket()->read(len);
    QByteArray bflag =  bta.left(4);            // flag
    int flag = bflag.toInt();qDebug()<<"flag = "<<flag;
    QString msg = bta.remove(0, 4);
    processResponse(flag, msg);
}

void Control::onFileSockConnected()
{
    //从fileSockMap中找到该套接字关联的文件路径，移除
    QTcpSocket * sock = qobject_cast<QTcpSocket*>(sender());
    QString filePath = "";
    QMap<QString,QTcpSocket*>::iterator iter = fileSockMap.begin();
    for (; iter != fileSockMap.end(); iter++) {
        if(iter.value() == sock){
            filePath = iter.key();
            fileSockMap.erase(iter);
            break;
        }
    }
    if(filePath == "")
        qDebug() << "send file path is null";
    //发送文件
    sendFile(&sock,filePath);
    //删除对象
//    delete sock;
}

//void Control::onFileSockDisconnected()
//{
//    QTcpSocket * sock = qobject_cast<QTcpSocket*>(sender());
////    if(sock != nullptr)
////        emit sigDeleteSock(sock);
//}

bool Control::processRegisterMsg(QString & msg)
{
    bool isSuccessed = false;
    qDebug()<< "**" <<msg << "**";
    if(msg.compare("   3yes") == 0)
        isSuccessed = true;
    return isSuccessed;
}

void Control::processLoginMsg(QString & msg)     //msg成功是“yes”,密码不正确是“no”,不存在用户是“null”
{
    QString temp = "";
    if(msg.compare("   3yes") == 0)
        temp = "yes";
    else if(msg.compare("   2no") == 0)
        temp = "no";
    else if(msg.compare("   6online") == 0)
        temp = "online";
    else {
        temp = "null";
    }
    msg = temp;
}
