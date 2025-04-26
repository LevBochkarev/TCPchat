#ifndef EMOJIWINDOW_H
#define EMOJIWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QTextDocument>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
//#include "mainwindow.h"
#include <QTextEdit>

namespace Ui {
class EmojiWindow;
}

class EmojiWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EmojiWindow(QWidget *parent = nullptr, QPushButton* button = nullptr);
    ~EmojiWindow();

private:
    Ui::EmojiWindow *ui;
    void setupEmojiTable();
    QPushButton* createEmojiButton(QString html);

};

#endif // EMOJIWINDOW_H
