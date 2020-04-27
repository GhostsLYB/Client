#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QListWidget>
#include "homepagetop.h"
#include "customlistwidgetitem.h"

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMain(QWidget *parent = nullptr);
    ~WidgetMain();

signals:
    void sigItemClicked(QString);

public slots:
//    void onAddFriendItem(QString image, int msgNum,QString friendName,QString msg);
    void onItemClicked(QString);

private:
    Ui::WidgetMain *ui;
    QListWidget * listWidget = nullptr;
};

#endif // WIDGETMAIN_H
