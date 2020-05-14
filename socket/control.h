#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <QMap>

#include "socket/socketcontrol.h"
#include "../globaldate.h"

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    QTcpSocket * createSocket();
    void sendFile(QTcpSocket**, QString);
    void sendFileRequest(QTcpSocket**, QString);
    void createSockAndSend(QString filePath);
    void recvFile(QTcpSocket**);
    SocketControl * sock;
signals:
    void sigRegisterResponse(bool);
    void sigLoginResponse(QString);
    void sigRecvMessage(QString msg, int flag);
    void sigRecvDeleteFriend(QString msg);
    void sigRecvAddFriend(QString msg);
    void sigRecvAddFriendResponse(QString msg);
    void sigOneFileRecvFinish(QString filePath);
//    void sigDeleteSock(QTcpSocket*);
private:
    void processResponse(int, QString&);
    bool processRegisterMsg(QString & msg);
    void processLoginMsg(QString & msg);     //msg成功是“yes”,密码不正确是“no”,不存在用户是“null”
    void saveFileSection(QByteArray &bta);
public slots:
    void onRead();
    void onFileSockConnected();
//    void onFileSockDisconnected();
private:
    QFile * file;
    bool isFirstRecvFile;
    long recvFileSize;
    long recvSize;
    QString currentRecvFilePath;
    QMap<QString, QTcpSocket*> fileSockMap;
};

#endif // CONTROL_H
