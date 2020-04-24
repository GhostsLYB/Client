#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include "socketcontrol.h"
#include <QThread>

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    QTcpSocket * createSocket();
    void sendFile(QTcpSocket**, QString);
    void sendFileRequest(QTcpSocket**, QString);
    void recvFile(QTcpSocket**);
    SocketControl * sock;
signals:
    void sigRegisterResponse(bool);
    void sigLoginResponse(QString);
    void sigRecvMessage(QString);
private:
    void processResponse(int, QString&);
    bool processRegisterMsg(QString & msg);
    void processLoginMsg(QString & msg);     //msg成功是“yes”,密码不正确是“no”,不存在用户是“null”
public slots:
    void onRead();
private:
    QFile * file;
    bool isFirstRecvFile;
    long recvFileSize;
    long recvSize;
};

#endif // CONTROL_H
