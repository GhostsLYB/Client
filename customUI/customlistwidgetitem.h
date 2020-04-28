#ifndef CUSTOMLISTWIDGETITEM_H
#define CUSTOMLISTWIDGETITEM_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>

namespace Ui {
class CustomListWidgetItem;
}

class CustomListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit CustomListWidgetItem(QWidget *parent = nullptr);
    ~CustomListWidgetItem();
    int getIndex(){return index;}
    void setIndex(int i){index = i;}
    void setFriendName(QString s){lb_friendName->setText(s);}
    QString getFriendName(){return lb_friendName->text();}
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void  isItemClicked(QString);

private:
    Ui::CustomListWidgetItem *ui;
    int index;
    bool isMousePressed = false;
    QLabel * lb_image;
    QLabel * lb_messageNumber;
    QLabel * lb_friendName;
    QLabel * lb_lastTime;
    QLabel * lb_lastMessage;
    QLabel * lb_voiceIcon;
};

#endif // CUSTOMLISTWIDGETITEM_H
