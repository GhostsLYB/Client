#include "globaldate.h"

QString GlobalDate::userName = "";
QMap<QString,QString> GlobalDate::userImageUrlMap = QMap<QString,QString>();
QMap<QString,QString> GlobalDate::friendNameImageMap = QMap<QString,QString>();

GlobalDate::GlobalDate(QObject *parent) : QObject(parent)
{

}

void GlobalDate::insertUserImageUrl(QString userName, QString imageUrl)
{
    userImageUrlMap.insert(userName,imageUrl);
}

QString GlobalDate::getImageUrl(QString userName)
{
    return userImageUrlMap.value(userName);
}

void GlobalDate::insertFriendNameImage(QString userName, QString imageUrl)
{
    friendNameImageMap.insert(userName,imageUrl);
}

QString GlobalDate::getFriendImage(QString friendName)
{
    return friendNameImageMap.value(friendName);
}
