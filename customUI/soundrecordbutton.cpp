#include "soundrecordbutton.h"
#include "ui_soundrecordbutton.h"

QString wavAudioFilePath = "/storage/emulated/0/IM/file/";
//"E:/always/IM/file/";
//QString wavAudioFilePath = GlobalDate::getGlobalFilePath();

SoundRecordButton::SoundRecordButton(QWidget *parent) :
    QToolButton(parent),
    ui(new Ui::SoundRecordButton),
    recorder(new QAudioRecorder(this))
{
    ui->setupUi(this);
    setText("按住 说话");
    QString str = recorder->defaultAudioInput();

    recorder->audioSettings().setBitRate(128000);
    recorder->audioSettings().setSampleRate(11025);

    qDebug() << "default input Device :" << str;
    int bitRate = recorder->audioSettings().bitRate();
    qDebug() << "default bit rate :" << bitRate;
    int sampleRate = recorder->audioSettings().sampleRate();
    qDebug() << "default sample rate :" << sampleRate;
}

void SoundRecordButton::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    qDebug() << "start sound recording ";
    setText("松开 send");
    savePath = wavAudioFilePath+getCurrentDataTime();
    qDebug() << "savePath = " << savePath;
    if(recorder->setOutputLocation(QUrl::fromLocalFile(savePath)))
        qDebug() << "set local file path success";
    else {
        qDebug() << "set local file Path fail";
    }

    QString str = recorder->defaultAudioInput();
    qDebug() << "default input Device :" << str;
    int bitRate = recorder->audioSettings().bitRate();
    qDebug() << "default bit rate :" << bitRate;
    int sampleRate = recorder->audioSettings().sampleRate();
    qDebug() << "default sample rate :" << sampleRate;

    recorder->record();
}

void SoundRecordButton::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    qDebug() << "sound record end and send the sound";
    setText("按住 说话");
    recorder->stop();
    qDebug() << "savePath = " << savePath;
    QFile file(savePath+".mp4");
    if(file.rename(savePath+".mp3"))
        qDebug() << "rename success";
    else {
        qDebug() << "rename fail";
    }
    savePath = savePath + ".mp3";
    file.setFileName(savePath);
    if(file.size() < 5*1024){  //音频文件太小不发送并删除文件
        qDebug() << "record sound too short";
        file.remove();
        return;
    }
//    QSound::play(savePath);
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


