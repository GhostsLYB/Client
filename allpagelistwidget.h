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
public slots:
    void onChatWith(QString);
    void onShowUserInfo(QString, int);
    void onLoginSuccessed(QString);
    void onChatMsgInsert(ChatInfo);
private:
    Ui::AllPageListWidget *ui;
    Control       *ctrl         = nullptr;
    HomePage      *homepage     = nullptr;
    Login         *login        = nullptr;
    Register      *regist       = nullptr;
    ChatPage      *chatPage     = nullptr;
    UserInfoPage  *userInfoPage = nullptr;
    bool          isLogin       = false  ;
    SqliteControl *sqlite       = nullptr;
};

#endif // ALLPAGELISTWIDGET_H
