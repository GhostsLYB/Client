#ifndef HOMEPAGETOP_H
#define HOMEPAGETOP_H

#include <QWidget>
#include <QToolButton>

namespace Ui {
class HomePageTop;
}

class HomePageTop : public QWidget
{
    Q_OBJECT

public:
    explicit HomePageTop(QWidget *parent = nullptr);
    ~HomePageTop();
    void setWord(QString);

signals:
    void sigSearch();

private:
    Ui::HomePageTop *ui;
    QToolButton * btn_word = nullptr;
    QToolButton * btn_search = nullptr;
};

#endif // HOMEPAGETOP_H
