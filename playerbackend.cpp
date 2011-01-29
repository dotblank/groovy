
// Qt
#include <QDebug>

// Us
#include "playerbackend.h"
#include <QtCore/QTime>

PlayerBackend::PlayerBackend(QObject *parent, GrooveSearchModel *searchModel)
    : QObject(parent)
    , m_searchModel(searchModel)
    , m_playlistModel(new GroovePlaylistModel(this))
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_nowStreaming(false)
    , m_isPlaying(false)
{
    connect(m_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(onStateChanged(QMediaPlayer::State)));

//    connect(m_mediaObject, SIGNAL(tick(qint64)), SIGNAL(positionChanged()));
//    connect(m_mediaObject, SIGNAL(totalTimeChanged(qint64)), SIGNAL(albumChanged()));
}

bool PlayerBackend::isPlaying()
{
    return m_isPlaying;
}

QUrl PlayerBackend::albumArt()
{
    GrooveSong *song = m_playlistModel->currentTrack();
    return (song ? song->coverArtUrl() : QUrl());
}

qint64 PlayerBackend::totalTimeMS()
{
    return m_mediaPlayer->duration();
}

QString PlayerBackend::totalTime()
{
    QTime qt = QTime().addMSecs(m_mediaPlayer->duration());
    return qt.toString(QLatin1String("mm:ss"));
}

qint64 PlayerBackend::currentTimeMS()
{
    return m_mediaPlayer->position();
}

QString PlayerBackend::currentTime()
{
    QTime qt = QTime().addMSecs(m_mediaPlayer->position());
    return qt.toString(QLatin1String("mm:ss"));
}

QString PlayerBackend::currentTimeLeft()
{
    QTime qt = QTime().addMSecs(m_mediaPlayer->duration() - m_mediaPlayer->position());
    return qt.toString(QLatin1String("-mm:ss"));
}

// TODO: passing QModelIndex directly from QML won't work
// Error: Unknown method parameter type: QModelIndex
void PlayerBackend::queueSong(int index)
{
    GrooveSong *song = m_searchModel->songByIndex(m_searchModel->index(index, 0, QModelIndex()));
    qDebug() << Q_FUNC_INFO << "Queueing " << song->songName();

    m_playlistModel->append(song);

    if (!m_isPlaying)
        fetchNextSong();
}

void PlayerBackend::stopSong()
{
    m_mediaPlayer->stop();
}

void PlayerBackend::pausePlaySong()
{
    if (m_mediaPlayer->state() == QMediaPlayer::PlayingState)
        m_mediaPlayer->pause();
    else
        m_mediaPlayer->play();
}

void PlayerBackend::fetchNextSong()
{
    // if we are currently downloading, or there is a song buffered but not played, don't prefetch
    // (so far, we can only stream one song at a time..)
    if (m_nowStreaming) {
        qDebug() << Q_FUNC_INFO << "Already streaming or we already prefetched, ignoring fetch";
        return;
    }

    GrooveSong *song = m_playlistModel->next();

    if (!song) {
        // end of the playlist, for now!
        qDebug() << Q_FUNC_INFO << "End of playlist.";
        return;
    }

    qDebug() << Q_FUNC_INFO << "Fetching " << song->songName();

    m_nowStreaming = true;
    // TODO: handle error case to reset m_nowStreaming

    connect(song, SIGNAL(streamingStarted(QIODevice*)), SLOT(onStreamingStarted(QIODevice*)), Qt::UniqueConnection);
    song->startStreaming();
}

void PlayerBackend::onStreamingStarted(QIODevice *httpStream)
{
    qDebug() << Q_FUNC_INFO << "Streaming started... :)";

    m_mediaPlayer->setMedia(QUrl("test"), httpStream);
    m_mediaPlayer->play();
    connect(httpStream, SIGNAL(readyRead()), SLOT(onStreamReadReady()));
    connect(httpStream, SIGNAL(finished()), SLOT(onStreamingFinished()));
}

void PlayerBackend::onStreamReadReady()
{
    static int callNumber = 0;
    if (callNumber++ % 100 == 0)
        qDebug() << Q_FUNC_INFO << "Streaming";
}

void PlayerBackend::onStreamingFinished()
{
    qDebug() << Q_FUNC_INFO << "finished.";
    m_nowStreaming = false;

    if (m_mediaPlayer->state() == QMediaPlayer::StoppedState) {
        qDebug() << Q_FUNC_INFO << "Initiating play.";
        fetchNextSong();
    }
}

void PlayerBackend::onStateChanged(QMediaPlayer::State newstate)
{
   m_isPlaying = (newstate == QMediaPlayer::PlayingState);

   if (!m_isPlaying) {
       fetchNextSong();
   }

   emit statusChanged();
}

void PlayerBackend::seekTo(qint64 newTime)
{
    m_mediaPlayer->setPosition(newTime);
}

GroovePlaylistModel *PlayerBackend::playlistModel() const
{
    return m_playlistModel;
}

void PlayerBackend::skipTo(int trackIndex)
{
    stopSong();
    m_nowStreaming = false;
    m_mediaPlayer->setMedia(QMediaContent());
    m_playlistModel->setCurrentTrackIndex(trackIndex);
    fetchNextSong();
}
