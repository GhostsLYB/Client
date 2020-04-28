#ifndef WIDGETMINE_H
#define WIDGETMINE_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>

namespace Ui {
class WidgetMine;
}

class WidgetMine : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMine(QWidget *parent = nullptr);
    void setUserName(QString);
    ~WidgetMine();

private:
    Ui::WidgetMine  *ui;
    QLabel          *lb_image;
    QLabel          *lb_userName;
    QLabel          *lb_number;
    QToolButton     *btn_info;
    QToolButton     *btn_modifyInfo;
    QToolButton     *btn_setup;
};

#endif // WIDGETMINE_H
