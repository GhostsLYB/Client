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
    lb_image->setFixedSize(QSize(60,60));
    lb_image->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    imageLayout->addWidget(lb_image);
    imageLayout->setMargin(0);
    imageLayout->setSpacing(0);

    infoLayout = new QVBoxLayout(this);
    te_info = ui->te_info;
    te_info->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_info->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    te_info->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    te_info->setFixedWidth(600);
    //布局
    QSpacerItem *spacerItem = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    if(isSend){ //发送信息       //设置布局
        layout->addSpacerItem(spacerItem);
        layout->addLayout(infoLayout);
        layout->addLayout(imageLayout);
//        setLayoutDirection(Qt::RightToLeft);
    }
    else {      //接收信息s
        layout->addLayout(imageLayout);
        layout->addLayout(infoLayout);
        layout->addSpacerItem(spacerItem);
//        setLayoutDirection(Qt::LeftToRight);
    }
    this->setLayout(layout);
    layout->setMargin(0);
    layout->setSpacing(0);
    //自适应高度
    connect(te_info->document(),&QTextDocument::contentsChanged,
            this,&TextChatInfoItem::onContentsChanged);
    infoLayout->addWidget(te_info);
    infoLayout->setMargin(8);
    QPixmap pix;
    if(imagePath != "")
        pix.load(imagePath);
    else
        pix.load(":/icon/head_picture/2.png");
    pix = pix.scaled(60,60);
    lb_image->setPixmap(pix);   //设置头像
    te_info->append(info);      //设置信息
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
            editor->setFixedWidth(document->size().width()+100);

//            int newWidth = document->size().rwidth() + 14;
//            if (newWidth != editor->width()){
//                qDebug() << "height:" << newHeight << " width:" << newWidth;
//                editor->setFixedWidth(newWidth);
//            }
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
            player->setVolume(100);
            player->play();
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
