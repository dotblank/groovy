/*
 * Copyright (C) 2010 Robin Burchell <robin.burchell@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <QMessageBox>
#include <QDebug>

#include "grooveclient.h"
#include "groovesong.h"
#include "groovesearchmodel.h"
#include "grooveplaylistmodel.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_searchModel(new GrooveSearchModel(this)),
    m_playlistModel(new GroovePlaylistModel(this)),
    m_mediaObject(new Phonon::MediaObject(this)),
    m_playBuffer(0),
    m_nowStreaming(false)
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

    connect(m_mediaObject, SIGNAL(finished()), SLOT(playNextSong()));
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
    fetchNextSong();
}

void MainWindow::fetchNextSong()
{
    // if we are currently downloading, or there is a song buffered but not played, don't prefetch
    // (so far, we can only stream one song at a time..)
    if (m_nowStreaming || m_preloadData.length()) {
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

    connect(song, SIGNAL(streamingStarted(QNetworkReply*)), SLOT(onStreamingStarted(QNetworkReply*)));
    song->startStreaming();
}

void MainWindow::onStreamingStarted(QNetworkReply *httpStream)
{
    qDebug() << Q_FUNC_INFO << "Streaming started... :)";

    connect(httpStream, SIGNAL(readyRead()), SLOT(onStreamReadReady()));
    connect(httpStream, SIGNAL(finished()), SLOT(onStreamingFinished()));
}

void MainWindow::onStreamReadReady()
{
    QIODevice *httpStream = qobject_cast<QIODevice *>(sender());
    if (GROOVE_VERIFY(httpStream, "no stream pointer from sender")) return;

    m_preloadData.append(httpStream->readAll());

    qDebug() << Q_FUNC_INFO << "Stream data length: " << m_preloadData.length();
}

void MainWindow::onStreamingFinished()
{
    qDebug() << Q_FUNC_INFO << "finished.";
    m_nowStreaming = false;

    if (m_mediaObject->state() != Phonon::PlayingState) {
        qDebug() << Q_FUNC_INFO << "Initiating play.";
        playNextSong();
    }
}

void MainWindow::playNextSong()
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
