#ifndef ALLPAGELISTWIDGET_H
#define ALLPAGELISTWIDGET_H

#include <QWidget>
#include <QTabBar>
#include "homepage.h"
#include "login.h"
#include "register.h"
#include "chatpage.h"
#include "userinfopage.h"
#include "socket/control.h"
#include "database/sqlitecontrol.h"
#include "assistPage/detailedinfopage.h"

#include "globaldate.h"

namespace Ui {
class AllPageListWidget;
}

class AllPageListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AllPageListWidget(QWidget *parent = nullptr);
    ~AllPageListWidget();
    bool isLogined(){return isLogin;}
    void setLoginState(bool state){isLogin = state;}
signals:
    void sigGetTableDataFinish();
public slots:
    void onChatWith(QString);
    void onShowUserInfo(QString, int backPage);
    void onLoginSuccessed(QString);
    void onChatMsgInsert(ChatInfo);
    void onSetMineInfo(QString, QString, QList<QString>&);
    void onShowAllInfo(QString, int targetPage);
    void onRecvDeleteFriend(QString msg);
private:
    Ui::AllPageListWidget   *ui;
    Control                 *ctrl         = nullptr;
    HomePage                *homepage     = nullptr;
    Login                   *login        = nullptr;
    Register                *regist       = nullptr;
    ChatPage                *chatPage     = nullptr;
    UserInfoPage            *userInfoPage = nullptr;
    DetailedInfoPage        *detailedInfoPage = nullptr;
    bool                    isLogin       = false  ;
    SqliteControl           *sqlite       = nullptr;
};

#endif // ALLPAGELISTWIDGET_H
