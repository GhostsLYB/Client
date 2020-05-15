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
    lb_image->setFixedSize(QSize(48,48));
    lb_image->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    imageLayout->addWidget(lb_image);
    imageLayout->setMargin(0);
    imageLayout->setSpacing(0);

    infoLayout = new QVBoxLayout(this);
    te_info = ui->te_info;
    te_info->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_info->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_info->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    //自适应高度
    connect(te_info->document(),&QTextDocument::contentsChanged,
            this,&TextChatInfoItem::onContentsChanged);
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
//    te_info->setFixedSize(QSize(200,130));
    if(flag == 8){ //图片
//        te_info->setFixedSize(QSize(200,300));
    }
    if(flag == 9){ //文件
//        te_info->setFixedSize(QSize(200,130));
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

int TextChatInfoItem::getHeight()
{
    return te_info->document()->size().height();
}

void TextChatInfoItem::onContentsChanged()
{
    QTextDocument *document=qobject_cast<QTextDocument*>(sender());
    document->adjustSize();
    if(document){
        QTextEdit *editor=qobject_cast<QTextEdit*>(document->parent()->parent());
        if (editor){
            int newHeight = document->size().rheight() + 6;
            if (newHeight != editor->height()){
                editor->setFixedHeight(newHeight);
            }
            int newWidth = document->size().rwidth() + 6;
            if (newWidth != editor->width()){
                qDebug() << "height:" << newHeight << " width:" << newWidth;
                editor->setFixedWidth(300);
            }
        }
    }
}

void TextChatInfoItem::mouseReleaseEvent(QMouseEvent *)
{
    qDebug() << "TextChatInfoItem audioPath = " << filePath;
    if(!filePath.isEmpty()){
        if(QFile::exists(filePath)){
            QMediaPlayer * player = new QMediaPlayer;
            player->setMedia(QUrl::fromLocalFile(filePath));
            player->setVolume(50);
            player->play();
//            QSound::play(filePath);
        }
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
