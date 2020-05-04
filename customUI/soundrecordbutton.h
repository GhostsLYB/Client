#ifndef SOUNDRECORDBUTTON_H
#define SOUNDRECORDBUTTON_H

#include <QToolButton>

namespace Ui {
class SoundRecordButton;
}

class SoundRecordButton : public QToolButton
{
    Q_OBJECT

public:
    explicit SoundRecordButton(QWidget *parent = nullptr);
    ~SoundRecordButton();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    Ui::SoundRecordButton *ui;
};

#endif // SOUNDRECORDBUTTON_H
