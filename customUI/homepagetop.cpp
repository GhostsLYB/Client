#include "homepagetop.h"
#include "ui_homepagetop.h"
#include <QBoxLayout>
#include <QToolButton>
#include <QSizePolicy>

HomePageTop::HomePageTop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePageTop)
{
    ui->setupUi(this);

    btn_word = new QToolButton(this);
    btn_word->setObjectName("btn_Word");
    btn_word->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    btn_word->setAutoRaise(true);
    btn_word->setText("趣聊");
    btn_search = new QToolButton(this);
    btn_search->setObjectName("btn_serach");
    btn_search->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    btn_search->setAutoRaise(true);
    btn_search->setText("搜索");
    //btn_serach->setIconSize(QSize(30,30));
    //btn_serach->setIcon(QIcon("/icon/2/png"));
    btn_addOption = new QToolButton(this);
    btn_addOption->setObjectName("btn_addOption");
    btn_addOption->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    btn_addOption->setAutoRaise(true);
    btn_addOption->setText("添加");
    //btn_addOption->setIconSize(QSize(30,30));
    //btn_addOption->setIcon(QIcon("/icon/2/png"));
    ui->hLayout->addWidget(btn_word,2);
    ui->hLayout->addStretch(4);
    ui->hLayout->addWidget(btn_search,1);
    ui->hLayout->addWidget(btn_addOption,1);
}

void HomePageTop::setWord(QString word)
{
    btn_word->setText(word);
}

HomePageTop::~HomePageTop()
{
    delete ui;
}
