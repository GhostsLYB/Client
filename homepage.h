#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QTabBar>
#include <QFileDialog>
#include "customWidget/widgetmain.h"
#include "customWidget/widgetfriend.h"
#include "customWidget/widgetmine.h"
#include "customUI/homepagetop.h"
#include "socket/control.h"

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
signals:
    void sigExit();
    void sigChatWith(QString);
    void sigShowUserInfo(QString, int backPage = 0);

public slots:
    void onChatItemClicked(QString);
    void onFriendItemClicked(QString);

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
};

#endif // HOMEPAGE_H
