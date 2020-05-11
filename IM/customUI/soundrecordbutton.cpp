#include "soundrecordbutton.h"
#include "ui_soundrecordbutton.h"

#define PI_osd die
//QString wavAudioFilePath = QString("e:/always/audio/");

QString wavAudioFilePath = QString("e:/always/IM/file/");

SoundRecordButton::SoundRecordButton(QWidget *parent) :
    QToolButton(parent),
    ui(new Ui::SoundRecordButton),
    recorder(new QAudioRecorder(this))
{
    ui->setupUi(this);
    setText("按住 说话");
    recorder->audioSettings().setBitRate(8000);
    recorder->audioSettings().setSampleRate(8000);
}

void SoundRecordButton::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    qDebug() << "start sound recording ";
    setText("松开 send");
    savePath = wavAudioFilePath+getCurrentDataTime()+".wav";
    qDebug() << "savePath = " << savePath;
    recorder->setOutputLocation(QUrl::fromLocalFile(savePath));
    recorder->record();
}

void SoundRecordButton::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    qDebug() << "sound record end and send the sound";
    setText("按住 说话");
    recorder->stop();
    qDebug() << "savePath = " << savePath;
    QSound::play(savePath);
    emit sigRecordFinish(savePath);
}

QString SoundRecordButton::getCurrentDataTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    return dt.toString("yyyyMMddhhmmss");
}

SoundRecordButton::~SoundRecordButton()
{
    delete ui;
}


