#include "soundrecordbutton.h"
#include "ui_soundrecordbutton.h"

SoundRecordButton::SoundRecordButton(QWidget *parent) :
    QToolButton(parent),
    ui(new Ui::SoundRecordButton)
{
    ui->setupUi(this);
}

SoundRecordButton::~SoundRecordButton()
{
    delete ui;
}
