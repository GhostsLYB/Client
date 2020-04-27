#ifndef WIDGETFRIEND_H
#define WIDGETFRIEND_H

#include <QWidget>
#include <QListWidget>
#include <QDebug>
#include "customUI/friendlistitem.h"

namespace Ui {
class WidgetFriend;
}

class WidgetFriend : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetFriend(QWidget *parent = nullptr);
    ~WidgetFriend();

signals:
    void sigItemClicked(QString);

private slots:
    void onItemClicked(QString);

private:
    Ui::WidgetFriend *ui;
    QListWidget * listWidget = nullptr;
};

#endif // WIDGETFRIEND_H
