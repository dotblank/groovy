/*
 * Copyright (C) 2011 Robin Burchell <robin.burchell@collabora.co.uk>
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

// Qt
#include <QtNetwork>
#include <QDesktopServices>

// Us
#include "qtcachingnetworkaccessmanagerfactory.h"

QtCachingNetworkAccessManagerFactory::QtCachingNetworkAccessManagerFactory() :
    QDeclarativeNetworkAccessManagerFactory()
{
}

// Thanks to Lauri Jaaskela for this.
// http://blogs.forum.nokia.com/blog/lauri-jaaskelas-forum-nokia-blog/2011/01/11/qml-network-access-caching
QNetworkAccessManager *QtCachingNetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(parent);
    QNetworkDiskCache *diskCache = new QNetworkDiskCache(parent);

    // Get a default system directory for storing cache data
    QString dataPath =
        QDesktopServices::storageLocation(QDesktopServices::CacheLocation);

    // Make sure that the directory exists
    QDir().mkpath(dataPath);
    diskCache->setCacheDirectory(dataPath);

    // Set cache size to max 2 megabytes
    diskCache->setMaximumCacheSize(2*1024*1024);
    manager->setCache(diskCache);
    return manager;
}
