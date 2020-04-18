#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QTabBar>
#include <QFileDialog>
#include "widgetmain.h"
#include "widgetfriend.h"
#include "homepagetop.h"
#include "control.h"

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(Control * parentTrol = nullptr, QWidget *parent = nullptr);
    ~HomePage();
    void onItemClicked(QString);
    void setIndex(int index);
    void setTopShow();
signals:
    void sigExit();
    void sigChatWith(QString);

private slots:
    void on_btn_exit_clicked();

    void on_btn_sendFile_clicked();
private:
    Ui::HomePage *ui;
    WidgetMain * wid_main = nullptr;
    WidgetFriend * wid_friend = nullptr;
    Control * ctrl = nullptr;
    QString filePath;
};

#endif // HOMEPAGE_H
