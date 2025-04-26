#ifndef VOLUMEDIALOG_H
#define VOLUMEDIALOG_H

#include <QDialog>
#include <QStyle>
#include <QMediaPlayer>

namespace Ui {
class VolumeDialog;
}

class VolumeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VolumeDialog(QWidget *parent = nullptr, int volume = 50);
    ~VolumeDialog();

private slots:
    void on_volumeSlider_sliderMoved(int position);

    void on_playButton_clicked();

    void on_buttonsBottom_accepted();

private:
    Ui::VolumeDialog *ui;
    QMediaPlayer *player;
    int volumeDialog;

signals:
    void sendVolume(int volume);
};

#endif // VOLUMEDIALOG_H
