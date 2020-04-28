#ifndef HOMEPAGEMENU_H
#define HOMEPAGEMENU_H

#include <QWidget>

namespace Ui {
class HomePageMenu;
}

class HomePageMenu : public QWidget
{
    Q_OBJECT

public:
    explicit HomePageMenu(QWidget *parent = nullptr);
    ~HomePageMenu();

signals:
    void btn_pageClicked(int);

public slots:
    void onPage1(){emit btn_pageClicked(0);}
    void onPage2(){emit btn_pageClicked(1);}
    void onPage3(){emit btn_pageClicked(2);}
    void onPage4(){emit btn_pageClicked(3);}
private:
    Ui::HomePageMenu *ui;
};

#endif // HOMEPAGEMENU_H
