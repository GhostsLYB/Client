#include "allpagelistwidget.h"
#include <QApplication>
#include "globaldate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GlobalDate::initCatalog("Download");
    GlobalDate::initCatalog("IM");
    GlobalDate::initCatalog("IM/file");
    GlobalDate::initCatalog("IM/data");
    /* 创建IM文件夹*/
//        QDir *photo = new QDir;
//        QUrl url("file:///storage/emulated/0/IM");
//        QString path = url.toLocalFile();  //获取到的路径
//        bool exist = photo->exists(path);
//        qDebug()<<"IM"<<exist;

//        if(exist)
//        {
//            QMessageBox::information(0,"eeee","ttttt");
//        } else
//        {
//            //创建IM文件夹
//            if(photo->mkdir(path))
//                qDebug() << "create successed";
//            else {
//                qDebug() << "create failed";
//            }
//            QMessageBox::information(0,"eeee","create IM");
//        }

    AllPageListWidget w;
    w.show();

    return a.exec();
}
