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
    void setUserInfo(QList<QString>&);
    QString getUserName();
    ~WidgetMine();

signals:
    void sigShowAllInfo(QString userName, int targetPage);
private slots:
    void on_btn_info_clicked();

    void on_btn_setup_clicked();

private:
    Ui::WidgetMine  *ui;
    QLabel          *lb_image;
    QLabel          *lb_userName;
    QLabel          *lb_number;
    QToolButton     *btn_info;
    QToolButton     *btn_modifyInfo;
    QToolButton     *btn_setup;
    QString         preNumber;
};

#endif // WIDGETMINE_H
