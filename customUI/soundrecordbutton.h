#ifndef SOUNDRECORDBUTTON_H
#define SOUNDRECORDBUTTON_H

#include <QToolButton>
#include <QDebug>

#include <QAudioRecorder>
#include <QSound>
#include <QUrl>
#include <QDateTime>
#include "../globaldate.h"

namespace Ui {
class SoundRecordButton;
}

class SoundRecordButton : public QToolButton
{
    Q_OBJECT

public:
    explicit SoundRecordButton(QWidget *parent = nullptr);
    ~SoundRecordButton();
    QString getCurrentDataTime();

signals:
    void sigRecordFinish(QString);
    void sigRequestFile(QString fileName, QString savePath);
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    Ui::SoundRecordButton *ui;
    QAudioRecorder * recorder;
    QString savePath;
};

#endif // SOUNDRECORDBUTTON_H
