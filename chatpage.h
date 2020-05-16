#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QDateTime>
#include <QFileDialog>

#include "socket/control.h"
#include "database/sqlitecontrol.h"
#include "customUI/textchatinfoitem.h"
#include "customUI/soundrecordbutton.h"
#include "customUI/customlistwidget.h"
#include "globaldate.h"

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(Control * parentCtrl = nullptr,
                      SqliteControl *sqlite = nullptr,
                      QWidget *parent = nullptr);
    ~ChatPage();

    void setFriendName(QString friendName){lb_friendName->setText(friendName);}
    QString getFriendName(){return lb_friendName->text();}
    void initInfo(QList<ChatInfo> *list);

signals:
    void sigSend(ChatInfo);
    void sigRecv(ChatInfo);
    void sigRequestDownloadFile(QString fileName);

public slots:
    void onRecvMessage(QString msg, int flag);
    void onRecordFinish(QString);
    void onDownloadFile(QString filePath)
    {
        emit sigRequestDownloadFile(filePath);
    }

private:
    void onSend(int msgFlag = -1);
    void addToListWidget(const QString&,const int&,const QString&,const QString&,const QString&,const QString&);

public:
    QToolButton * btn_back;
    QLabel      * lb_friendName;
//    QToolButton * btn_info;
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

    void on_btn_p1_clicked();

    void on_btn_p2_clicked();

    void on_btn_p3_clicked();

    void on_btn_sendPicture_clicked();

    void on_btn_sendFile_clicked();

private:
    Ui::ChatPage *ui;
    Control      *ctrl = nullptr;
    SqliteControl *sqlite;
    int msgType = 3;    //发送消息的类型，默认为3表示文字消息
    QString sendFilePath = "";
};

#endif // CHATPAGE_H
