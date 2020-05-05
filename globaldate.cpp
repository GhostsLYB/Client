#include "globaldate.h"

QString GlobalDate::userName = "";
QMap<QString,QString> GlobalDate::userImageUrlMap = QMap<QString,QString>();

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
