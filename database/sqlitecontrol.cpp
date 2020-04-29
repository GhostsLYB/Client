#include "sqlitecontrol.h"
#include "sqlitecontrol.h"

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
    return false;
}

bool SqliteControl::isExistTable(QString table)
{
    bool bRet = false;
    if (!m_DataBase.open()) {
        return bRet;
    }
    QSqlQuery query(m_DataBase);
    bool ret =  query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(table));

    if (query.next())
    {
        if (query.value(0).toInt() > 0)
        {
            bRet = true;
        }
    }
    if (ret)
//        qDebug()<<"select sqlite_master successed "<<bRet;
//    else {
//        qDebug()<<"select sqlite_master fail"<<bRet;
//    }
    return bRet;
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
                                    "word      VARCHAR(1024)   ,"
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
                                    "lastMessage    VARCHAR(1024)   NOT NULL DEFAULT '',"
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
        selectString = QString("SELECT * FROM %1 where username = '%2'")
                                .arg(tableName)
                                .arg(userName);
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
                data << query.value(1).toString();
                data << query.value(2).toString();
                data << query.value(3).toString();
                data << query.value(4).toString();
                data << query.value(5).toString();
                data << query.value(6).toString();
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

QString SqliteControl::getDatabaseName()
{
    QString databaseName = m_DataBase.databaseName();
    databaseName = databaseName.remove(0, databaseName.lastIndexOf('/') + 1);
    databaseName = databaseName.left(databaseName.indexOf('.'));
    return databaseName;
}

//bool SqliteControl::importTxtForChatInfo()
//{
//    if(!m_DataBase.isOpen())
//        return false;
//    QString sql = "LOAD DATA INFILE 'E:/always/Linux/root_chatInfo.txt' "
//                  "INTO TABLE root_chatInfo "
//                  "FIELDS "
//                  "TERMINATED BY ',' "
//                  "ENCLOSED BY '\"' "
//                  "LINES "
//                  "TERMINATED BY '\r\n'";
//    qDebug() << sql;
//    QSqlQuery query = QSqlQuery(m_DataBase);
//    bool ret = query.exec(sql);
//    if(ret)
//        qDebug() << "import successed";
//    else {
//        qDebug() << "import fail";
//    }

//}






