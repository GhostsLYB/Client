#include "socketcontrol.h"

QTcpSocket * const SocketControl::clientSocket = new QTcpSocket();
QTimer * SocketControl::timer = nullptr;

SocketControl::SocketControl(QObject *parent) : QObject(parent)
{

}

//创建一个用于发送文件的套接字
QTcpSocket * SocketControl::createSocket()
{
    QTcpSocket * sock = new QTcpSocket();
    sock->connectToHost("39.105.105.251", 5188);
    return sock;
}

bool SocketControl::isconnected(){
    if(clientSocket->state() == QTcpSocket::ConnectedState)
        return true;
    else {
        return false;
    }
}

QTcpSocket * SocketControl::socket(){
    return clientSocket;
}

void SocketControl::onConnected(){
    if(timer != nullptr)
    {
        if(timer->isActive())
            timer->stop();
        delete timer;
        timer = nullptr;
    }
}

void SocketControl::onDisconnected(){
    toConnect();
}

void SocketControl::toConnect(){
    timer = new QTimer();
    connect(timer,&QTimer::timeout,[](){
        if(!SocketControl::isconnected())
            clientSocket->connectToHost("39.105.105.251", 5188);
    });
    timer->start(3000);

}

bool SocketControl::send(QString &msg){
    if(isconnected())
    {
        clientSocket->write(msg.toUtf8());
        return true;
    }
    else {
        return false;
    }
}
