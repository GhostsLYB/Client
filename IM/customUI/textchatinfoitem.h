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
#include <QFileInfo>

namespace Ui {
class TextChatInfoItem;
}

class TextChatInfoItem : public QWidget
{
    Q_OBJECT

public:
    explicit TextChatInfoItem(QWidget *parent = nullptr,QString imagePath = "",
                              QString info = "",bool isSend = true,int flag = 3);
    void setFilePath(QString path){filePath = path;}
    QString getFilePath(){return filePath;}

    ~TextChatInfoItem();

signals:
    void sigRequestDownloadFile(QString filePath);

protected:
    void mouseReleaseEvent(QMouseEvent*);

private:
    Ui::TextChatInfoItem *ui;
    QVBoxLayout * imageLayout;
    QVBoxLayout * infoLayout;
    QTextEdit * te_info;
    QLabel * lb_image;

    QString filePath = "";
    int flag;
};

#endif // TEXTCHATINFOITEM_H
