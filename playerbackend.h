#ifndef PLAYERBACKEND_H
#define PLAYERBACKEND_H

#include <QMediaPlayer>
#include <QObject>
#include <QModelIndex>
#include <QNetworkReply>

#include <grooveplaylistmodel.h>
#include <groovesearchmodel.h>

class PlayerBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY statusChanged)
    Q_PROPERTY(QUrl albumArt READ albumArt NOTIFY albumChanged)
    Q_PROPERTY(qint64 totalTimeMS READ totalTimeMS NOTIFY albumChanged)
    Q_PROPERTY(QString totalTime READ totalTime NOTIFY albumChanged)
    Q_PROPERTY(qint64 currentTimeMS READ currentTimeMS NOTIFY positionChanged)
    Q_PROPERTY(QString currentTime READ currentTime NOTIFY positionChanged)
    Q_PROPERTY(QString currentTimeLeft READ currentTimeLeft NOTIFY positionChanged)

public:
    explicit PlayerBackend(QObject *parent, GrooveSearchModel *searchModel);
    bool isPlaying();
    QUrl albumArt();
    qint64 totalTimeMS();
    QString totalTime();
    qint64 currentTimeMS();
    QString currentTime();
    QString currentTimeLeft();
    GroovePlaylistModel *playlistModel() const;

public slots:
    void queueSong(int index);
    void skipTo(int trackIndex);
    void stopSong();
    void pausePlaySong();
    void seekTo(qint64 newTime);

signals:
    void statusChanged();
    void albumChanged();
    void positionChanged();

private slots:
    void fetchNextSong();
    void onStreamingStarted(QIODevice *);
    void onStreamReadReady();
    void onStreamingFinished();
    void onStateChanged(QMediaPlayer::State newstate);

private:
    GrooveSearchModel *m_searchModel;
    GroovePlaylistModel *m_playlistModel;
    QMediaPlayer *m_mediaPlayer;

    bool m_nowStreaming;
    bool m_isPlaying;
};

#endif // PLAYERBACKEND_H
