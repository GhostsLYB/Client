#include "filesendrecv.h"

#define SENDSIZE 4096   //发送数据包最大为4k

FileSendRecv::FileSendRecv(QTcpSocket **sock, QString sendFilePath, QObject *parent) :
    QObject(parent),
    sendSock(*sock),
    filePath(sendFilePath),
    currNum(1),
    sendbuf(""),
    timer(nullptr)
{
    QFileInfo info(filePath);
    fileName = info.fileName();
    fileSize = info.size();
    maxNum = fileSize / SENDSIZE + 1;
    file.setParent(this);
    file.setFileName(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file" << filePath << "fail";
    }
    char temp[1024] = {0};
    sprintf(temp, "%4d", 0);
    sendbuf.append(temp);       //总长度
    sprintf(temp, "%4d", 4);
    sendbuf.append(temp);       //类型
    sprintf(temp, "%4d", qstrlen(fileName.toUtf8().data()));//文件名长度
    sendbuf.append(temp);
    sendbuf.append(fileName);   //文件名
    sprintf(temp, "%8d", currNum);
    sendbuf.append(temp);       //分段号
    sprintf(temp, "%8d", maxNum);
    sendbuf.append(temp);       //最大编号
    qDebug() << "maxNum is " << maxNum;
}

void FileSendRecv::sendFile(){
    //        /*发送文件的格式 总长度+类型+文件名长+文件名+分段编号+最大编号+数据段长度+数据段
    //         *           [  4 ]+[ 4]+[  4   ]+[ *  ]+[  8位 ]+[  8位 ]+[   4位  ]+[ *  ]*/

    if(timer == nullptr){
        timer = new QTimer(this);
    }
    connect(timer, &QTimer::timeout,[&](){
        QByteArray data;
        char temp[1024] = {0};
        QByteArray sendBta = sendbuf.toUtf8();
        sprintf(temp, "%8d", currNum);
        sendBta = sendBta.replace(12 + qstrlen(fileName.toUtf8().data()), 8, temp);
        data = file.read(4096);
        sprintf(temp, "%4d", qstrlen(data.data()));
        sendBta.append(temp);   //数据长度
        sendBta.append(data);   //数据
        sprintf(temp, "%4d", qstrlen(sendBta.data()) - 4);
        sendBta = sendBta.replace(0, 4, temp);memset(temp, 0, sizeof(temp));//设置总长度
        qint64 len = sendSock->write(sendBta);//客户端长连接套接字，应该是短链接套接字
        qDebug() << "currNum is " << currNum << " send len = " << len;
        if(currNum == maxNum){
            currNum = 0;
            maxNum = 0;
            file.close();
            timer->stop();
        }
        currNum++;
    });
//    while(currNum <= maxNum)
//    {

//    }
}
