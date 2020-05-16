#ifndef DETAILEDINFOPAGE_H
#define DETAILEDINFOPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QSpacerItem>
#include <QList>
#include <QFileDialog>
#include <QCheckBox>

#include "socket/control.h"
#include "socket/socketcontrol.h"
#include "database/sqlitecontrol.h"
#include "customUI/longbutton.h"
#include "../globaldate.h"
namespace Ui {
class DetailedInfoPage;
}

class DetailedInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit DetailedInfoPage(Control *ctrl, SqliteControl *sqlite, QWidget *parent = nullptr);
    void setCurrentPage(int index, QString userName, bool isReFresh = false);
    int getCurrentPage();
    ~DetailedInfoPage();

private:
    void initAllInfoPage();

signals:
    void sigBackToMine();

private slots:
    void on_btn_cancle_clicked();
    void onToModifyPage(QString btnNameText);

    void on_btn_save_clicked();

private:
    Ui::DetailedInfoPage *ui;
    Control *ctrl;
    SqliteControl *sqlite;
    QTabWidget *tab_wdiget;
    QLabel *lb_tabTitle;
    QToolButton *btn_save;
    QList<QString> mineData;
    QString currUserName = "";
    //allInfo page
    LongButton * lbtn_image;
    LongButton * lbtn_userName;
    LongButton * lbtn_number;
    LongButton * lbtn_address;
    LongButton * lbtn_personalSignature;
    LongButton * lbtn_phone;
    //modify page
    QLabel * lb_tip;
    QLineEdit * le_infoInput;
};

#endif // DETAILEDINFOPAGE_H
