#include "../include/music.h"

Music::Music(QObject *parent)
    : QObject(parent), isMuted(false)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/assets/music/music1.mp3"));

    audioOutput->setVolume(0.5);
    player->play();
}

void Music::play()
{
    player->play();
}

void Music::pause()
{
    player->pause();
}

void Music::toggleMute()
{
    isMuted = !isMuted;
    audioOutput->setMuted(isMuted);
}
