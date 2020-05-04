#ifndef FRIENDLISTITEM_H
#define FRIENDLISTITEM_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QLine>

namespace Ui {
class FriendListItem;
}

class FriendListItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendListItem(QWidget *parent = nullptr);
    ~FriendListItem();
    int getIndex(){return index;}
    void setIndex(int i){index = i;}
    void setFriendName(QString s){lb_friendName->setText(s);}
    QString getFriendName(){return lb_friendName->text();}
    void setPicture(QString);
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:

signals:
    void  isItemClicked(QString);
private:
    Ui::FriendListItem *ui;
    int index;
    bool isMousePressed = false;
    QLabel * lb_image;
    QLabel * lb_friendName;
};

#endif // FRIENDLISTITEM_H
