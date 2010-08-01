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

#include <QMessageBox>
#include <QDebug>

#include "grooveclient.h"
#include "groovesong.h"
#include "groovesearchmodel.h"
#include "grooveplaylistmodel.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

class GrooveStream : public Phonon::AbstractMediaStream
{
    friend class MainWindow;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_searchModel(new GrooveSearchModel(this)),
    m_playlistModel(new GroovePlaylistModel(this)),
    m_mediaObject(new Phonon::MediaObject(this)),
    m_currentStream(0)
{
    GrooveClient::instance()->establishConnection();
    connect(GrooveClient::instance(), SIGNAL(connected()), SLOT(onConnected()));

    m_ui->setupUi(this);
    m_ui->searchResults->setModel(m_searchModel);
    m_ui->playlistView->setModel(m_playlistModel);

    connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(onSearchButtonPress()));
    connect(m_ui->searchResults, SIGNAL(doubleClicked(QModelIndex)), SLOT(onQueueSong(QModelIndex)));

    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(m_mediaObject, audioOutput);

    connect(m_mediaObject, SIGNAL(finished()), SLOT(advanceToNextSongInPlaylist()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::onConnected()
{
   // QMessageBox::information(NULL, "Connected!", "Groovy is now connected.");
}

void MainWindow::onSearchButtonPress()
{
    qDebug() << Q_FUNC_INFO << "Searching for " << m_ui->lineEdit->text();
    m_searchModel->searchBySong(m_ui->lineEdit->text());
}

void MainWindow::onQueueSong(const QModelIndex &index)
{
    GrooveSong *song = m_searchModel->songByIndex(index);
    qDebug() << Q_FUNC_INFO << "Queueing " << song->songName();

    m_playlistModel->append(song);

    if (m_currentStream == 0)
        advanceToNextSongInPlaylist();
}

void MainWindow::advanceToNextSongInPlaylist()
{
    GrooveSong *song = m_playlistModel->next();

    if (!song) {
        // end of the playlist, for now!
        return;
    }

    qDebug() << Q_FUNC_INFO << "Playing " << song->songName();

    connect(song, SIGNAL(streamingStarted(QNetworkReply*)), SLOT(onStreamingStarted(QNetworkReply*)));
    song->startStreaming();
}

void MainWindow::onStreamingStarted(QNetworkReply *httpStream)
{
    qDebug() << Q_FUNC_INFO << "Streaming started... :)";
    m_mediaObject->stop();
    m_audioBuffer.buffer().clear();

    connect(httpStream, SIGNAL(readyRead()), SLOT(onStreamReadReady()));
    connect(httpStream, SIGNAL(finished()), SLOT(onStreamingFinished()));

    m_currentStream = httpStream;
}

void MainWindow::onStreamReadReady()
{
    //qDebug() << Q_FUNC_INFO << "onStreamReadReady! buffer size is " << m_audioBuffer.buffer().length();
    QIODevice *httpStream = qobject_cast<QIODevice *>(sender());
    Q_ASSERT(httpStream);
    if (!httpStream) {
        qWarning() << Q_FUNC_INFO << "No stream pointer from sender";
        return;
    }

    //qDebug() << Q_FUNC_INFO << "Buffer length is now " << m_audioBuffer.buffer().length();
    m_audioBuffer.buffer().append(httpStream->readAll());
}

void MainWindow::onStreamingFinished()
{
    qDebug() << Q_FUNC_INFO << "finished.";

    if (m_mediaObject->state() != Phonon::PlayingState && m_mediaObject->state() != Phonon::BufferingState) {
        m_mediaObject->setCurrentSource(Phonon::MediaSource(&m_audioBuffer));
        m_mediaObject->play();
    }

    m_currentStream = 0;
}
