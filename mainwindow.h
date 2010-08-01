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
    void fetchNextSong();
    void playNextSong();
    void onStreamingStarted(QNetworkReply *);
    void onStreamReadReady();
    void onStreamingFinished();

private:
    Ui::MainWindow *m_ui;
    GrooveSearchModel *m_searchModel;
    GroovePlaylistModel *m_playlistModel;
    Phonon::MediaObject *m_mediaObject;
    QBuffer *m_playBuffer;
    QByteArray m_preloadData;
    QByteArray m_playData;

    bool m_nowStreaming;
};

#endif // MAINWINDOW_H
