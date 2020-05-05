#ifndef TEXTCHATINFOITEM_H
#define TEXTCHATINFOITEM_H

#include <QWidget>
#include <QBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QPixmap>
#include <QSound>
#include <QDebug>

namespace Ui {
class TextChatInfoItem;
}

class TextChatInfoItem : public QWidget
{
    Q_OBJECT

public:
    explicit TextChatInfoItem(QWidget *parent = nullptr,QString imagePath = "",
                              QString info = "",bool isSend = true);
    void setAudioPath(QString path){audioPath = path;}
    QString getAudioPath(){return audioPath;}

    ~TextChatInfoItem();

protected:
    void mouseReleaseEvent(QMouseEvent*);

private:
    Ui::TextChatInfoItem *ui;
    QVBoxLayout * imageLayout;
    QVBoxLayout * infoLayout;
    QTextEdit * te_info;
    QLabel * lb_image;

    QString audioPath = "";
};

#endif // TEXTCHATINFOITEM_H
