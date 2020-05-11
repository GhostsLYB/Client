#include "sqlitecontrol.h"
#include "sqlitecontrol.h"

QString resourceFilePath = "E:/always/IM/file/";

SqliteControl::SqliteControl(QObject * parent)
    :QObject (parent)
{

}

void SqliteControl::setDatabase(QString databaseName)
{
    if (QSqlDatabase::contains(databaseName)) {
            qDebug() << "connected database "<<databaseName;
            m_DataBase = QSqlDatabase::database(databaseName);
    }
    else {
        qDebug() << "connecting database "<<databaseName;
        m_DataBase = QSqlDatabase::addDatabase("QSQLITE");
        m_DataBase.setDatabaseName(QCoreApplication::applicationDirPath()+"/"+ databaseName+".db");//windows
        //            m_DataBase.setDatabaseName("/storage/emulated/0/0_temp/"+databaseName+".db");//雷电 手机
        //            m_DataBase.setDatabaseName("/document/"+databaseName+".db");
    }
        initTable();
}

bool SqliteControl::isEnable()
{
    return m_DataBase.isValid();
}

bool SqliteControl::initTable()
{
    if (!m_DataBase.open()) {
        qDebug() << "Database open fail";
        return false;
    }
    createTable("my_friend");
    createTable(getDatabaseName() + "_chatInfo");
    createTable("user_friendList");
    createTable("recent_chatList");
    createTable("user_info");
    createTable("users");
    return false;
}

bool SqliteControl::isExistTable(QString table)
{
    bool bRet = false;
    if (!m_DataBase.open()) {
        return bRet;
    }
    QSqlQuery query(m_DataBase);
    query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));

    if (query.next())
    {
        if (query.value(0).toInt() > 0)
        {
            bRet = true;
        }
    }
    return bRet;
}

bool SqliteControl::isFriend(QString userName1, QString userName2)
{
    bool bRet = false;
    if (!m_DataBase.open()) {
        return bRet;
    }
    QSqlQuery query(m_DataBase);
    query.exec(QString("select count(*) from user_friendList where "
                       "(requestUser = '%1' and responseUser = '%2') or "
                       "(requestUser = '%3' and responseUser = '%4')")
               .arg(userName1).arg(userName2).arg(userName2).arg(userName1));

    if (query.next())
    {
        if (query.value(0).toInt() > 0)
        {
            bRet = true;
        }
    }
    return bRet;
}

void SqliteControl::deleteFriend(QString userName1, QString userName2)
{
    if (!m_DataBase.open())
        return;
/*
delete from recent_chatList where friendName = 'test';
delete from root_chatInfo where peerName = 'test';
delete from user_friendList where requestUser = 'test' or responseUser = 'test';
delete from user_info where username = 'test';
delete from users where username = 'test';
*/
    QSqlQuery query(m_DataBase);
    QString sql = QString("delete from recent_chatList where friendName = '%1'").arg(userName2);
    qDebug() << sql;
    query.exec(sql);//最近聊天记录
    sql = QString("delete from %1_chatInfo where peerName = '%2'")
            .arg(userName1).arg(userName2);
    qDebug() << sql;
    query.exec(sql);//删除好友聊天记录
    sql = QString("delete from user_friendList where requestUser = '%1' or responseUser = '%2'")
            .arg(userName2).arg(userName2);
    qDebug() << sql;
    query.exec(sql);//删除本地好友列表数据
    sql = QString("delete from user_info where username = '%1'").arg(userName2);
    qDebug() << sql;
    query.exec(sql);//删除本地账户信息
    sql = QString("delete from users where username = '%1'").arg(userName2);
    qDebug() << sql;
    query.exec(sql);//删除本地用户信息
}

void SqliteControl::getFriendList(QMap<QString, QString> &map)
{

    if(!m_DataBase.open())
        return ;
    QString currName = GlobalDate::currentUserName();
    QString sql = QString("select username,imageUrl "
                          "from user_friendList,user_info "
                          "where (user_friendList.responseUser = user_info.username "
                          "and user_friendList.requestUser = '%1') or"
                          "(user_friendList.requestUser = user_info.username "
                          "and user_friendList.responseUser = '%2' )")
            .arg(currName).arg(currName);
    qDebug() << "sql = [" << sql << "]";
    QSqlQuery query(m_DataBase);
    bool success = query.exec(sql);
    if(success)
    {
        map.clear();
        QSqlRecord rec = query.record();
        while (query.next())
        {
            map.insert(query.value(0).toString(),query.value(1).toString());
        }
    }
}

bool SqliteControl::createTable(QString tableName)
{
    if (!m_DataBase.open()) {
        return false;
    }
    QString createTableString;
    QSqlQuery query(m_DataBase);
    if(tableName == QString("my_friend"))
    {
        if(isExistTable(QString("my_friend")))
            return false;
        createTableString = QString("CREATE TABLE my_friend ("
                                    "id         INTEGER         PRIMARY KEY AUTOINCREMENT, "
                                    "friendName VARCHAR(40)     NOT NULL)");
    }
    else if (tableName.contains(QString("chatInfo")))
    {
        if(isExistTable(tableName))
            return false;
        createTableString = QString("CREATE TABLE %1 ("
                                    "id        INTEGER         PRIMARY KEY AUTOINCREMENT, "
                                    "peerName  VARCHAR(40)     NOT NULL, "
                                    "flag      INTEGER         NOT NULL, "
                                    "direction VARCHAR(20)     NOT NULL,"
                                    "word      VARCHAR(2048)   ,"
                                    "url       VARCHAR(100)    ,"
                                    "time      VARCHAR(100))    ").arg(tableName);
    }
    else if(tableName == QString("user_friendList")){
        if(isExistTable(QString("user_friendList")))
            return false;
        createTableString = QString("CREATE TABLE user_friendList ("
                                    "id             INTEGER     PRIMARY KEY AUTOINCREMENT, "
                                    "requestUser    VARCHAR(50) NOT NULL,"
                                    "responseUser   VARCHAR(50) NOT NULL,"
                                    "time           VARCHAR(50) NOT NULL)");
    }
    else if (tableName == QString("recent_chatList")) {
        if(isExistTable(QString("recent_chatList")))
            return false;
        createTableString = QString("CREATE TABLE recent_chatList ("
                                    "id             INTEGER         PRIMARY KEY AUTOINCREMENT,"
                                    "userName       VARCHAR(50)     NOT NULL,"
                                    "friendName     VARCHAR(50)     NOT NULL,"
                                    "lastMessage    VARCHAR(2048)   NOT NULL DEFAULT '',"
                                    "lastTime       VARCHAR(50)     NOT NULL DEFAULT '',"
                                    "voice          VARCHAR(10)     DEFAULT '')");
    }
    else if (tableName == QString("user_info")) {
        if(isExistTable(QString("user_info")))
            return false;
        createTableString = QString("CREATE TABLE user_info ("
                                    "id                 INTEGER      PRIMARY KEY AUTOINCREMENT,"
                                    "username           VARCHAR(50)  NOT NULL,"
                                    "number             VARCHAR(20)  DEFAULT '',"
                                    "imageUrl           VARCHAR(100) DEFAULT '',"
                                    "personalSignature  VARCHAR(200) DEFAULT '',"
                                    "address            VARCHAR(255) DEFAULT '',"
                                    "time               VARCHAR(20)  DEFAULT '')");
    }
    else if (tableName == QString("users")) {
        if(isExistTable(QString("users")))
            return false;
        createTableString = QString("CREATE TABLE users ("
                                    "id                 INTEGER         PRIMARY KEY AUTOINCREMENT NOT NULL,"
                                    "username           VARCHAR(50)     NOT NULL,"
                                    "sex                VARCHAR(10)     DEFAULT '',"
                                    "age                INTEGER         DEFAULT 0,"
                                    "email              VARCHAR(20)     DEFAULT '',"
                                    "phone              VARCHAR(20)     DEFAULT '')");
    }
    else {
        return false;
    }
    qDebug() <<createTableString;
    bool success = query.exec(createTableString);
    if (success) {
        return true;   //创建表成功
    }
    else {
        QSqlError lastError = query.lastError();
        QString err = lastError.driverText();
        qDebug() << "create table error:" << err;
        return false;
    }
}

bool SqliteControl::insertData(QString tableName, const MyFriend *mData, const ChatInfo *cData)
{
    if (!m_DataBase.open()) {
            return false;
    }
    QString insertString;
    if(tableName.contains(QString("chatInfo")))
    {
        insertString = QString("INSERT INTO %1 (id,peerName,flag,direction,word,url,time) "
                               "VALUES (null, '%2', %3, '%4', '%5', '%6', '%7')")
                               .arg(tableName)
                               .arg(cData->peerName)
                               .arg(cData->flag)
                               .arg(cData->direction)
                               .arg(cData->word)
                               .arg(cData->url)
                               .arg(cData->time);
    }
    else if (tableName == QString("my_friend")) {
        insertString = QString("INSERT INTO my_friend (id,friendName)"
                               "VALUES (null,'%1')").arg(mData->friendName);
    }
    else {
        return false;
    }
    qDebug() << insertString;
    QSqlQuery query(m_DataBase);
    bool success = query.exec(insertString);
    if (!success) {
        QSqlError lastError = query.lastError();
        QString err = lastError.driverText();
        qDebug() << "insert data error: " << err;
        return false;
    }
    return true;
}

bool SqliteControl::deleteData(QString tableName, QString userName)
{
    if (!m_DataBase.open()) {
            return false;
    }
    QString deleteString;
    if(tableName == QString("my_friend"))
    {
        deleteString = QString("DELETE FROM my_friend WHERE friendName = '%1'").arg(userName);
    }
    else if(tableName.contains("chatInfo") && !userName.isEmpty())
    {
        deleteString = QString("DELETE FROM %1 where peerName = '%2'").arg(tableName).arg(userName);
    }
    else {
        return false;
    }
    QSqlQuery query(m_DataBase);
    bool success = query.exec(deleteString);
    if(!success)
    {
        QSqlError error = query.lastError();
        qDebug() << "delete data error : " << error.driverText();
        return false;
    }
    return true;
}

bool SqliteControl::getData(QString tableName, QString userName, MyFriend * mData, ChatInfo * cData)
{
    if (!m_DataBase.open()) {
            return false;
    }
    QString selectString;
    if(tableName == QString("my_friend"))
    {
        selectString = QString("SELECT * FROM my_friend WHERE friendName = '%1'").arg(userName);
    }
    else if (tableName.contains(QString("chatInfo")) && !userName.isEmpty()) {
        selectString = QString("SELECT * FROM %1 WHERE ORDER BY id DESC LIMIT 1").arg(tableName);
    }
    else {
        return false;
    }
    QSqlQuery query(m_DataBase);
    bool scuess = query.exec(selectString);
    if(scuess){
        QSqlRecord rec = query.record();
        if (query.next())
        {
            if(tableName == QString("my_friend"))
            {
                mData->id         = query.value(0).toInt();
                mData->friendName = query.value(1).toString();
            }
            else if (tableName.contains(QString("chatInfo"))) {
                cData->peerName   = query.value(1).toString();
                cData->flag       = query.value(2).toInt();
                cData->direction  = query.value(3).toString();
                cData->word       = query.value(4).toString();
                cData->url        = query.value(5).toString();
                cData->time       = query.value(6).toString();
            }
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool SqliteControl::getDataList(QString tableName, QList<MyFriend> *mList, QList<ChatInfo> *cList, QString userName)
{
    if (!m_DataBase.open()) {
            return false;
    }
    QString selectString;
    if(tableName == QString("my_friend"))
    {
        selectString = QString("SELECT * FROM my_friend");
    }
    else if(tableName.contains(QString("chatInfo")))
    {
        selectString = QString("SELECT * FROM %1 where peerName = '%2'").arg(tableName).arg(userName);
    }
    else {
        return false;
    }
    QSqlQuery query(m_DataBase);
    bool success = query.exec(selectString);
    if(success)
    {
        QSqlRecord rec = query.record();
        MyFriend myFriend;
        ChatInfo chatInfo;
        while (query.next())
        {

            if(tableName == QString("my_friend"))
            {
                myFriend.id         = query.value(0).toInt();
                myFriend.friendName = query.value(1).toString();
                mList->append(myFriend);
            }
            else if (tableName.contains(QString("chatInfo"))) {
                chatInfo.peerName   = query.value(1).toString();
                chatInfo.flag       = query.value(2).toInt();
                chatInfo.direction  = query.value(3).toString();
                chatInfo.word       = query.value(4).toString();
                chatInfo.url        = query.value(5).toString();
                chatInfo.time       = query.value(6).toString();
                cList->append(chatInfo);
            }
            else {
                return false;
            }
        }
        return true;
    }
    else
        return false;
}

bool SqliteControl::getTableData(QString tableName, QString userName, QList<QString> &data)
{
    if (!m_DataBase.open()) {
            return false;
    }
    QString selectString;
    if(tableName == QString("my_friend"))
    {
        selectString = QString("SELECT * FROM my_friend");
    }
    else if(tableName == QString("user_info")){
        selectString = QString("select user_info.username,number,imageUrl,personalSignature,"
                               "address,sex,age,email,phone "
                               "FROM user_info,users "
                               "WHERE user_info.username = users.username and "
                               "user_info.username = '%1'").arg(userName);
        qDebug() << selectString;
    }
    else{
        return false;
    }
    QSqlQuery query(m_DataBase);
    bool success = query.exec(selectString);
    if(success)
    {
        QSqlRecord rec = query.record();
        MyFriend myFriend;
        ChatInfo chatInfo;
        data.clear();
        while (query.next())
        {

            if(tableName == QString("my_friend"))
            {
                data << query.value(0).toString();
                data << query.value(1).toString();
            }
            else if (tableName == QString("user_info")) {
                data << query.value(0).toString();
                data << query.value(1).toString();
                data << query.value(2).toString();
                data << query.value(3).toString();
                data << query.value(4).toString();
                data << query.value(5).toString();
                data << query.value(6).toString();
                data << query.value(7).toString();
                data << query.value(8).toString();
            }
            else {
                return false;
            }
        }
        return true;
    }
    else
        return false;
}

bool SqliteControl::getRecentChatInfo(QString userName, QVector<QList<QString>> &data)
{
    if (!m_DataBase.open()) {
            return false;
    }
    QString selectString = QString("select imageUrl,friendName,lastMessage,lastTime,voice "
                                   "from recent_chatList,user_info "
                                   "where recent_chatList.friendName = user_info.username "
                                   "and recent_chatList.userName = '%1' "
                                   "order by lastTime desc").arg(userName);
    qDebug() << "sql sentence = " << selectString;
    QSqlQuery query(m_DataBase);
    bool success = query.exec(selectString);
    if(success)
    {
        QSqlRecord rec = query.record();
        MyFriend myFriend;
        ChatInfo chatInfo;
        data.clear();
        while (query.next())
        {
            QList<QString> lData;
            lData << query.value(0).toString();
            lData << query.value(1).toString();
            lData << query.value(2).toString();
            lData << query.value(3).toString();
            lData << query.value(4).toString();
            data.append(lData);
        }
        return true;
    }
    else
        return false;
}

QString SqliteControl::getDatabaseName()
{
    QString databaseName = m_DataBase.databaseName();
    databaseName = databaseName.remove(0, databaseName.lastIndexOf('/') + 1);
    databaseName = databaseName.left(databaseName.indexOf('.'));
    return databaseName;
}

bool SqliteControl::update(QString tableName, QString conditionFieldName,
                           QString condeitonValue, QString modifyFieldName, QString modifyValue)
{
    if (!m_DataBase.open()) {
            return false;
    }
    QString sql = QString("update %1 set %2 = '%3' where %4 = '%5'")
            .arg(tableName)
            .arg(modifyFieldName)
            .arg(modifyValue)
            .arg(conditionFieldName)
            .arg(condeitonValue);
    QSqlQuery query(m_DataBase);
    bool success = query.exec(sql);
    if (!success) {
        QSqlError lastError = query.lastError();
        QString err = lastError.driverText();
        qDebug() << "insert data error: " << err;
        return false;
    }
    return true;
}

void SqliteControl::importSyncData(QString loginUserName)
{
    if(!m_DataBase.isOpen())
        return;
    QString currPath = resourceFilePath;
    //读取文件并存储到数据库，导入完成后删除文件
    QList<QString> fileList;
    fileList << QString("%1%2_chatInfo.txt").arg(currPath).arg(loginUserName)
             << QString("%1%2_recent_chatList.txt").arg(currPath).arg(loginUserName)
             << QString("%1%2_user_friendList.txt").arg(currPath).arg(loginUserName)
             << QString("%1user_info.txt").arg(currPath)
             << QString("%1users.txt").arg(currPath);
    QList<QString> tableNameList;
    tableNameList << QString("%1_chatInfo").arg(loginUserName)
                  << "recent_chatList" << "user_friendList" << "user_info" << "users";
    QString sql = "";
    QList<QString> info;
    QSqlQuery query(m_DataBase);
    for (int i = 0; i < fileList.count(); i++) {
        QFile file(fileList[i]);
        if(file.open(QIODevice::ReadOnly)){
            qDebug() << "file [" << fileList[i] << "] open success!";
            QByteArray allData = file.readAll();    //读取左右数据
            if(i == 0){//userName_chatInfo表
                //"48" "root" "3" "send" "时间格式" "" "2020-05-04 11:52:38"
                QList<QByteArray> list = allData.split('"');
                for(int j = 0; j < list.count() - 1; ){  //所有数据在list中
                    info.clear();
                    for(int n = 0; n < 7 && j < list.count(); n++){//一条记录读取到info中
                        info << QString(list[j+1]);
                        j += 2;
                    }
                    if(info.count() == 7){//有七个字段时
                        sql = QString("insert into %1 values(%2,'%3',%4,'%5','%6','%7','%8')")
                                .arg(tableNameList[i])
                                .arg(info[0].toInt()).arg(info[1]).arg(info[2].toInt())
                                .arg(info[3]).arg(info[4]).arg(info[5]).arg(info[6]);
//                        qDebug() << sql;
                        query.exec(sql);
                    }
                }
            }
            else if(i == 1){//recent_chatList表
                //"3" "root" "root" "this is root" "2020-05-05 13:13:11" "0"
                QList<QByteArray> list = allData.split('"');
                for(int j = 0; j < list.count() - 1; ){  //所有数据在list中
                    info.clear();
                    for(int n = 0; n < 6 && j < list.count(); n++){//一条记录读取到info中
                        info << QString(list[j+1]);
                        j += 2;
                    }
                    if(info.count() == 6){//有6个字段时
                        sql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7')")
                                .arg(tableNameList[i])
                                .arg(info[0].toInt()).arg(info[1]).arg(info[2])
                                .arg(info[3]).arg(info[4]).arg(info[5]);
//                        qDebug() << sql;
                        query.exec(sql);
                    }
                }
            }
            else if(i == 2){//user_friendList表
                //"1" "ghost" "root" "2020-05-11 11:15:01"
                QList<QByteArray> list = allData.split('"');
                for(int j = 0; j < list.count() - 1; ){  //所有数据在list中
                    info.clear();
                    for(int n = 0; n < 4 && j < list.count(); n++){//一条记录读取到info中
                        info << QString(list[j+1]);
                        j += 2;
                    }
                    if(info.count() == 4){//有4个字段时
                        sql = QString("insert into %1 values(%2,'%3','%4','%5')")
                                .arg(tableNameList[i])
                                .arg(info[0].toInt()).arg(info[1]).arg(info[2])
                                .arg(info[3]);
                        qDebug() << sql;
                        query.exec(sql);
                    }
                }
            }
            else if(i == 3){//user_info表
                //"1" "root" "IM0001" "/tmp/图片1.png" "人之初，性本善。" "哈尔滨巴彦" "2020-03-14 12:12:12"
                QList<QByteArray> list = allData.split('"');
                for(int j = 0; j < list.count() - 1; ){  //所有数据在list中
                    info.clear();
                    for(int n = 0; n < 7 && j < list.count(); n++){//一条记录读取到info中
                        info << QString(list[j+1]);
                        j += 2;
                    }
                    if(info.count() == 7){//有7个字段时
                        //第四个字段是路径 需要更改
                        info[3] = resourceFilePath + info[3].mid(info[3].lastIndexOf('/')+1);
                        sql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7','%8')")
                                .arg(tableNameList[i])
                                .arg(info[0].toInt()).arg(info[1]).arg(info[2])
                                .arg(info[3]).arg(info[4]).arg(info[5]).arg(info[6]);
                        qDebug() << sql;
                        query.exec(sql);
                    }
                }
            }
            else if(i == 4){//users表
                //"1" "root" "男" "20" "123456@qq.com" "182111111111"
                QList<QByteArray> list = allData.split('"');
                for(int j = 0; j < list.count() - 1; ){  //所有数据在list中
                    info.clear();
                    for(int n = 0; n < 6 && j < list.count(); n++){//一条记录读取到info中
                        info << QString(list[j+1]);
                        j += 2;
                    }
                    if(info.count() == 6){//有6个字段时
                        sql = QString("insert into %1 values(%2,'%3','%4',%5,'%6','%7')")
                                .arg(tableNameList[i])
                                .arg(info[0].toInt()).arg(info[1]).arg(info[2])
                                .arg(info[3].toInt()).arg(info[4]).arg(info[5]);
                        qDebug() << sql;
                        query.exec(sql);
                    }
                }
            }
            file.remove();
        }
        else {
            qDebug() << "file [" << fileList[i] << "] open fail!";
        }
    }
}

bool SqliteControl::userIsExist(QString userName)
{
    bool bRet = false;
    if (!m_DataBase.open())
        return false;
    QString sql = QString("select count(*) from user_info where username = '%1'").arg(userName);
    QSqlQuery query(m_DataBase);
    query.exec(sql);
    if (query.next())
    {
        if (query.value(0).toInt() > 0)
        {
            bRet = true;
        }
    }
    return bRet;
}
