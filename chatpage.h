#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QDateTime>
#include "socket/control.h"
#include "database/sqlitecontrol.h"
#include "customUI/textchatinfoitem.h"
#include "customUI/soundrecordbutton.h"
#include "globaldate.h"

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
    void sigSend(ChatInfo);
    void sigRecv(ChatInfo);

public slots:
    void onRecvMessage(QString msg, int flag);
    void onRecordFinish(QString);

private:
    void onSend();
    void addToListWidget(const QString&,const int&,const QString&,const QString&,const QString&,const QString&);

public:
    QToolButton * btn_back;
    QLabel      * lb_friendName;
    QToolButton * btn_info;
    QListWidget * listWidget;
    QTextEdit   * te_sendBox;
    QPushButton * btn_send;
    QToolButton * btn_voiceSend;
    QToolButton * btn_expression;
    QToolButton * btn_otherSend;
    SoundRecordButton * btn_soundRecord;

private slots:
    void on_btn_voiceSend_clicked();

    void on_btn_expression_clicked();

    void on_btn_soundRecord_clicked();

    void on_btn_otherSend_clicked();

private:
    Ui::ChatPage *ui;
    Control      *ctrl = nullptr;
    int msgType = 3;    //发送消息的类型，默认为3表示文字消息
    QString sendFilePath = "";
};

#endif // CHATPAGE_H
