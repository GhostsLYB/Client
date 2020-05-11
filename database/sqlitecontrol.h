#ifndef SQLITECONTROL_H
#define SQLITECONTROL_H

#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QList>
#include <QVector>
#include <QDebug>
#include <QSqlRecord>

#include "globaldate.h"

//每个用户登陆时创建数据库（数据库不存在的情况下）
struct ChatInfo{
public:
    QString peerName;
    int     flag;
    QString direction;
    QString word;
    QString url;
    QString time;
};
struct MyFriend{
public:
    int id;
    QString friendName;
};

class SqliteControl : public QObject
{
public:
    SqliteControl(QObject * parent = nullptr);
    void setDatabase(QString databaseName);
    bool isEnable();
    bool insertData(QString tableName, const MyFriend *mData, const ChatInfo *cData);
    bool createTable(QString tableName);
    bool deleteData(QString tableName, QString userName = nullptr);
    bool getData(QString tableName, QString userName, MyFriend * mData, ChatInfo * cData);
    bool getDataList(QString tableName, QList<MyFriend> * mList, QList<ChatInfo> * cList, QString userName = nullptr);
    bool getTableData(QString tableName, QString userName, QList<QString> &data);
    bool getRecentChatInfo(QString userName, QVector<QList<QString>> &data);
    void closeDatabase(){m_DataBase.close();}
    bool isExistTable(QString table);
    bool isFriend(QString userName1 ,QString userName2);
    void deleteFriend(QString userName1 ,QString userName2);
    void getFriendList(QMap<QString,QString> &map);
    QString getDatabaseName();
    bool update(QString tableName,QString conditionFieldName,QString condeitonValue,
                QString modifyFieldName,QString modifyValue);
    void importSyncData(QString loginUserName);
    bool userIsExist(QString userName);
private:
    QSqlDatabase m_DataBase;
private:
    bool initTable();
};

#endif // SQLITECONTROL_H
