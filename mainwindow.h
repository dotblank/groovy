/*
 * This file is part of groovy
 *
 * Copyright (C) 2010 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, including documentation, is protected by copyright.
 * All rights are reserved.
 *
 * Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of the author.
 *
 * This material may also contain confidential information which may not be
 * disclosed to others without the prior written consent of the author.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QBuffer>
#include <phonon>
#include <QNetworkReply>

class GrooveSearchModel;
class GroovePlaylistModel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void onConnected();
    void onSearchButtonPress();
    void onQueueSong(const QModelIndex &index);
    void advanceToNextSongInPlaylist();
    void onStreamingStarted(QNetworkReply *);
    void onStreamReadReady();
    void onStreamingFinished();

private:
    Ui::MainWindow *m_ui;
    GrooveSearchModel *m_searchModel;
    GroovePlaylistModel *m_playlistModel;
    Phonon::MediaObject *m_mediaObject;
    QBuffer m_audioBuffer;

    // we don't own this!
    QNetworkReply *m_currentStream;
};

#endif // MAINWINDOW_H