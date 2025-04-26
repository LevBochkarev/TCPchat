#include "emojiwindow.h"
#include "ui_emojiwindow.h"


EmojiWindow::EmojiWindow(QWidget *parent, QPushButton* pushButton) :
    QDialog(parent),
    ui(new Ui::EmojiWindow)
{
    ui->setupUi(this);
    //Qt::SplashScreen |
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setupEmojiTable();
    int hTable = pushButton->y() - height();
    int wTable = pushButton->x() - width()/1.5;
    move(wTable,hTable);
    //qDebug()<<hTable<<wTable;
}

EmojiWindow::~EmojiWindow()
{
    delete ui;
}

void EmojiWindow::setupEmojiTable()
{
    QGridLayout* Layout = new QGridLayout(this);
    for(int j = 0; j < 3; j++){
        for(int i = 0; i < 17; i++){
            QString number = QString::number(j*17 + i + 11);
            if(number.size() == 1){
                number = "0" + number;
            }
            QString html = "&#1285" + number;
            Layout->addWidget(createEmojiButton(html), i, j);
        }
    }
    QWidget* widget1 = new QWidget(this);
    widget1->setLayout(Layout);

    QScrollArea* area = new QScrollArea(this);
    area->setWidget(widget1);
    area->resize(360, 360);
}

QPushButton* EmojiWindow::createEmojiButton(QString html)
{
    QPushButton* new_PushButton = new QPushButton(this);

    QFont new_font = new_PushButton->font();
    new_font.setPointSize(32);
    new_PushButton->setFont(new_font);

    new_PushButton->setMaximumSize(100,100);
    new_PushButton->setMinimumSize(100,100);

    QTextDocument doc;
    doc.setHtml(html);
    QString emoji_str = doc.toPlainText();
    new_PushButton->setText(emoji_str);

    QTextEdit* chatbox = parentWidget()->findChild<QTextEdit*>("textEditSend");

    connect(new_PushButton, &QPushButton::clicked, chatbox, [=]{chatbox->insertHtml(html);});

    return new_PushButton;
}
