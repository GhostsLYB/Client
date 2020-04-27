#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QDateTime>
#include "control.h"
#include "sqlitecontrol.h"

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(Control * parentCtrl = nullptr,QWidget *parent = nullptr);
    ~ChatPage();

    void setFriendName(QString friendName){lb_friendName->setText(friendName);}
    QString getFriendName(){return lb_friendName->text();}
    void initInfo(QList<ChatInfo> *list);
signals:
    void isClosed();
    void sigSend(ChatInfo);
    void sigRecv(ChatInfo);
public slots:
    void onRecvMessage(QString);
protected:
    void closeEvent(QCloseEvent *e);
private:
    void onSend();
    void addToListWidget(const QString&,const int&,const QString&,const QString&,const QString&,const QString&);

public:
    QPushButton * btn_back;
    QLabel * lb_friendName;
    QPushButton * btn_info;
    QListWidget * listWidget;
    QTextEdit * te_sendBox;
    QPushButton * btn_send;
private:
    Ui::ChatPage *ui;
    Control *ctrl = nullptr;

};

#endif // CHATPAGE_H
