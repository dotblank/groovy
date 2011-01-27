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

#include <QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeView>

#include "grooveclient.h"
#include "groovesong.h"
#include "groovesearchmodel.h"

#include "playerbackend.h"
#include "qtcachingnetworkaccessmanagerfactory.h"

int main(int argc, char **argv)
{
    QApplication qca(argc, argv);
    qca.setApplicationName("Groovy");

//    MainWindow mw;
//    mw.show();
    GrooveClient::instance()->establishConnection();

    GrooveSearchModel searchModel;
    PlayerBackend playerBackend(0, &searchModel);

    QDeclarativeView view;
    QtCachingNetworkAccessManagerFactory qtcnamf;
    view.engine()->setNetworkAccessManagerFactory(&qtcnamf);
    QDeclarativeContext *context = view.rootContext();
    context->setContextProperty("searchModel", &searchModel);
    context->setContextProperty("playerBackend", &playerBackend);

    view.setSource(QUrl::fromLocalFile("mainwindow.qml"));
    view.show();
//     QObject *object = view.rootObject();

    qca.exec();
}
