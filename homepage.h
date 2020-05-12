#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QTabBar>
#include <QFileDialog>
#include <QVector>
#include <QList>
#include <QQueue>

#include "customWidget/widgetmain.h"
#include "customWidget/widgetfriend.h"
#include "customWidget/widgetmine.h"
#include "customUI/homepagetop.h"
#include "socket/control.h"
#include "globaldate.h"

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(Control * parentTrol = nullptr, QWidget *parent = nullptr);
    ~HomePage();
    void setIndex(int index);
    void setTopShow();
    void setUserName(QString name){userName = name;}
    QString getUserName(){return userName;}
    void addRecentChatItems(QVector<QList<QString>> &datas);
    void cleanRecentChatItems();

signals:
    void sigExit();
    void sigChatWith(QString);
    void sigShowUserInfo(QString, int backPage = 0);
    void sigSetMineInfo(QString, QString, QList<QString>&);
    void sigShowAllInfo(QString, int targetPage);
    void sigSearchShow();
    void syncFileDownloadFinish();

public slots:
    void onChatItemClicked(QString);
    void onFriendItemClicked(QString);
    void onSetMineInfo();
    void onDownloadFile(QString filePath);
    void onInitFriendList(QMap<QString,QString> &map);

private slots:
    void on_btn_exit_clicked();
    void on_btn_sendFile_clicked();
    void on_btn_recvFile_clicked();

private:
    Ui::HomePage    *ui;
    WidgetMain      *wid_main = nullptr;
    WidgetFriend    *wid_friend = nullptr;
    WidgetMine      *wid_mine;
    Control         *ctrl = nullptr;
    QTcpSocket      *recvSendFileSocket;
    QString         filePath;
    QString         userName;
    QList<QString>  mineInfo;

    QQueue<QString> recvFilePathQueue;
};

#endif // HOMEPAGE_H
