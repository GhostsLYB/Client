#ifndef ALLPAGELISTWIDGET_H
#define ALLPAGELISTWIDGET_H

#include <QWidget>
#include <QTabBar>
#include "homepage.h"
#include "login.h"
#include "register.h"
#include "chatpage.h"
#include "control.h"
#include "sqlitecontrol.h"

#include <QJsonObject>
#include <QJsonValue>

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
    bool setLoginState(bool state){isLogin = state;}
public slots:
    void onChatWith(QString);
    void onUserInfo(QString);
    void onLoginSuccessed(QString);
    void onChatMsgInert(ChatInfo);
private:
    Ui::AllPageListWidget *ui;
    Control     *ctrl       = nullptr;
    HomePage    *homepage   = nullptr;
    Login       *login      = nullptr;
    Register    *regist     = nullptr;
    ChatPage    *chatPage   = nullptr;
    bool        isLogin     = false;
    SqliteControl * sqlite  = nullptr;
};

#endif // ALLPAGELISTWIDGET_H
