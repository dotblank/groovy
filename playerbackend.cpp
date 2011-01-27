
// Qt
#include <QDebug>

// Us
#include "playerbackend.h"
#include <QtCore/QTime>

PlayerBackend::PlayerBackend(QObject *parent, GrooveSearchModel *searchModel)
    : QObject(parent)
    , m_searchModel(searchModel)
    , m_mediaObject(new Phonon::MediaObject(this))
    , m_playBuffer(0)
    , m_nowStreaming(false)
    , m_isPlaying(false)
{
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(m_mediaObject, audioOutput);
    m_mediaObject->setTickInterval(1000);

    connect(m_mediaObject, SIGNAL(finished()), SLOT(playNextSong()));
    connect(m_mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            SLOT(onStateChanged(Phonon::State)));
    connect(m_mediaObject, SIGNAL(tick(qint64)), SIGNAL(positionChanged()));
    connect(m_mediaObject, SIGNAL(totalTimeChanged(qint64)), SIGNAL(albumChanged()));
}

bool PlayerBackend::isPlaying()
{
    return m_isPlaying;
}

QUrl PlayerBackend::albumArt()
{
    GrooveSong *song = m_playlistModel.currentTrack();
    return (song ? song->coverArtUrl() : QUrl());
}

qint64 PlayerBackend::totalTimeMS()
{
    return m_mediaObject->totalTime();
}

QString PlayerBackend::totalTime()
{
    QTime qt = QTime().addMSecs(m_mediaObject->totalTime());
    return qt.toString(QLatin1String("mm:ss"));
}

qint64 PlayerBackend::currentTimeMS()
{
    return m_mediaObject->currentTime();
}

QString PlayerBackend::currentTime()
{
    QTime qt = QTime().addMSecs(m_mediaObject->currentTime());
    return qt.toString(QLatin1String("mm:ss"));
}

QString PlayerBackend::currentTimeLeft()
{
    QTime qt = QTime().addMSecs(m_mediaObject->totalTime() - m_mediaObject->currentTime());
    return qt.toString(QLatin1String("-mm:ss"));
}

// TODO: passing QModelIndex directly from QML won't work
// Error: Unknown method parameter type: QModelIndex
void PlayerBackend::queueSong(int index)
{
    GrooveSong *song = m_searchModel->songByIndex(m_searchModel->index(index, 0, QModelIndex()));
    qDebug() << Q_FUNC_INFO << "Queueing " << song->songName();

    m_playlistModel.append(song);
    fetchNextSong();
}

void PlayerBackend::stopSong()
{
    m_mediaObject->stop();
}

void PlayerBackend::pausePlaySong()
{
    if (m_isPlaying)
        m_mediaObject->pause();
    else
        m_mediaObject->play();
}

void PlayerBackend::fetchNextSong()
{
    // if we are currently downloading, or there is a song buffered but not played, don't prefetch
    // (so far, we can only stream one song at a time..)
    if (m_nowStreaming || m_preloadData.length()) {
        qDebug() << Q_FUNC_INFO << "Already streaming or we already prefetched, ignoring fetch";
        return;
    }

    GrooveSong *song = m_playlistModel.next();

    if (!song) {
        // end of the playlist, for now!
        qDebug() << Q_FUNC_INFO << "End of playlist.";
        return;
    }

    qDebug() << Q_FUNC_INFO << "Fetching " << song->songName();

    m_nowStreaming = true;
    // TODO: handle error case to reset m_nowStreaming

    connect(song, SIGNAL(streamingStarted(QIODevice*)), SLOT(onStreamingStarted(QIODevice*)));
    song->startStreaming();
}

void PlayerBackend::onStreamingStarted(QIODevice *httpStream)
{
    qDebug() << Q_FUNC_INFO << "Streaming started... :)";

    connect(httpStream, SIGNAL(readyRead()), SLOT(onStreamReadReady()));
    connect(httpStream, SIGNAL(finished()), SLOT(onStreamingFinished()));
}

void PlayerBackend::onStreamReadReady()
{
    QIODevice *httpStream = qobject_cast<QIODevice *>(sender());
    if (GROOVE_VERIFY(httpStream, "no stream pointer from sender")) return;

    m_preloadData.append(httpStream->readAll());

    static int callNumber = 0;
    if (callNumber++ % 100 == 0)
        qDebug() << Q_FUNC_INFO << "Stream data length: " << m_preloadData.length();
}

void PlayerBackend::onStreamingFinished()
{
    qDebug() << Q_FUNC_INFO << "finished.";
    m_nowStreaming = false;

    if (m_mediaObject->state() != Phonon::PlayingState) {
        qDebug() << Q_FUNC_INFO << "Initiating play.";
        playNextSong();
    }
}

void PlayerBackend::onStateChanged(Phonon::State newstate)
{
    m_isPlaying = (newstate == Phonon::PlayingState);
    emit statusChanged();
}

void PlayerBackend::playNextSong()
{
    // if there is no data to play, or the stream is still downloading, don't do anything here
    if (m_preloadData.length() == 0 || m_nowStreaming) {
        qDebug() << Q_FUNC_INFO << "No data to play or stream is still downloading.";
        qDebug() << Q_FUNC_INFO << m_nowStreaming << " and data length is " << m_preloadData.length();
        return;
    }

    qDebug() << Q_FUNC_INFO << "Swapping buffers, and playing, buffer size: " << m_preloadData.size();

    // swap buffer
    m_playData = m_preloadData;

    // clear preload ready for next stream
    m_preloadData.clear();

    // tell Phonon about the buffer swap
    delete m_playBuffer;
    m_playBuffer = new QBuffer(&m_playData, this);
    m_mediaObject->enqueue(Phonon::MediaSource(m_playBuffer));
    m_mediaObject->play();
    emit albumChanged();

    // initiate next prefetch
    fetchNextSong();
}

void PlayerBackend::seekTo(qint64 newTime)
{
    m_mediaObject->seek(newTime);
}
