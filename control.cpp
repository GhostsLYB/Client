#include "control.h"

//文件名->文件的各个分段map
//分段号->改分段的数据
QMap<QString, QMap<int, QByteArray> > filebuf;

Control::Control(QObject *parent) : QObject(parent)
{
    sock = new SocketControl(this);
}

//服务器响应消息处理函数
void Control::processResponse(int flag, QString &msg)    //msg格式 ：信息段+信息段。。。
{
    //根据不同flag调用processMsg.h中的函数 并触发响应的信号
    switch (flag) {
    case 1: {
        bool isSuccessed = processRegisterMsg(msg);
        emit sigRegisterResponse(isSuccessed);
        return;
    }
    case 2: {
        processLoginMsg(msg);
        emit sigLoginResponse(msg);
        return;
    }
    case 3: {               //来自好友的消息 格式：好友名长度+好友名+消息长度+消息
        qDebug()<<msg;
        emit sigRecvMessage(msg);
        return;
    }
//    case 20:{
//        /*发送文件的格式 总长度+类型+文件名长+文件名+分段编号+最大编号+数据段长度+数据段
//         *           [  4 ]+[ 4]+[  4   ]+[ *  ]+[  8位 ]+[  8位 ]+[   4位  ]+[ *  ]*/
//        qDebug()<<msg;
//        saveFileSection(msg);
//        return;
//    }

    default: return;
    }
}

void Control::onRead()        //读取消息长度len和消息类型flag后交给processResponse函数
{
    QByteArray bta;
    bta = SocketControl::socket()->read(4);
    int len;
    len = bta.toInt();qDebug()<<"len = "<<len;  //len
    bta = SocketControl::socket()->read(len);
    QByteArray bflag =  bta.left(4);            // flag
    int flag = bflag.toInt();qDebug()<<"flag = "<<flag;
    QString msg = bta.remove(0, 4);
    if(flag == 20)
        saveFileSection(bta);
    else
        processResponse(flag, msg);

}

void Control::saveFileSection(QByteArray &bta)
{
    /*发送文件的格式 文件名长+文件名+分段编号+最大编号+数据段长度+数据段
     *           [  4   ]+[ *  ]+[  8位 ]+[  8位 ]+[   4位  ]+[ *  ]*/
    qDebug() << bta;
    int len = 0;
    len = bta.left(4).toInt();
    bta.remove(0,4);
    QString fileName = bta.left(len);
    bta.remove(0, len);
    int curSection = bta.left(8).toInt();//qDebug() << curSection;
    bta.remove(0, 8);
    int maxSection = bta.left(8).toInt();
    bta.remove(0, 8);
    bta.remove(0, 4);//此时bta是最后的数据部分
    QMap<QString, QMap<int, QByteArray> >::iterator iterSM = filebuf.find(fileName);
    QMap<int,QByteArray>::iterator iterIB;
    if(iterSM != filebuf.end()){            //将该片段存入filebuf map
        iterSM->insert(curSection, bta);
    }
    else {
        QMap<int, QByteArray> fileMap;
        fileMap.insert(curSection, bta);
        filebuf.insert(fileName, fileMap);
    }
    //如果该文件所有片段都已经接收，则打开(不存在的时候创建)该文件并写入数据
    if(filebuf.value(fileName).size() == maxSection){
        QFile file(QDir::currentPath() + "\\" + fileName);
        if(file.open(QFile::WriteOnly)){
            qDebug() << QDir::currentPath()<<"/"<<fileName;
            QMap<int,QByteArray> item = filebuf.value(fileName);
            for(QMap<int,QByteArray>::iterator iter = item.begin();iter != item.end();iter++){
                file.write(iter.value());
            }
            filebuf.remove(fileName);
        }
        else {
            qDebug() << "file " << fileName <<"open failed";
        }
    }
    if(SocketControl::socket()->bytesAvailable() > 0){
        emit SocketControl::socket()->readyRead();
    }
}

bool Control::processRegisterMsg(QString & msg)
{
    bool isSuccessed = false;
    qDebug()<< "**" <<msg << "**";
    if(msg.compare("   3yes") == 0)
        isSuccessed = true;
    return isSuccessed;
}

void Control::processLoginMsg(QString & msg)     //msg成功是“yes”,密码不正确是“no”,不存在用户是“null”
{
    QString temp = "";
    if(msg.compare("   3yes") == 0)
        temp = "yes";
    else if(msg.compare("   2no") == 0)
        temp = "no";
    else if(msg.compare("   6online") == 0)
        temp = "online";
    else {
        temp = "null";
    }
    msg = temp;
}
