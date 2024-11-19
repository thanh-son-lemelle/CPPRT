#ifndef MUSIC_H
#define MUSIC_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class Music : public QObject
{
    Q_OBJECT

public:
    explicit Music(QObject *parent = nullptr);
    void play();
    void pause();
    void toggleMute();

private:
    QMediaPlayer *player;
    bool isMuted;
    QAudioOutput *audioOutput;
};

#endif // MUSIC_H
