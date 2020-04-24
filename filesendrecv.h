#ifndef FILESENDRECV_H
#define FILESENDRECV_H

#include <QObject>
#include <QTcpSocket>
#include <QFileInfo>
#include <QFile>
#include <QTimer>

class FileSendRecv : public QObject
{
    Q_OBJECT
public:
    explicit FileSendRecv(QTcpSocket** sock, QString filePath, QObject *parent = nullptr);
    void sendFile();

signals:

public slots:

private:
    QTcpSocket * sendSock;
    QString filePath;
    QString fileName;
    qint64  fileSize;
    int     maxNum  ;
    int     currNum ;
    QString sendbuf ;
    QFile   file;
    QTimer *timer;
};

#endif // FILESENDRECV_H
