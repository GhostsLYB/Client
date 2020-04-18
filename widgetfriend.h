#ifndef WIDGETFRIEND_H
#define WIDGETFRIEND_H

#include <QWidget>
#include <QListWidget>

namespace Ui {
class WidgetFriend;
}

class WidgetFriend : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetFriend(QWidget *parent = nullptr);
    ~WidgetFriend();

private:
    Ui::WidgetFriend *ui;
    QListWidget * listWidget = nullptr;
};

#endif // WIDGETFRIEND_H
