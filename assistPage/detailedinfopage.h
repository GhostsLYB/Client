#ifndef DETAILEDINFOPAGE_H
#define DETAILEDINFOPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QTabBar>

#include <socket/control.h>
#include <socket/socketcontrol.h>

namespace Ui {
class DetailedInfoPage;
}

class DetailedInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit DetailedInfoPage(Control *ctrl, QWidget *parent = nullptr);
    void setCurrentPage(int index);
    int getCurrentPage();
    ~DetailedInfoPage();

signals:
    void sigBackToMine();

private slots:
    void on_btn_cancle_clicked();

private:
    Ui::DetailedInfoPage *ui;
    Control *ctrl;
    QTabWidget *tab_wdiget;
//    int currentPage = 0;//标识当前页面0：allInfo 1:modifyInfo 2:setup
};

#endif // DETAILEDINFOPAGE_H
