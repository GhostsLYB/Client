#include "textchatinfoitem.h"
#include "ui_textchatinfoitem.h"

//默认为发送消息，isSend为false时为接收消息
TextChatInfoItem::TextChatInfoItem(QWidget *parent,QString imagePath,
                                   QString info,bool isSend, int flag) :
    QWidget(parent),
    ui(new Ui::TextChatInfoItem),
    flag(flag)
{
    ui->setupUi(this);
    QHBoxLayout * layout = new QHBoxLayout(this);

    imageLayout = new QVBoxLayout(this);
    lb_image = ui->lb_image;
    lb_image->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    imageLayout->addWidget(lb_image);
    imageLayout->setMargin(0);
    imageLayout->setSpacing(0);

    infoLayout = new QVBoxLayout(this);
    te_info = ui->te_info;
    te_info->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_info->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_info->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    infoLayout->addWidget(te_info);
    infoLayout->setMargin(8);
    QSpacerItem *spacerItem = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    QPixmap pix;
    if(imagePath != "")
        pix.load(imagePath);
    else
        pix.load(":/icon/head_picture/2.png");
    lb_image->setPixmap(pix);   //设置头像
    te_info->append(info);      //设置信息
    te_info->setFixedSize(QSize(100,24));
    if(flag == 8){ //图片
        te_info->setFixedSize(QSize(100,150));
    }
    if(flag == 9){ //文件
        te_info->setFixedSize(QSize(100,38));
    }
    if(isSend){ //发送信息       //设置布局
        layout->addSpacerItem(spacerItem);
        layout->addLayout(infoLayout);
        layout->addLayout(imageLayout);
    }
    else {      //接收信息s
        layout->addLayout(imageLayout);
        layout->addLayout(infoLayout);
        layout->addSpacerItem(spacerItem);
    }
    this->setLayout(layout);
    layout->setMargin(0);
    layout->setSpacing(0);


}

void TextChatInfoItem::mouseReleaseEvent(QMouseEvent *)
{
    qDebug() << "TextChatInfoItem audioPath = " << filePath;
    if(!filePath.isEmpty()){
        QFileInfo fileInfo(filePath);
        if(fileInfo.isFile())
            QSound::play(filePath);
        else {
            QString fileName = filePath.mid(filePath.lastIndexOf('/')+1);
            qDebug() << "请求文件下载" <<fileName;
            emit sigRequestDownloadFile(filePath);
        }
    }
}

TextChatInfoItem::~TextChatInfoItem()
{
    delete ui;
}
