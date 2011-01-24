#ifndef PLAYERBACKEND_H
#define PLAYERBACKEND_H

#include <QObject>
#include <QModelIndex>
#include <QBuffer>
#include <phonon>
#include <QNetworkReply>

#include <grooveplaylistmodel.h>
#include <groovesearchmodel.h>

class PlayerBackend : public QObject
{
    Q_OBJECT
public:
    explicit PlayerBackend(QObject *parent, GrooveSearchModel *searchModel);

public slots:
    void queueSong(int index);
    void playNextSong();

private slots:
    void fetchNextSong();
    void onStreamingStarted(QNetworkReply *);
    void onStreamReadReady();
    void onStreamingFinished();

private:
    GrooveSearchModel *m_searchModel;
    GroovePlaylistModel m_playlistModel;
    Phonon::MediaObject *m_mediaObject;
    QBuffer *m_playBuffer;
    QByteArray m_preloadData;
    QByteArray m_playData;

    bool m_nowStreaming;
};

#endif // PLAYERBACKEND_H
