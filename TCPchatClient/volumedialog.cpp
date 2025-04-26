#include "volumedialog.h"
#include "ui_volumedialog.h"

VolumeDialog::VolumeDialog(QWidget *parent, int volume) :
    QDialog(parent),
    ui(new Ui::VolumeDialog)
{
    volumeDialog = volume;
    ui->setupUi(this);
    ui->VolumeLabel->setNum(volumeDialog);
    ui->volumeSlider->setValue(volumeDialog);
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    player = new QMediaPlayer;
    player->setMedia(QUrl("E:/work/3sem/3/UDPchat/notification1.wav"));
    player->setVolume(volumeDialog);
}

VolumeDialog::~VolumeDialog()
{
    delete ui;
}

void VolumeDialog::on_volumeSlider_sliderMoved(int position)
{
    volumeDialog = position;
    player->setVolume(volumeDialog);
}


void VolumeDialog::on_playButton_clicked()
{
    player->stop();
    player->play();
}


void VolumeDialog::on_buttonsBottom_accepted()
{
    emit sendVolume(volumeDialog);
    close();
    destroy();
}

