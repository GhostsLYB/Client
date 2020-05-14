#include "globaldate.h"

QString GlobalDate::userName = "";
QMap<QString,QString> GlobalDate::userImageUrlMap = QMap<QString,QString>();
QMap<QString,QString> GlobalDate::friendNameImageMap = QMap<QString,QString>();
QString GlobalDate::globalFilePath = "/storage/emulated/0/IM/file/";

GlobalDate::GlobalDate(QObject *parent) : QObject(parent)
{

}

void GlobalDate::initCatalog(QString dirName)
{
    QDir *dir = new QDir;
    QUrl url("file:///storage/emulated/0/"+dirName);
    QString path = url.toLocalFile();  //获取到的路径
    qDebug() << "path = " << path;
    bool exist = dir->exists(path);
    if(exist)
    {
        qDebug()<< dirName << " exist";
    }
    else{
        qDebug() << "dir is not exist create " << dirName;
        if(dir->mkdir("/storage/emulated/0/"+dirName))
            qDebug() << "create successed " << dirName;
        else {
            qDebug() << "create fail " <<dirName;
        }
    }
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
