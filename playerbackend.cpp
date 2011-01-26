
// Qt
#include <QDebug>

// Us
#include "playerbackend.h"

PlayerBackend::PlayerBackend(QObject *parent, GrooveSearchModel *searchModel)
    : QObject(parent)
    , m_searchModel(searchModel)
    , m_mediaObject(new Phonon::MediaObject(this))
    , m_playBuffer(0)
    , m_nowStreaming(false)
{
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(m_mediaObject, audioOutput);

    connect(m_mediaObject, SIGNAL(finished()), SLOT(playNextSong()));
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

    // initiate next prefetch
    fetchNextSong();
}
