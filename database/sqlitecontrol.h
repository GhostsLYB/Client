#ifndef SQLITECONTROL_H
#define SQLITECONTROL_H

#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QList>
#include <QVector>
#include <QDebug>
#include <QSqlRecord>

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

class SqliteControl : QObject
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
    QString getDatabaseName();
    bool update(QString tableName,QString conditionFieldName,QString condeitonValue,
                QString modifyFieldName,QString modifyValue);
//    bool importTxtForChatInfo();
private:
    QSqlDatabase m_DataBase;
private:
    bool initTable();
};

#endif // SQLITECONTROL_H
