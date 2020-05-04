#ifndef TEXTCHATINFOITEM_H
#define TEXTCHATINFOITEM_H

#include <QWidget>
#include <QBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QPixmap>

namespace Ui {
class TextChatInfoItem;
}

class TextChatInfoItem : public QWidget
{
    Q_OBJECT

public:
    explicit TextChatInfoItem(QWidget *parent = nullptr,QString imagePath = "",
                              QString info = "",bool isSend = true);
    ~TextChatInfoItem();

private:
    Ui::TextChatInfoItem *ui;
    QVBoxLayout * imageLayout;
    QVBoxLayout * infoLayout;
    QTextEdit * te_info;
    QLabel * lb_image;
};

#endif // TEXTCHATINFOITEM_H
