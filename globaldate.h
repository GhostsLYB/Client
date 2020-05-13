#ifndef GLOBALDATE_H
#define GLOBALDATE_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QDir>
#include <QDebug>

class GlobalDate : public QObject
{
    Q_OBJECT
public:
    explicit GlobalDate(QObject *parent = nullptr);
    static void initCatalog(QString dirName);
    static QString currentUserName(){return userName;}
    static void setCurrentUserName(QString name){userName = name;}
    static void insertUserImageUrl(QString userName, QString imageUrl);
    static QString getImageUrl(QString userName);
    static void insertFriendNameImage(QString userName,QString imageUrl);
    static QString getFriendImage(QString friendName);
    static void cleanFriendImageMap(){friendNameImageMap.clear();}
    static QMap<QString,QString> & getFriendNameImageMap(){return friendNameImageMap;}
private:
    static QString userName;
    static QMap<QString,QString> friendNameImageMap;
    static QMap<QString,QString> userImageUrlMap;
signals:

public slots:
};

#endif // GLOBALDATE_H
