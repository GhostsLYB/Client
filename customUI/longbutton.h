#ifndef LONGBUTTON_H
#define LONGBUTTON_H

#include <QWidget>
#include <QToolButton>
#include <QMouseEvent>
#include <QIcon>
#include <QSize>

#include <QDebug>

namespace Ui {
class LongButton;
}

class LongButton : public QWidget
{
    Q_OBJECT

public:
    explicit LongButton(QWidget *parent = nullptr);
    void setBtnNameIcon(QString path);
    void setBtnInfoIcon(QString path);
    void setBtnNameIconSize(QSize);
    void setBtnInfoIconSize(QSize);
    void setBtnNameText(QString name){btn_name->setText(name);}
    QString getBtnNameText(){return btn_name->text();}
    void setBtnInfoText(QString info){btn_info->setText(info);}
    QString getBtnInfoText(){return btn_info->text();}
    void setLineShow(bool topShow, bool bottomShow);
    ~LongButton();

signals:
    void sigClicked(QString btnNameText);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::LongButton *ui;
    QToolButton *btn_name;    //自定义按钮的左侧名称（可以是图标+文字格式）
    QToolButton *btn_info;    //自定义按钮的右侧信息（可以是图标+文字格式）
};

#endif // LONGBUTTON_H
