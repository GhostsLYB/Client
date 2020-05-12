#ifndef USERINFOPAGE_H
#define USERINFOPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QToolButton>
#include <QMessageBox>
#include "socket/control.h"
#include "database/sqlitecontrol.h"
#include "globaldate.h"

namespace Ui {
class UserInfoPage;
}

class UserInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfoPage(Control *ctrl, SqliteControl *sqlite, QWidget *parent = nullptr);
    void setBackPage(int page){backPage = page;}
    int getBackPage(){return backPage;}
    void setUserName(QString);
    QString getUserName();
    void setUserInfo(QList<QString>&);//设置用户信息界面的信息
    ~UserInfoPage();

signals:
    void sigFriendList();
    void sigChatWith(QString);
    void sigDeleteFriend();
    void sigShowHomePage();

private slots:
    void on_btn_cancle_clicked();

    void on_btn_sendMsg_clicked();

    void on_btn_deleteFriend_clicked();

private:
    Ui::UserInfoPage    *ui;
    Control             *ctrl;
    SqliteControl       *sqlite;
    QToolButton         *btn_cancle;
    QLabel              *lb_image;
    QLabel              *lb_remark;
    QLabel              *lb_userName;
    QLabel              *lb_number;
    QLabel              *lb_address;
    QLabel              *lb_personalSignature;
    QLabel              *lb_phone;
    QLabel              *lb_dynamic1;
    QLabel              *lb_dynamic2;
    QLabel              *lb_dynamic3;
    QLabel              *lb_dynamic4;
    QToolButton         *btn_sendMsg;
    QToolButton         *btn_deleteFriend;
    int                 backPage;   //返回界面，0：通讯录 1：聊天界面
    QString             perUserName;
    QString             perNumber;
    QString             perAddress;
};

#endif // USERINFOPAGE_H
