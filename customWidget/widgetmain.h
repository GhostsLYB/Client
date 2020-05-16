#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QListWidget>
#include "customUI/homepagetop.h"
#include "customUI/customlistwidgetitem.h"
#include "../customUI/customlistwidget.h"

namespace Ui {
class WidgetMain;
}

class WidgetMain : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMain(QWidget *parent = nullptr);
    ~WidgetMain();
    int getItemNum(){return listWidget->count();}

signals:
    void sigItemClicked(QString);

public slots:
    void onAddFriendItem(QString image,QString msgNum,QString friendName,
                         QString lastMsg,QString lastTime,QString voice);
    void onCleanAllItems();
    void onItemClicked(QString);

private:
    Ui::WidgetMain *ui;
    CustomListWidget * listWidget = nullptr;
};

#endif // WIDGETMAIN_H
