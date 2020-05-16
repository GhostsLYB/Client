#ifndef WIDGETFRIEND_H
#define WIDGETFRIEND_H

#include <QWidget>
#include <QListWidget>
#include <QDebug>
#include <QMap>

#include "customUI/friendlistitem.h"
#include "../globaldate.h"
#include "../customUI/customlistwidget.h"

namespace Ui {
class WidgetFriend;
}

class WidgetFriend : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetFriend(QWidget *parent = nullptr);
    ~WidgetFriend();
    void initFriendList(QMap<QString,QString> &map = GlobalDate::getFriendNameImageMap());

signals:
    void sigItemClicked(QString);

private slots:
    void onItemClicked(QString);

private:
    Ui::WidgetFriend *ui;
    CustomListWidget * listWidget = nullptr;
};

#endif // WIDGETFRIEND_H
