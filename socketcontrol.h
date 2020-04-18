#ifndef SOCKETCONTROL_H
#define SOCKETCONTROL_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

class SocketControl : public QObject
{
    Q_OBJECT
public:
    explicit SocketControl(QObject *parent = nullptr);

signals:
    void sigRegisterResponse(bool);
    void sigLoginResponse(QString);
public:
    static QTcpSocket * const clientSocket;
    static bool isconnected();
    static QTcpSocket * socket();
    static void toConnect();

    bool send(QString &);
    void recv();

private:
    //static void processResponse(int, QString);

public slots:
    static void onConnected();
    static void onDisconnected();
private:
    static QTimer * timer;
};

#endif // SOCKETCONTROL_H
